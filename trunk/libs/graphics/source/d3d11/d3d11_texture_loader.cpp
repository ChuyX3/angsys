#include "pch.h"
#include "d3d11/d3d11_driver.h"

#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#else
#define NEW new
#endif

#if DIRECTX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;

//--------------------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------------------
#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((ang_uint32_t)(ang_uint8_t)(ch0) | ((ang_uint32_t)(ang_uint8_t)(ch1) << 8) |       \
                ((ang_uint32_t)(ang_uint8_t)(ch2) << 16) | ((ang_uint32_t)(ang_uint8_t)(ch3) << 24 ))
#endif /* defined(MAKEFOURCC) */


#pragma pack(push,1) // Byte alignment (8-bit)

const ang_uint32_t DDS_MAGIC = 0x20534444; // "DDS "

struct DDS_PIXELFORMAT
{
	ang_uint32_t    size;
	ang_uint32_t    flags;
	ang_uint32_t    fourCC;
	ang_uint32_t    RGBBitCount;
	ang_uint32_t    RBitMask;
	ang_uint32_t    GBitMask;
	ang_uint32_t    BBitMask;
	ang_uint32_t    ABitMask;
};

#define DDS_FOURCC      0x00000004  // DDPF_FOURCC
#define DDS_RGB         0x00000040  // DDPF_RGB
#define DDS_LUMINANCE   0x00020000  // DDPF_LUMINANCE
#define DDS_ALPHA       0x00000002  // DDPF_ALPHA

#define DDS_HEADER_FLAGS_VOLUME         0x00800000  // DDSD_DEPTH

#define DDS_HEIGHT 0x00000002 // DDSD_HEIGHT
#define DDS_WIDTH  0x00000004 // DDSD_WIDTH

#define DDS_CUBEMAP_POSITIVEX 0x00000600 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
#define DDS_CUBEMAP_NEGATIVEX 0x00000a00 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
#define DDS_CUBEMAP_POSITIVEY 0x00001200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
#define DDS_CUBEMAP_NEGATIVEY 0x00002200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
#define DDS_CUBEMAP_POSITIVEZ 0x00004200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
#define DDS_CUBEMAP_NEGATIVEZ 0x00008200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ

#define DDS_CUBEMAP_ALLFACES ( DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX |\
                               DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY |\
                               DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ )

#define DDS_CUBEMAP 0x00000200 // DDSCAPS2_CUBEMAP


enum DDS_MISC_FLAGS2
{
	DDS_MISC_FLAGS2_ALPHA_MODE_MASK = 0x7L,
};

struct DDS_HEADER
{
	ang_uint32_t        size;
	ang_uint32_t        flags;
	ang_uint32_t        height;
	ang_uint32_t        width;
	ang_uint32_t        pitchOrLinearSize;
	ang_uint32_t        depth; // only if DDS_HEADER_FLAGS_VOLUME is set in flags
	ang_uint32_t        mipMapCount;
	ang_uint32_t        reserved1[11];
	DDS_PIXELFORMAT		ddspf;
	ang_uint32_t        caps;
	ang_uint32_t        caps2;
	ang_uint32_t        caps3;
	ang_uint32_t        caps4;
	ang_uint32_t        reserved2;
};

struct DDS_HEADER_DXT10
{
	DXGI_FORMAT			dxgiFormat;
	ang_uint32_t        resourceDimension;
	ang_uint32_t        miscFlag; // see D3D11_RESOURCE_MISC_FLAG
	ang_uint32_t        arraySize;
	ang_uint32_t        miscFlags2;
};


struct TGA_HEADER
{
	ang_uint8_t  identsize;			// size of ID field that follows 18 byte header (0 usually)
	ang_uint8_t  colourmaptype;		// type of colour map 0=none, 1=has palette
	ang_uint8_t  imagetype;			// type of image 2=rgb uncompressed, 10 - rgb rle compressed

	ang_uint16_t colourmapstart;				// first colour map entry in palette
	ang_uint16_t colourmaplength;				// number of colours in palette
	ang_uint8_t  colourmapbits;		// number of bits per palette entry 15,16,24,32

	ang_uint16_t xstart;						// image x origin
	ang_uint16_t ystart;						// image y origin
	ang_uint16_t width;						// image width in pixels
	ang_uint16_t height;						// image height in pixels
	ang_uint8_t  bits;				// image bits per pixel 24,32
	ang_uint8_t  descriptor;			// image descriptor bits (vh flip bits)
										// pixel data follows header
};

#pragma pack(pop)

const int IT_COMPRESSED = 10;
const int IT_UNCOMPRESSED = 2;

bool operator == (D3D11_SUBRESOURCE_DATA const&, D3D11_SUBRESOURCE_DATA const&) { return false; }
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(D3D11_SUBRESOURCE_DATA);

//--------------------------------------------------------------------------------------
// Return the BPP for a particular format
//--------------------------------------------------------------------------------------
static wsize BitsPerPixel(_In_ DXGI_FORMAT fmt)
{
	switch (fmt)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		return 128;

	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		return 96;

	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
	case DXGI_FORMAT_Y416:
	case DXGI_FORMAT_Y210:
	case DXGI_FORMAT_Y216:
		return 64;

	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
	case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
	case DXGI_FORMAT_B8G8R8A8_TYPELESS:
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8X8_TYPELESS:
	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
	case DXGI_FORMAT_AYUV:
	case DXGI_FORMAT_Y410:
	case DXGI_FORMAT_YUY2:
		return 32;

	case DXGI_FORMAT_P010:
	case DXGI_FORMAT_P016:
		return 24;

	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
	case DXGI_FORMAT_B5G6R5_UNORM:
	case DXGI_FORMAT_B5G5R5A1_UNORM:
	case DXGI_FORMAT_A8P8:
	case DXGI_FORMAT_B4G4R4A4_UNORM:
		return 16;

	case DXGI_FORMAT_NV12:
	case DXGI_FORMAT_420_OPAQUE:
	case DXGI_FORMAT_NV11:
		return 12;

	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
	case DXGI_FORMAT_AI44:
	case DXGI_FORMAT_IA44:
	case DXGI_FORMAT_P8:
		return 8;

	case DXGI_FORMAT_R1_UNORM:
		return 1;

	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		return 4;

	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
	case DXGI_FORMAT_BC6H_TYPELESS:
	case DXGI_FORMAT_BC6H_UF16:
	case DXGI_FORMAT_BC6H_SF16:
	case DXGI_FORMAT_BC7_TYPELESS:
	case DXGI_FORMAT_BC7_UNORM:
	case DXGI_FORMAT_BC7_UNORM_SRGB:
		return 8;

#if defined(_XBOX_ONE) && defined(_TITLE)

	case DXGI_FORMAT_R10G10B10_7E3_A2_FLOAT:
	case DXGI_FORMAT_R10G10B10_6E4_A2_FLOAT:
		return 32;

	case DXGI_FORMAT_D16_UNORM_S8_UINT:
	case DXGI_FORMAT_R16_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X16_TYPELESS_G8_UINT:
		return 24;

#endif // _XBOX_ONE && _TITLE

	default:
		return 0;
	}
}


//--------------------------------------------------------------------------------------
// Get surface information for a particular format
//--------------------------------------------------------------------------------------
static void GetSurfaceInfo(_In_ wsize width,
	_In_ wsize height,
	_In_ DXGI_FORMAT fmt,
	_Out_opt_ wsize* outNumBytes,
	_Out_opt_ wsize* outRowBytes,
	_Out_opt_ wsize* outNumRows)
{
	wsize numBytes = 0;
	wsize rowBytes = 0;
	wsize numRows = 0;

	bool bc = false;
	bool packed = false;
	bool planar = false;
	wsize bpe = 0;
	switch (fmt)
	{
	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		bc = true;
		bpe = 8;
		break;

	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
	case DXGI_FORMAT_BC6H_TYPELESS:
	case DXGI_FORMAT_BC6H_UF16:
	case DXGI_FORMAT_BC6H_SF16:
	case DXGI_FORMAT_BC7_TYPELESS:
	case DXGI_FORMAT_BC7_UNORM:
	case DXGI_FORMAT_BC7_UNORM_SRGB:
		bc = true;
		bpe = 16;
		break;

	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
	case DXGI_FORMAT_YUY2:
		packed = true;
		bpe = 4;
		break;

	case DXGI_FORMAT_Y210:
	case DXGI_FORMAT_Y216:
		packed = true;
		bpe = 8;
		break;

	case DXGI_FORMAT_NV12:
	case DXGI_FORMAT_420_OPAQUE:
		planar = true;
		bpe = 2;
		break;

	case DXGI_FORMAT_P010:
	case DXGI_FORMAT_P016:
		planar = true;
		bpe = 4;
		break;

#if defined(_XBOX_ONE) && defined(_TITLE)

	case DXGI_FORMAT_D16_UNORM_S8_UINT:
	case DXGI_FORMAT_R16_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X16_TYPELESS_G8_UINT:
		planar = true;
		bpe = 4;
		break;

#endif
	}

	if (bc)
	{
		wsize numBlocksWide = 0;
		if (width > 0)
		{
			numBlocksWide = ang::max<wsize, wsize>(1, (width + 3) / 4);
		}
		wsize numBlocksHigh = 0;
		if (height > 0)
		{
			numBlocksHigh = ang::max<wsize, wsize>(1, (height + 3) / 4);
		}
		rowBytes = numBlocksWide * bpe;
		numRows = numBlocksHigh;
		numBytes = rowBytes * numBlocksHigh;
	}
	else if (packed)
	{
		rowBytes = ((width + 1) >> 1) * bpe;
		numRows = height;
		numBytes = rowBytes * height;
	}
	else if (fmt == DXGI_FORMAT_NV11)
	{
		rowBytes = ((width + 3) >> 2) * 4;
		numRows = height * 2; // Direct3D makes this simplifying assumption, although it is larger than the 4:1:1 data
		numBytes = rowBytes * numRows;
	}
	else if (planar)
	{
		rowBytes = ((width + 1) >> 1) * bpe;
		numBytes = (rowBytes * height) + ((rowBytes * height + 1) >> 1);
		numRows = height + ((height + 1) >> 1);
	}
	else
	{
		wsize bpp = BitsPerPixel(fmt);
		rowBytes = (width * bpp + 7) / 8; // round up to nearest byte
		numRows = height;
		numBytes = rowBytes * height;
	}

	if (outNumBytes)
	{
		*outNumBytes = numBytes;
	}
	if (outRowBytes)
	{
		*outRowBytes = rowBytes;
	}
	if (outNumRows)
	{
		*outNumRows = numRows;
	}
}


//--------------------------------------------------------------------------------------
#define ISBITMASK( r,g,b,a ) ( ddpf.RBitMask == r && ddpf.GBitMask == g && ddpf.BBitMask == b && ddpf.ABitMask == a )

static DXGI_FORMAT GetDXGIFormat(const DDS_PIXELFORMAT& ddpf)
{
	if (ddpf.flags & DDS_RGB)
	{
		// Note that sRGB formats are written using the "DX10" extended header

		switch (ddpf.RGBBitCount)
		{
		case 32:
			if (ISBITMASK(0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000))
			{
				return DXGI_FORMAT_R8G8B8A8_UNORM;
			}

			if (ISBITMASK(0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000))
			{
				return DXGI_FORMAT_B8G8R8A8_UNORM;
			}

			if (ISBITMASK(0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000))
			{
				return DXGI_FORMAT_B8G8R8X8_UNORM;
			}

			// No DXGI format maps to ISBITMASK(0x000000ff,0x0000ff00,0x00ff0000,0x00000000) aka D3DFMT_X8B8G8R8

			// Note that many common DDS reader/writers (including D3DX) swap the
			// the RED/BLUE masks for 10:10:10:2 formats. We assumme
			// below that the 'backwards' header mask is being used since it is most
			// likely written by D3DX. The more robust solution is to use the 'DX10'
			// header extension and specify the DXGI_FORMAT_R10G10B10A2_UNORM format directly

			// For 'correct' writers, this should be 0x000003ff,0x000ffc00,0x3ff00000 for RGB data
			if (ISBITMASK(0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000))
			{
				return DXGI_FORMAT_R10G10B10A2_UNORM;
			}

			// No DXGI format maps to ISBITMASK(0x000003ff,0x000ffc00,0x3ff00000,0xc0000000) aka D3DFMT_A2R10G10B10

			if (ISBITMASK(0x0000ffff, 0xffff0000, 0x00000000, 0x00000000))
			{
				return DXGI_FORMAT_R16G16_UNORM;
			}

			if (ISBITMASK(0xffffffff, 0x00000000, 0x00000000, 0x00000000))
			{
				// Only 32-bit color channel format in D3D9 was R32F
				return DXGI_FORMAT_R32_FLOAT; // D3DX writes this out as a FourCC of 114
			}
			break;

		case 24:
			// No 24bpp DXGI formats aka D3DFMT_R8G8B8
			break;

		case 16:
			if (ISBITMASK(0x7c00, 0x03e0, 0x001f, 0x8000))
			{
				return DXGI_FORMAT_B5G5R5A1_UNORM;
			}
			if (ISBITMASK(0xf800, 0x07e0, 0x001f, 0x0000))
			{
				return DXGI_FORMAT_B5G6R5_UNORM;
			}

			// No DXGI format maps to ISBITMASK(0x7c00,0x03e0,0x001f,0x0000) aka D3DFMT_X1R5G5B5

			if (ISBITMASK(0x0f00, 0x00f0, 0x000f, 0xf000))
			{
				return DXGI_FORMAT_B4G4R4A4_UNORM;
			}

			// No DXGI format maps to ISBITMASK(0x0f00,0x00f0,0x000f,0x0000) aka D3DFMT_X4R4G4B4

			// No 3:3:2, 3:3:2:8, or paletted DXGI formats aka D3DFMT_A8R3G3B2, D3DFMT_R3G3B2, D3DFMT_P8, D3DFMT_A8P8, etc.
			break;
		}
	}
	else if (ddpf.flags & DDS_LUMINANCE)
	{
		if (8 == ddpf.RGBBitCount)
		{
			if (ISBITMASK(0x000000ff, 0x00000000, 0x00000000, 0x00000000))
			{
				return DXGI_FORMAT_R8_UNORM; // D3DX10/11 writes this out as DX10 extension
			}

			// No DXGI format maps to ISBITMASK(0x0f,0x00,0x00,0xf0) aka D3DFMT_A4L4
		}

		if (16 == ddpf.RGBBitCount)
		{
			if (ISBITMASK(0x0000ffff, 0x00000000, 0x00000000, 0x00000000))
			{
				return DXGI_FORMAT_R16_UNORM; // D3DX10/11 writes this out as DX10 extension
			}
			if (ISBITMASK(0x000000ff, 0x00000000, 0x00000000, 0x0000ff00))
			{
				return DXGI_FORMAT_R8G8_UNORM; // D3DX10/11 writes this out as DX10 extension
			}
		}
	}
	else if (ddpf.flags & DDS_ALPHA)
	{
		if (8 == ddpf.RGBBitCount)
		{
			return DXGI_FORMAT_A8_UNORM;
		}
	}
	else if (ddpf.flags & DDS_FOURCC)
	{
		if (MAKEFOURCC('D', 'X', 'T', '1') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC1_UNORM;
		}
		if (MAKEFOURCC('D', 'X', 'T', '3') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC2_UNORM;
		}
		if (MAKEFOURCC('D', 'X', 'T', '5') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC3_UNORM;
		}

		// While pre-mulitplied alpha isn't directly supported by the DXGI formats,
		// they are basically the same as these BC formats so they can be mapped
		if (MAKEFOURCC('D', 'X', 'T', '2') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC2_UNORM;
		}
		if (MAKEFOURCC('D', 'X', 'T', '4') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC3_UNORM;
		}

		if (MAKEFOURCC('A', 'T', 'I', '1') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC4_UNORM;
		}
		if (MAKEFOURCC('B', 'C', '4', 'U') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC4_UNORM;
		}
		if (MAKEFOURCC('B', 'C', '4', 'S') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC4_SNORM;
		}

		if (MAKEFOURCC('A', 'T', 'I', '2') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC5_UNORM;
		}
		if (MAKEFOURCC('B', 'C', '5', 'U') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC5_UNORM;
		}
		if (MAKEFOURCC('B', 'C', '5', 'S') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC5_SNORM;
		}

		// BC6H and BC7 are written using the "DX10" extended header

		if (MAKEFOURCC('R', 'G', 'B', 'G') == ddpf.fourCC)
		{
			return DXGI_FORMAT_R8G8_B8G8_UNORM;
		}
		if (MAKEFOURCC('G', 'R', 'G', 'B') == ddpf.fourCC)
		{
			return DXGI_FORMAT_G8R8_G8B8_UNORM;
		}

		if (MAKEFOURCC('Y', 'U', 'Y', '2') == ddpf.fourCC)
		{
			return DXGI_FORMAT_YUY2;
		}

		// Check for D3DFORMAT enums being set here
		switch (ddpf.fourCC)
		{
		case 36: // D3DFMT_A16B16G16R16
			return DXGI_FORMAT_R16G16B16A16_UNORM;

		case 110: // D3DFMT_Q16W16V16U16
			return DXGI_FORMAT_R16G16B16A16_SNORM;

		case 111: // D3DFMT_R16F
			return DXGI_FORMAT_R16_FLOAT;

		case 112: // D3DFMT_G16R16F
			return DXGI_FORMAT_R16G16_FLOAT;

		case 113: // D3DFMT_A16B16G16R16F
			return DXGI_FORMAT_R16G16B16A16_FLOAT;

		case 114: // D3DFMT_R32F
			return DXGI_FORMAT_R32_FLOAT;

		case 115: // D3DFMT_G32R32F
			return DXGI_FORMAT_R32G32_FLOAT;

		case 116: // D3DFMT_A32B32G32R32F
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
	}

	return DXGI_FORMAT_UNKNOWN;
}


//--------------------------------------------------------------------------------------
static DXGI_FORMAT MakeSRGB(_In_ DXGI_FORMAT format)
{
	switch (format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	case DXGI_FORMAT_BC1_UNORM:
		return DXGI_FORMAT_BC1_UNORM_SRGB;

	case DXGI_FORMAT_BC2_UNORM:
		return DXGI_FORMAT_BC2_UNORM_SRGB;

	case DXGI_FORMAT_BC3_UNORM:
		return DXGI_FORMAT_BC3_UNORM_SRGB;

	case DXGI_FORMAT_B8G8R8A8_UNORM:
		return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;

	case DXGI_FORMAT_B8G8R8X8_UNORM:
		return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;

	case DXGI_FORMAT_BC7_UNORM:
		return DXGI_FORMAT_BC7_UNORM_SRGB;

	default:
		return format;
	}
}

bool FillInitDataFromDDS(ang::ibuffer_t in_data, tex_file_info_t const* header, wsize& twidth, wsize& theight, wsize& tdepth, wsize& skipMip, ang::array<D3D11_SUBRESOURCE_DATA>& out_data)
{
	using namespace ang;

	skipMip = 0;
	twidth = 0;
	theight = 0;
	tdepth = 0;

	wsize NumBytes = 0;
	wsize RowBytes = 0;
	const byte* pSrcBits = (byte*)in_data->buffer_ptr();
	const byte* pEndBits = (byte*)in_data->buffer_ptr() + in_data->buffer_size();

	out_data.size(header->arraySize * header->mipCount);

	wsize idx = 0;
	for (wsize j = 0; j < header->arraySize; j++)
	{
		wsize w = header->width;
		wsize h = header->height;
		wsize d = header->depth;
		for (wsize i = 0; i < header->mipCount; i++)
		{
			GetSurfaceInfo(w, h, (DXGI_FORMAT)header->format, &NumBytes, &RowBytes, nullptr);

			if (!twidth)
			{
				twidth = w;
				theight = h;
				tdepth = d;
			}

			//assert(idx < header->mipCount * header->arraySize);
			out_data[idx].pSysMem = (const void*)pSrcBits;
			out_data[idx].SysMemPitch = static_cast<uint>(RowBytes);
			out_data[idx].SysMemSlicePitch = static_cast<uint>(NumBytes);
			++idx;

			if (pSrcBits + (NumBytes*d) > pEndBits)
			{
				D3D11_SUBRESOURCE_DATA* p = null;
				out_data = p;
				return false;
			}

			pSrcBits += NumBytes * d;

			w = w >> 1;
			h = h >> 1;
			d = d >> 1;
			if (w == 0)
			{
				w = 1;
			}
			if (h == 0)
			{
				h = 1;
			}
			if (d == 0)
			{
				d = 1;
			}
		}
	}

	return (idx > 0) ? true : false;
}

void LoadCompressedTGAImage(byte* pDest, byte * pSrc, TGA_HEADER * pHeader)
{
	int w = pHeader->width;
	int h = pHeader->height;
	int rowSize = w * 4;// pHeader->bits / 8;
	bool bInverted = ((pHeader->descriptor & (1 << 5)) != 0);
	byte * pDestPtr = bInverted ? pDest + (h + 1) * rowSize : pDest;
	int countPixels = 0;
	int nPixels = w * h;

	while (nPixels > countPixels)
	{
		byte chunk = *pSrc++;
		if (chunk < 128)
		{
			int chunkSize = chunk + 1;
			for (int i = 0; i < chunkSize; i++)
			{
				if (bInverted && (countPixels % w) == 0)
					pDestPtr -= 2 * rowSize;
				*pDestPtr++ = pSrc[2];
				*pDestPtr++ = pSrc[1];
				*pDestPtr++ = pSrc[0];
				pSrc += 3;
				if (pHeader->bits != 24)
					*pDestPtr++ = *pSrc++;
				else *pDestPtr++ = 1;//ADDING ALPHA CHANEL
				countPixels++;
			}
		}
		else
		{
			int chunkSize = chunk - 127;
			for (int i = 0; i < chunkSize; i++)
			{
				if (bInverted && (countPixels % w) == 0)
					pDestPtr -= 2 * rowSize;
				*pDestPtr++ = pSrc[2];
				*pDestPtr++ = pSrc[1];
				*pDestPtr++ = pSrc[0];
				if (pHeader->bits != 24)
					*pDestPtr++ = pSrc[3];
				else *pDestPtr++ = 1;//ADDING ALPHA CHANEL
				countPixels++;
			}
			pSrc += (pHeader->bits >> 3);
		}
	}
}

void LoadUncompressedTGAImage(byte* pDest, byte * pSrc, TGA_HEADER * pHeader)
{
	int w = pHeader->width;
	int h = pHeader->height;
	int rowSize = w * pHeader->bits / 8;
	bool bInverted = ((pHeader->descriptor & (1 << 5)) != 0);
	for (int i = 0; i < h; i++)
	{
		byte * pSrcRow = pSrc +
			(bInverted ? (h - i - 1) * rowSize : i * rowSize);
		if (pHeader->bits == 24)
		{
			for (int j = 0; j < w; j++)
			{
				*pDest++ = pSrcRow[2];
				*pDest++ = pSrcRow[1];
				*pDest++ = pSrcRow[0];
				*pDest++ = -1;//ADDING ALPHA CHANEL
				pSrcRow += 3;
			}
		}
		else
		{
			for (int j = 0; j < w; j++)
			{
				*pDest++ = pSrcRow[2];
				*pDest++ = pSrcRow[1];
				*pDest++ = pSrcRow[0];
				*pDest++ = pSrcRow[3];
				pSrcRow += 4;
			}
		}
	}
}

pointer ExtractFaceFromBMP(byte* bmp, byte* temp, uint width, uint offsetX, uint offsetY)
{
	int w = width / 4;
	int startRead = offsetY * width * 4 + offsetX * 4;
	for (int i = 0; i < w; i++)
	{
		memcpy((temp + i * w * 4), (bmp + startRead), (w * 4));
		startRead += width * 4;
	}
	return temp;
}

bool FillInitDataFromTGA(ang::ibuffer_t in_data, d3d11::tex_file_info_t const* header, ang::array<D3D11_SUBRESOURCE_DATA>& init_data, ang::array<ang::ibuffer_t>& out_data)
{
	if (header->isCubeMap)
	{
		auto w = header->width / 4;

		out_data = ang::collections::array<ang::ibuffer_t>{
				new collections::array_buffer<byte>(4 * w * w),
				new collections::array_buffer<byte>(4 * w * w),
				new collections::array_buffer<byte>(4 * w * w),
				new collections::array_buffer<byte>(4 * w * w),
				new collections::array_buffer<byte>(4 * w * w),
				new collections::array_buffer<byte>(4 * w * w)
		};
		init_data.size(6); // = NEW ang::collections::array_buffer<D3D11_SUBRESOURCE_DATA>(6);

		memset(init_data.data(), 0, init_data.size());

		init_data[0].pSysMem = out_data[0]->buffer_ptr();
		init_data[0].SysMemPitch = header->width;
		init_data[1].pSysMem = out_data[1]->buffer_ptr();
		init_data[1].SysMemPitch = header->width;
		init_data[2].pSysMem = out_data[2]->buffer_ptr();
		init_data[2].SysMemPitch = header->width;
		init_data[3].pSysMem = out_data[3]->buffer_ptr();
		init_data[3].SysMemPitch = header->width;
		init_data[4].pSysMem = out_data[4]->buffer_ptr();
		init_data[4].SysMemPitch = header->width;
		init_data[5].pSysMem = out_data[5]->buffer_ptr();
		init_data[5].SysMemPitch = header->width;

		ExtractFaceFromBMP((byte*)in_data->buffer_ptr(), (byte*)out_data[0]->buffer_ptr(), header->width, 2 * w, w);
		ExtractFaceFromBMP((byte*)in_data->buffer_ptr(), (byte*)out_data[1]->buffer_ptr(), header->width, 0, w);
		ExtractFaceFromBMP((byte*)in_data->buffer_ptr(), (byte*)out_data[2]->buffer_ptr(), header->width, w, 0);
		ExtractFaceFromBMP((byte*)in_data->buffer_ptr(), (byte*)out_data[3]->buffer_ptr(), header->width, w, 2 * w);
		ExtractFaceFromBMP((byte*)in_data->buffer_ptr(), (byte*)out_data[4]->buffer_ptr(), header->width, w, w);
		ExtractFaceFromBMP((byte*)in_data->buffer_ptr(), (byte*)out_data[5]->buffer_ptr(), header->width, 3 * w, w);

	}
	else
	{
		init_data.size(1); // = NEW ang::collections::array_buffer<D3D11_SUBRESOURCE_DATA>(1);
		init_data[0].SysMemSlicePitch = 0;
		init_data[0].pSysMem = in_data->buffer_ptr();
		init_data[0].SysMemPitch = header->width * 4;
		out_data = { in_data };
	}
	return true;
}

ang::ibuffer_t d3d11_texture_loader::load_dds(ang::core::files::input_binary_file_t file, d3d11::tex_file_info_t& dds_header)
{
	using namespace ang;

	ang::ibuffer_t outData = null;
	wsize dataSize = (wsize)file->size();
	int offset = sizeof(DDS_HEADER) + sizeof(ang_uint32_t);
	if (dataSize < (wsize)offset)
		return null;

	file->read([&](streams::ibinary_input_stream_t stream)->error
	{

		ang_uint32_t dwMagicNumber;
		DDS_HEADER header;
		stream >> dwMagicNumber;
		stream >> header;

		if (dwMagicNumber != DDS_MAGIC
			|| header.size != sizeof(DDS_HEADER)
			|| header.ddspf.size != sizeof(DDS_PIXELFORMAT))
			return error_code::failed;

		bool bDXT10Header = false;
		if ((header.ddspf.flags & DDS_FOURCC) && (MAKEFOURCC('D', 'X', '1', '0') == header.ddspf.fourCC))
		{
			// Must be long enough for both headers and magic value
			if (dataSize < (offset + sizeof(DDS_HEADER_DXT10)))
				return false;
			offset += sizeof(DDS_HEADER_DXT10);
			bDXT10Header = true;
		}

		dds_header.width = header.width;
		dds_header.height = header.height;
		dds_header.depth = header.depth;
		dds_header.resDim = D3D11_RESOURCE_DIMENSION_UNKNOWN;
		dds_header.arraySize = 1;
		dds_header.isCubeMap = false;

		dds_header.mipCount = header.mipMapCount;
		if (dds_header.mipCount > D3D11_REQ_MIP_LEVELS)
			return error_code::failed;
		if (0 == dds_header.mipCount)
			dds_header.mipCount = 1;

		dds_header.format = GetDXGIFormat(header.ddspf);
		if (dds_header.format == DXGI_FORMAT_UNKNOWN)
			return error_code::failed;

		if (header.flags & DDS_HEADER_FLAGS_VOLUME)
		{
			dds_header.resDim = D3D11_RESOURCE_DIMENSION_TEXTURE3D;
		}
		else
		{
			if (header.caps2 & DDS_CUBEMAP)
			{
				// We require all six faces to be defined
				if ((header.caps2 & DDS_CUBEMAP_ALLFACES) != DDS_CUBEMAP_ALLFACES)
				{
					return error_code::failed;
				}
				dds_header.arraySize = 6;
				dds_header.isCubeMap = true;
			}
			dds_header.depth = 1;
			dds_header.resDim = D3D11_RESOURCE_DIMENSION_TEXTURE2D;
		}

		switch (dds_header.resDim)
		{
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
			if (dds_header.isCubeMap)
			{
				// This is the right bound because we set arraySize to (NumCubes*6) above
				if ((dds_header.arraySize > D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION) ||
					(dds_header.width > D3D11_REQ_TEXTURECUBE_DIMENSION) ||
					(dds_header.height > D3D11_REQ_TEXTURECUBE_DIMENSION))
					return error_code::failed;
			}
			else if ((dds_header.arraySize > D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION) ||
				(dds_header.width > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION) ||
				(dds_header.height > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION))
				return error_code::failed;
			break;

		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
			if ((dds_header.arraySize > 1) ||
				(dds_header.width > D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION) ||
				(dds_header.height > D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION) ||
				(dds_header.depth > D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION))
				return error_code::failed;
			break;
		}

		outData = new collections::array_buffer<byte>(dataSize - offset);
		stream->read(outData->buffer_ptr(), outData->buffer_size());
		return error_code::success;
	});

	return outData;
}

ang::ibuffer_t d3d11_texture_loader::load_tga(ang::core::files::input_binary_file_t file, uint& width, uint& height, uint& format)
{
	ang::ibuffer_t out_data = null;
	file->read([&](ang::streams::ibinary_input_stream_t stream)->error
	{
		TGA_HEADER header;
		stream >> header;
		if (header.imagetype != IT_COMPRESSED && header.imagetype != IT_UNCOMPRESSED)
			return error_code::failed;
		if (header.bits != 24 && header.bits != 32)
			return error_code::failed;
		stream->cursor(header.identsize, ang::streams::stream_reference::current);
		wsize bufferSize = wsize(stream->size() - sizeof(header) - header.identsize);
		array<byte> data(bufferSize);
		//byte* data = memory::default_allocator<byte>::alloc(bufferSize);
		stream->read(data.data(), bufferSize);

		width = header.width;
		height = header.height;
		format = DXGI_FORMAT_R8G8B8A8_UNORM;

		out_data = new collections::array_buffer<byte>(header.width * header.height * 4);
		switch (header.imagetype)
		{
		case IT_UNCOMPRESSED:
			LoadUncompressedTGAImage((byte*)out_data->buffer_ptr(), data.data(), &header);
			break;
		case IT_COMPRESSED:
			LoadCompressedTGAImage((byte*)out_data->buffer_ptr(), data.data(), &header);
			break;
		}
		//memory::default_allocator<byte>::free(data);
		return error_code::success;
	});

	return out_data;
}

bool d3d11_texture_loader::create_texture(d3d11_driver_t driver, d3d11::tex_file_info_t const* info, ang::ibuffer_t buffer, ID3D11Resource** resource, ID3D11ShaderResourceView** resourceView)
{
	if (info->resDim != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
		return false;

	HRESULT hr = S_OK;
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Height = info->height;
	desc.Width = info->width;
	desc.Format = (DXGI_FORMAT)info->format;
	desc.MipLevels = info->autoGenMips ? 0 : info->mipCount;
	desc.ArraySize = info->arraySize;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags =
		(info->autoGenMips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0)
		| (info->isCubeMap ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0);
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | (info->autoGenMips ? D3D11_BIND_RENDER_TARGET : 0);

	ID3D11Texture2D* texture = null;
	ang::array<ang::ibuffer_t> out_data;
	ang::array<D3D11_SUBRESOURCE_DATA> init_data;

	if (info->fileType == ' SDD')
	{
		wsize skipMip = 0;
		wsize twidth = 0;
		wsize theight = 0;
		wsize tdepth = 0;
		FillInitDataFromDDS(buffer, info, twidth, theight, tdepth, skipMip, init_data);
	}
	else
	{
		FillInitDataFromTGA(buffer, info, init_data, out_data);
		if (info->isCubeMap)
		{
			desc.Width = info->width / 4;
			desc.Height = info->height / 3;
		}
	}

	hr = driver->D3D11Device()->CreateTexture2D(&desc, info->autoGenMips ? NULL : &init_data[0], &texture);
	if (FAILED(hr))
		return false;

	if (resourceView)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvdesc;
		ZeroMemory(&srvdesc, sizeof(srvdesc));
		srvdesc.Format = desc.Format;

		if (info->isCubeMap)
		{
			srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvdesc.Texture2D.MipLevels = !desc.MipLevels ? -1 : desc.MipLevels;
			srvdesc.Texture2D.MostDetailedMip = 0;
		}
		else if (info->arraySize > 1)
		{
			srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			srvdesc.Texture2DArray.MipLevels = !desc.MipLevels ? -1 : desc.MipLevels;
			srvdesc.Texture2DArray.ArraySize = desc.ArraySize;
			srvdesc.Texture2DArray.FirstArraySlice = 0;
			srvdesc.Texture2DArray.MostDetailedMip = 0;
		}
		else
		{
			srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvdesc.Texture2D.MipLevels = !desc.MipLevels ? -1 : desc.MipLevels;
			srvdesc.Texture2D.MostDetailedMip = 0;
		}

		hr = driver->D3D11Device()->CreateShaderResourceView(texture, &srvdesc, resourceView);

		if (FAILED(hr))
		{
			texture->Release();
			return false;
		}


		if (info->autoGenMips)
		{
			(*resourceView)->GetDesc(&srvdesc);
			uint mipLevels;
			switch (srvdesc.ViewDimension)
			{
			case D3D_SRV_DIMENSION_TEXTURE2D:       mipLevels = srvdesc.Texture2D.MipLevels; break;
			case D3D_SRV_DIMENSION_TEXTURE2DARRAY:  mipLevels = srvdesc.Texture2DArray.MipLevels; break;
			case D3D_SRV_DIMENSION_TEXTURECUBE:     mipLevels = srvdesc.TextureCube.MipLevels; break;

			default:
				(*resourceView)->Release();
				*resourceView = nullptr;
				texture->Release();
				return false;
			}
			driver->execute_on_thread_safe([&]()
			{
				for (uint item = 0; item < info->arraySize; ++item)
				{
					uint res = D3D11CalcSubresource(0, item, mipLevels);
					driver->D3D11Context()->UpdateSubresource(texture, res, nullptr, init_data[item].pSysMem, init_data[item].SysMemPitch, 0);
				}
				driver->D3D11Context()->GenerateMips(*resourceView);
			});
		}
	}

	if (resource) {
		*resource = texture;
		texture->AddRef();
	}

	texture->Release();

	return true;
}

bool d3d11_texture_loader::create_array_texture(d3d11_driver_t driver, tex_file_info_t const* info, ang::collections::vector<ang::ibuffer_t>const& buffers, ID3D11Resource** resource, ID3D11ShaderResourceView** resourceView)
{
	if (info->resDim != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
		return false;

	HRESULT hr = S_OK;
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Height = info->height;
	desc.Width = info->width;
	desc.Format = (DXGI_FORMAT)info->format;
	desc.MipLevels = info->autoGenMips ? 0 : info->mipCount;
	desc.ArraySize = info->arraySize;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags =
		(info->autoGenMips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0)
		| (info->isCubeMap ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0);
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | (info->autoGenMips ? D3D11_BIND_RENDER_TARGET : 0);

	ID3D11Texture2D* texture = null;
	ang::array<ang::ibuffer_t> out_data;
	ang::array<D3D11_SUBRESOURCE_DATA> init_data;

	if (info->fileType == ' SDD')
	{ //TODO:
	  //wsize skipMip = 0;
	  //wsize twidth = 0;
	  //wsize theight = 0;
	  //wsize tdepth = 0;
	  //FillInitDataFromDDS(buffer, info, twidth, theight, tdepth, skipMip, init_data);
	}
	else
	{
		init_data.size(info->arraySize);
		for (auto i = 0U; i < info->arraySize; ++i)
		{
			init_data[i].pSysMem = buffers[i]->buffer_ptr();
			init_data[i].SysMemPitch = info->width * 4;
			init_data[i].SysMemSlicePitch = 0;
		}
	}

	hr = driver->D3D11Device()->CreateTexture2D(&desc, info->autoGenMips ? NULL : &init_data[0], &texture);
	if (FAILED(hr))
		return false;

	if (resourceView)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvdesc;
		ZeroMemory(&srvdesc, sizeof(srvdesc));
		srvdesc.Format = desc.Format;

		if (info->isCubeMap)
		{
			srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvdesc.Texture2D.MipLevels = !desc.MipLevels ? -1 : desc.MipLevels;
			srvdesc.Texture2D.MostDetailedMip = 0;
		}
		else if (info->arraySize > 1)
		{
			srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			srvdesc.Texture2DArray.MipLevels = !desc.MipLevels ? -1 : desc.MipLevels;
			srvdesc.Texture2DArray.ArraySize = desc.ArraySize;
			srvdesc.Texture2DArray.FirstArraySlice = 0;
			srvdesc.Texture2DArray.MostDetailedMip = 0;
		}
		else
		{
			srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvdesc.Texture2D.MipLevels = !desc.MipLevels ? -1 : desc.MipLevels;
			srvdesc.Texture2D.MostDetailedMip = 0;
		}

		hr = driver->D3D11Device()->CreateShaderResourceView(texture, &srvdesc, resourceView);

		if (FAILED(hr))
		{
			texture->Release();
			return false;
		}


		if (info->autoGenMips)
		{
			(*resourceView)->GetDesc(&srvdesc);
			uint mipLevels;
			switch (srvdesc.ViewDimension)
			{
			case D3D_SRV_DIMENSION_TEXTURE2D:       mipLevels = srvdesc.Texture2D.MipLevels; break;
			case D3D_SRV_DIMENSION_TEXTURE2DARRAY:  mipLevels = srvdesc.Texture2DArray.MipLevels; break;
			case D3D_SRV_DIMENSION_TEXTURECUBE:     mipLevels = srvdesc.TextureCube.MipLevels; break;

			default:
				(*resourceView)->Release();
				*resourceView = nullptr;
				texture->Release();
				return false;
			}
			driver->execute_on_thread_safe([&]()
			{
				for (uint item = 0; item < info->arraySize; ++item)
				{
					uint res = D3D11CalcSubresource(0, item, mipLevels);
					driver->D3D11Context()->UpdateSubresource(texture, res, nullptr, init_data[item].pSysMem, init_data[item].SysMemPitch, 0);
				}
				driver->D3D11Context()->GenerateMips(*resourceView);
			});
		}
	}

	if (resource) {
		*resource = texture;
		texture->AddRef();
	}

	texture->Release();

	return true;
}


bool d3d11_texture_loader::load_texture(d3d11_driver_t driver, ang::core::files::input_binary_file_t file, tex_file_info_t& info, ID3D11Resource** resource, ID3D11ShaderResourceView** resourceView, bool isCube)
{
	if (resource == null && resourceView == null)
		return false;

	ibuffer_t data;
	ZeroMemory(&info, sizeof(info));
	file->read(&info.fileType, sizeof(info.fileType));
	if (info.fileType == ' SDD')
	{
		file->cursor(0, streams::stream_reference::begin);
		data = d3d11_texture_loader::load_dds(file, info);
	}
	else
	{
		file->cursor(0, streams::stream_reference::begin);
		data = d3d11_texture_loader::load_tga(file, info.width, info.height, info.format);
		info.depth = 1;
		info.mipCount = 1;
		info.resDim = D3D11_RESOURCE_DIMENSION_TEXTURE2D;
		info.autoGenMips = resourceView ? true : false;
		if (isCube)
		{
			info.arraySize = 6;
			info.isCubeMap = true;
		}
		else //tex_type::tex2D
		{
			info.arraySize = 1;
			info.isCubeMap = false;
		}
	}

	if (data.is_empty())
		return false;

	if (resource)
		*resource = null;

	if (resourceView)
		*resourceView = null;

	return create_texture(driver, &info, data, resource, resourceView);
}

bool d3d11_texture_loader::load_texture(d3d11_driver_t driver,
	ang::collections::vector<ang::core::files::input_binary_file_t> const& files, tex_file_info_t& info,
	ID3D11Resource** resource, ID3D11ShaderResourceView** resourceView, bool isCube)
{
	if (resource == null && resourceView == null)
		return false;

	ang::collections::vector<ibuffer_t> data;

	ZeroMemory(&info, sizeof(info));

	for(ang::core::files::input_binary_file_t file : files)
	{
		file->read(&info.fileType, sizeof(info.fileType));
		if (info.fileType == ' SDD')
		{
			data += d3d11_texture_loader::load_dds(file, info);
		}
		else
		{
			data += d3d11_texture_loader::load_tga(file, info.width, info.height, info.format);
		}
	}

	if (data.is_empty() || (isCube && data.size() != 6))
		return false;

	if (info.fileType != ' SDD')
	{
		info.depth = 1;
		info.mipCount = 1;
		info.resDim = D3D11_RESOURCE_DIMENSION_TEXTURE2D;
		info.arraySize = data.size();
		info.isCubeMap = isCube;
		info.autoGenMips = resourceView ? true : false;
	}

	if (resource)
		*resource = null;

	if (resourceView)
		*resourceView = null;

	return create_array_texture(driver, &info, data, resource, resourceView);
}

#endif
