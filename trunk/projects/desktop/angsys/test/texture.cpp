#include "pch.h"
#include "test.h"

using namespace ang;
using namespace ang::graphics;

static optional<array<byte>> load_tga(string name, uint& width, uint& height, uint& bpp);

const int REPEAT = 1, CLAMP_TO_EDGE = 2, MIRRORED_REPEAT = 3; // char is a byte

gl_texture::gl_texture()
	: m_tex_buffer_id(0)
{

}

error gl_texture::init(gl_context_t context, string path)
{
	m_context = context;
    int tiling = REPEAT;
    uint width, height, bpp;
    auto data = load_tga(path, width, height, bpp);

    if (data.failed())
        return data.error();

    core::async::scope_locker<gl_context_t> lock = context;

    // create the OpenGL ES texture resource and get the handle
    glGenTextures(1, &m_tex_buffer_id);
    // bind the texture to the 2D texture type
    glBindTexture(GL_TEXTURE_2D, m_tex_buffer_id);

    // load the image data into OpenGL ES texture resource
    if (bpp == 24) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data->data()); //GL_RGB R=8,G=8,B=8 24 Bits Per Pixel
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data->data()); // GL_RGBA = 32its...A=8
    }

    //set the filters for minification and magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //NEAREST draw quads

    // generate the mipmap chain
    glGenerateMipmap(GL_TEXTURE_2D);

    //set the wrapping modes
    switch (tiling)
    {
    case 1://GL_REPEAT
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        break;
    case 2://CLAMP_TO_EDGE
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        break;
    case 3://GL_MIRRORED_REPEAT
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        break;
    }//printf("Texture has been loaded successfully:\n");

    glBindTexture(GL_TEXTURE_2D, 0);

	return error_code::success;
}

core::async::iasync_op<gl_texture_t> gl_texture::init_async(gl_context_t context, string path)
{
	m_context = context;
	return context->create_task<optional<gl_texture_t>>([=]()->optional<gl_texture_t>
	{
        int tiling = REPEAT;
        uint width, height, bpp;
        auto data = load_tga(path, width, height, bpp);

        if (data.failed())
            return data.error();

        core::async::scope_locker<gl_context_t> lock = context;

        // create the OpenGL ES texture resource and get the handle
        glGenTextures(1, &m_tex_buffer_id);
        // bind the texture to the 2D texture type
        glBindTexture(GL_TEXTURE_2D, m_tex_buffer_id);

        // load the image data into OpenGL ES texture resource
        if (bpp == 24) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data->data()); //GL_RGB R=8,G=8,B=8 24 Bits Per Pixel
        }
        else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data->data()); // GL_RGBA = 32its...A=8
        }

        //set the filters for minification and magnification
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //NEAREST draw quads

        // generate the mipmap chain
        glGenerateMipmap(GL_TEXTURE_2D);

        //set the wrapping modes
        switch (tiling)
        {
        case 1://GL_REPEAT
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
        case 2://CLAMP_TO_EDGE
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;
        case 3://GL_MIRRORED_REPEAT
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            break;
        }//printf("Texture has been loaded successfully:\n");

        glBindTexture(GL_TEXTURE_2D, 0);

		return this;
	});
}

void gl_texture::dispose()
{
	if (m_tex_buffer_id != 0 && m_context.is_valid())
		m_context.lock()->create_task<void>([=]()->void
	{
		core::async::scope_locker<gl_context_t> lock = m_context.lock();
		glDeleteBuffers(1, &m_tex_buffer_id);
	})->result();
}

uint gl_texture::tex_buffer()const
{
	return m_tex_buffer_id;
}

///////////////////////////////////////////////////////////////////////////

#pragma pack(push,x1)					// Byte alignment (8-bit)
#pragma pack(1)

struct ANG_ALIGN(8) TGA_HEADER
{
    unsigned char  identsize;			// size of ID field that follows 18 byte header (0 usually)
    unsigned char  colourmaptype;		// type of colour map 0=none, 1=has palette
    unsigned char  imagetype;			// type of image 2=rgb uncompressed, 10 - rgb rle compressed

    short colourmapstart;				// first colour map entry in palette
    short colourmaplength;				// number of colours in palette
    unsigned char  colourmapbits;		// number of bits per palette entry 15,16,24,32

    short xstart;						// image x origin
    short ystart;						// image y origin
    short width;						// image width in pixels
    short height;						// image height in pixels
    unsigned char  bits;				// image bits per pixel 24,32
    unsigned char  descriptor;			// image descriptor bits (vh flip bits)

    // pixel data follows header
};

#pragma pack(pop,x1)

const int IT_COMPRESSED = 10;
const int IT_UNCOMPRESSED = 2;

static void LoadCompressedImage(array<byte>& dest, array<byte> const& src, TGA_HEADER& header)
{
    byte* pDest = &dest[0];
    byte const* pSrc = &src[0];

    int w = header.width;
    int h = header.height;
    int rowSize = w * 4;// header.bits / 8;
    bool bInverted = ((header.descriptor & (1 << 5)) != 0);
    byte* pDestPtr = bInverted ? pDest + (h + 1) * rowSize : pDest;
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
                if (header.bits != 24)
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
                if (header.bits != 24)
                    *pDestPtr++ = pSrc[3];
                else *pDestPtr++ = 1;//ADDING ALPHA CHANEL
                countPixels++;
            }
            pSrc += (header.bits >> 3);
        }
    }
}

static void LoadUncompressedImage(array<byte>& dest, array<byte> const& src, TGA_HEADER& header)
{
    byte* pDest = &dest[0];
    byte const* pSrc = &src[0];

    int w = header.width;
    int h = header.height;
    int rowSize = w * header.bits / 8;
    bool bInverted = ((header.descriptor & (1 << 5)) != 0);
    for (int i = 0; i < h; i++)
    {
        byte const* pSrcRow = pSrc +
            (bInverted ? (h - i - 1) * rowSize : i * rowSize);
        if (header.bits == 24)
        {
            for (int j = 0; j < w; j++)
            {
                *pDest++ = pSrcRow[2];
                *pDest++ = pSrcRow[1];
                *pDest++ = pSrcRow[0];
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

optional<array<byte>> load_tga(string path, uint& width, uint& height, uint& bpp)
{
    core::files::input_binary_file_t file;
    if (!core::files::ifile_system::instance()->open(path, &file, "$(Data)"_r))
        return error_code::file_not_found;
    error er;
    array<byte> data;
    TGA_HEADER header;
    memset(&header, 0, size_of(header));
    if ((er = file->read([&](streams::ibinary_input_stream_t stream)->error
    {  
        stream >> header;
        wsize size = (wsize)stream->size();
        stream->cursor(sizeof(header) + header.identsize, streams::stream_reference::begin);

        if (header.imagetype != IT_COMPRESSED && header.imagetype != IT_UNCOMPRESSED)     
            return error_code::failed;

        if (header.bits != 24 && header.bits != 32)
            return error_code::failed;
        
        data.size(size - size_of(header) - header.identsize);
        stream->read(data);

        return error_code::success;

    })).code() != error_code::success)
        return er;

    width = header.width;
    height = header.height;
    bpp = header.bits;

    array<byte> out_data(header.width * header.height * header.bits / 8);

    switch (header.imagetype)
    {
    case IT_UNCOMPRESSED:
        LoadUncompressedImage(out_data, data, header);
        break;
    case IT_COMPRESSED:
        LoadCompressedImage(out_data, data, header);
        break;
    }
    return move(out_data);
}
