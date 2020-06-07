#pragma once

#if DIRECTX11_SUPPORT

namespace ang
{
	namespace graphics
	{
		namespace d3d11
		{
			class d3d11_texture;
		
			typedef object_wrapper<d3d11_texture> d3d11_texture_t;

			struct tex_file_info_t
			{
				uint width;
				uint height;
				uint depth;
				uint resDim;
				uint arraySize;
				uint mipCount;
				uint format;
				uint fileType;
				bool isCubeMap;
				bool autoGenMips;
			};

			inline DXGI_FORMAT traslate_format_dx(textures::tex_format_t format);
			inline textures::tex_format_t traslate_format_dx(DXGI_FORMAT format);
			inline int pixel_size_by_format_dx(textures::tex_format format);
			inline D3D11_SRV_DIMENSION traslate_type_dx1(textures::tex_type_t type);
			inline textures::tex_type_t traslate_type_dx1(D3D11_SRV_DIMENSION type);
			inline D3D11_RESOURCE_DIMENSION traslate_type_dx2(textures::tex_type_t type);
			inline textures::tex_type_t traslate_type_dx2(D3D11_RESOURCE_DIMENSION type);

			class d3d11_texture
				: public graphic<d3d11_texture
				, iid("ang::graphics::d3d11::d3d11_texture")
				, textures::itexture
				, buffers::igpu_buffer
				, resources::iresource>
			{
			private:
				string m_resource_sid;
				DXGI_FORMAT m_tex_format;
				buffers::buffer_usage_t m_usage;
				buffers::buffer_bind_flag_t m_bind_flags;
				textures::tex_type_t m_tex_type;
				size3d<uint> m_tex_dimentions;
				//com_wrapper<ID3D11SamplerState> m_d3d_sampler;
				com_wrapper<ID3D11Resource> m_d3d_raw_resource;
				com_wrapper<ID3D11ShaderResourceView> m_d3d_shader_view;

			public:
				d3d11_texture();

			protected: //override
				void dispose()override;

			public: //overrides
				resources::iresource_t resource()const override;
				textures::tex_type_t tex_type()const override;
				textures::tex_format_t tex_format()const override;
				size3d<uint> tex_dimentions()const override;
				buffers::buffer_type_t buffer_type()const override;
				buffers::buffer_usage_t buffer_usage()const override;
				buffers::buffer_bind_flag_t buffer_bind_flag()const override;
				ibuffer_t map(idriver_t, bool writeMode) override;
				bool unmap(idriver_t, ibuffer_t) override;

				resources::resource_type_t resource_type()const override;
				string resource_sid()const override;
				void resource_sid(cstr_t) override;
				intfptr fast_cast(resources::resource_type_t) override;

			public: //internal
				bool create(d3d11_driver_t driver, textures::tex_type_t type, textures::tex_format_t color_format, buffers::buffer_usage_t usage, buffers::buffer_bind_flag_t flags, size3d<uint> dimentions, string sid = null);
				bool load(d3d11_driver_t, string filename, textures::tex_type_t type, string sid = null);
				//bool load(d3d11_driver_t, array<string> files, textures::tex_type_t type);
				bool use_texture(d3d11_driver_t, windex idx)const;
				bool attach(com_wrapper<ID3D11Resource> resource, d3d11_driver_t driver = null, string sid = null);
				bool attach(com_wrapper<ID3D11ShaderResourceView> _view, string sid = null);
				void clear();

				inline ID3D11Resource* D3D11Resource()const { return m_d3d_raw_resource.get(); }
				inline ID3D11Texture1D* D3D11Texture1D()const { return static_cast<ID3D11Texture1D*>(m_d3d_raw_resource.get()); }
				inline ID3D11Texture2D* D3D11Texture2D()const { return static_cast<ID3D11Texture2D*>(m_d3d_raw_resource.get()); }
				inline ID3D11Texture3D* D3D11Texture3D()const { return static_cast<ID3D11Texture3D*>(m_d3d_raw_resource.get()); }
				inline ID3D11ShaderResourceView* D3D11ShaderResourceView()const { return m_d3d_shader_view.get(); }
				inline DXGI_FORMAT D3D11Format()const { return m_tex_format; }
			};


			struct d3d11_texture_loader
			{
			public:
				static ibuffer_t load_dds(core::files::input_binary_file_t, tex_file_info_t& header);
				static ibuffer_t load_tga(core::files::input_binary_file_t, uint& width, uint& height, uint& format);
				static bool create_texture(d3d11_driver_t, tex_file_info_t const*, ibuffer_t, ID3D11Resource**, ID3D11ShaderResourceView**);
				static bool create_array_texture(d3d11_driver_t, tex_file_info_t const*, collections::vector<ibuffer_t> const&, ID3D11Resource**, ID3D11ShaderResourceView**);
				static bool load_texture(d3d11_driver_t, core::files::input_binary_file_t, tex_file_info_t& info, ID3D11Resource**, ID3D11ShaderResourceView**, bool isCube = false);
				static bool load_texture(d3d11_driver_t, collections::vector<core::files::input_binary_file_t> const&, tex_file_info_t& info, ID3D11Resource**, ID3D11ShaderResourceView**, bool isCube = false);

			};
		}
	}
}

inline DXGI_FORMAT ang::graphics::d3d11::traslate_format_dx(ang::graphics::textures::tex_format_t format)
{
	switch (format)
	{
	case textures::tex_format::R8G8B8A8: return DXGI_FORMAT_R8G8B8A8_UNORM;
	case textures::tex_format::D24S8: return DXGI_FORMAT_D24_UNORM_S8_UINT;
	case textures::tex_format::D16: return DXGI_FORMAT_D16_UNORM;
	}
	return DXGI_FORMAT_UNKNOWN;
}

inline ang::graphics::textures::tex_format_t ang::graphics::d3d11::traslate_format_dx(DXGI_FORMAT format)
{
	switch (format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM: return textures::tex_format::R8G8B8A8;
	case DXGI_FORMAT_D24_UNORM_S8_UINT: return textures::tex_format::D24S8;
	case DXGI_FORMAT_D16_UNORM: return textures::tex_format::D16;
	}
	return textures::tex_format::null;
}

inline int ang::graphics::d3d11::pixel_size_by_format_dx(ang::graphics::textures::tex_format format)
{
	switch (format)
	{
	case textures::tex_format::R8G8B8A8: return  32;
	case textures::tex_format::D24S8: return 32;
	case textures::tex_format::D16: return 16;
	case textures::tex_format::R32G32B32: return  32 * 3;
	case textures::tex_format::R32G32B32A32: return  32 * 4;
	case textures::tex_format::R10G310B10A2: return  32;
	case textures::tex_format::A8: return  8;
	case textures::tex_format::R8G8B8: return  24;
	case textures::tex_format::R5G6B5: return  16;
	case textures::tex_format::R5G5B5A1: return  16;
	case textures::tex_format::R4G4B4A4: return  16;
	}
	return 0;
}

inline D3D11_SRV_DIMENSION ang::graphics::d3d11::traslate_type_dx1(ang::graphics::textures::tex_type_t type)
{
	switch (type)
	{
	case textures::tex_type::tex1D:
		return D3D11_SRV_DIMENSION_TEXTURE1D;
		break;
	case textures::tex_type::tex2D:
		return D3D11_SRV_DIMENSION_TEXTURE2D;
		break;
	case textures::tex_type::tex3D:
		return D3D11_SRV_DIMENSION_TEXTURE3D;
		break;
	}
	return D3D11_SRV_DIMENSION_UNKNOWN;
}

inline ang::graphics::textures::tex_type_t ang::graphics::d3d11::traslate_type_dx1(D3D11_SRV_DIMENSION type)
{
	switch (type)
	{
	case D3D11_SRV_DIMENSION_TEXTURE1D:
		return textures::tex_type::tex1D;
		break;
	case D3D11_SRV_DIMENSION_TEXTURE2D:
		return textures::tex_type::tex2D;
		break;
	case D3D11_SRV_DIMENSION_TEXTURE3D:
		return textures::tex_type::tex3D;
		break;
	case D3D11_SRV_DIMENSION_TEXTURECUBE:
		return textures::tex_type::texCube;
		break;
	}
	return textures::tex_type::null;
}

inline D3D11_RESOURCE_DIMENSION ang::graphics::d3d11::traslate_type_dx2(ang::graphics::textures::tex_type_t type)
{
	switch (type)
	{
	case textures::tex_type::tex1D:
		return D3D11_RESOURCE_DIMENSION_TEXTURE1D;
		break;
	case textures::tex_type::tex2D:
		return D3D11_RESOURCE_DIMENSION_TEXTURE2D;
		break;
	case textures::tex_type::tex3D:
		return D3D11_RESOURCE_DIMENSION_TEXTURE3D;
		break;
	}
	return D3D11_RESOURCE_DIMENSION_UNKNOWN;
}

inline ang::graphics::textures::tex_type_t ang::graphics::d3d11::traslate_type_dx2(D3D11_RESOURCE_DIMENSION type)
{
	switch (type)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		return textures::tex_type::tex1D;
		break;
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		return textures::tex_type::tex2D;
		break;
	case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		return textures::tex_type::tex3D;
		break;
	}
	return textures::tex_type::null;
}

#endif
