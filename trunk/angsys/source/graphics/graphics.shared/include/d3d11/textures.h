#pragma once

#if DIRECTX11_SUPPORT

namespace ang
{
	namespace graphics
	{
		namespace d3d11
		{
			class d3d11_texture;
			class d3d11_texture_loader;
		
			typedef object_wrapper<d3d11_texture> d3d11_texture_t;
			typedef object_wrapper<d3d11_texture_loader> d3d11_texture_loader_t;

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
				: public object
				, public textures::itexture
			{
			private:
				string m_texture_name;
				DXGI_FORMAT m_tex_format;
				textures::tex_type_t m_tex_type;
				graphics::size<float> m_tex_dimentions;
				float m_tex_dimentions_depth;
				com_wrapper<ID3D11Resource> m_d3d_raw_resource;
				com_wrapper<ID3D11ShaderResourceView> m_d3d_shader_view;

			public:
				d3d11_texture();

				ANG_DECLARE_INTERFACE();


				void abc()
				{
					//has_runtime_type_info<d3d11_texture>::value
				}
			public: //overrides
				text::istring_view_t resource_name()const override;
				textures::tex_type_t tex_type()const override;
				textures::tex_format_t tex_format()const override;
				graphics::size<float> tex_dimentions()const override;

			public: //internal
				bool load(d3d11_texture_loader_t, dom::xml::xml_node_t);
				bool use_texture(d3d11_driver_t, index idx)const;

				bool attach(com_wrapper<ID3D11ShaderResourceView> _view);

				inline ID3D11Texture1D* D3D11Texture1D()const { return static_cast<ID3D11Texture1D*>(m_d3d_raw_resource.get()); }
				inline ID3D11Texture2D* D3D11Texture2D()const { return static_cast<ID3D11Texture2D*>(m_d3d_raw_resource.get()); }
				inline ID3D11Texture3D* D3D11Texture3D()const { return static_cast<ID3D11Texture3D*>(m_d3d_raw_resource.get()); }
				inline ID3D11ShaderResourceView* D3D11ShaderResourceView()const { return m_d3d_shader_view.get(); }
				inline DXGI_FORMAT D3D11Format()const { return m_tex_format; }

			private:
				virtual~d3d11_texture();
			};


			class d3d11_texture_loader
				: public object
				, public textures::itexture_loader
			{
			public:
				static ibuffer_t load_dds(core::files::input_binary_file_t, tex_file_info_t& header);
				static ibuffer_t load_tga(core::files::input_binary_file_t, uint& width, uint& height, uint& format);
				static bool create_texture(d3d11_driver_t, tex_file_info_t const*, ibuffer_t, ID3D11Resource**, ID3D11ShaderResourceView**);
				static bool create_array_texture(d3d11_driver_t, tex_file_info_t const*, collections::vector<ibuffer_t> const&, ID3D11Resource**, ID3D11ShaderResourceView**);
				static bool load_texture(d3d11_driver_t, core::files::input_binary_file_t, tex_file_info_t& info, ID3D11Resource**, ID3D11ShaderResourceView**, bool isCube = false);
				static bool load_texture(d3d11_driver_t, collections::vector<core::files::input_binary_file_t> const&, tex_file_info_t& info, ID3D11Resource**, ID3D11ShaderResourceView**, bool isCube = false);

			private:
				core::async::mutex_ptr_t m_mutex;
				d3d11_driver_t m_driver;
				core::files::ifile_system_t m_fs;
				core::async::thread_t m_work_thead;
				collections::hash_map<string, wstring> m_source_map;
				collections::hash_map<string, dom::xml::xml_node_t> m_texture_info_map;
				collections::hash_map<string, weak_ptr<d3d11_texture>> m_textures;

			public:
				d3d11_texture_loader(d3d11_driver_t parent);

				ANG_DECLARE_INTERFACE();

			public: //overrides
				bool load_sources(dom::xml::xml_node_t) override;
				void set_file_system(core::files::ifile_system_t) override;
				bool load_library(dom::xml::xml_node_t) override;
				core::async::iasync<textures::itexture_loader_t> load_library_async(dom::xml::xml_node_t) override;
				textures::itexture_t load_texture(text::raw_cstr_t) override;
				textures::itexture_t load_texture(dom::xml::xml_node_t) override;
				core::async::iasync<textures::itexture_t> load_texture_async(text::raw_cstr_t) override;
				core::async::iasync<textures::itexture_t> load_texture_async(dom::xml::xml_node_t) override;
				textures::itexture_t find_texture(text::raw_cstr_t)const override;

			public: //internal
				inline core::files::ifile_system* get_file_system()const {
					return m_fs.is_empty() ? core::files::ifile_system::fs_instance() : m_fs.get();
				}
				inline d3d11_driver* driver()const { return m_driver.get(); }
				inline wstring find_file(text::raw_cstr_t sid)const {
					core::async::scope_locker<core::async::mutex_ptr_t> lock = m_mutex;
					if (m_source_map.is_empty())
						return "";
					auto it = m_source_map->find(sid);
					return it.is_valid() ? it->value : wstring();
				}

			private:
				virtual~d3d11_texture_loader();
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
