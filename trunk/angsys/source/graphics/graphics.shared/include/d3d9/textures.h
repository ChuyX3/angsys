#pragma once

#include <ang/graphics/graphics.h>

#if DIRECTX9_SUPPORT

namespace ang
{
	namespace graphics
	{
		namespace d3d9
		{
			class d3d9_texture;
			class d3d9_texture_loader;
		
			typedef object_wrapper<d3d9_texture> d3d9_texture_t;
			typedef object_wrapper<d3d9_texture_loader> d3d9_texture_loader_t;

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

			class d3d9_texture
				: public object
				, public textures::itexture
			{
			private:
				string _texture_name;
				DXGI_FORMAT _tex_format;
				textures::tex_type_t _tex_type;
				graphics::size<float> _tex_dimentions;
				float _tex_dimentions_depth;
				com_wrapper<ID3D11Resource> d3d_raw_resource;
				com_wrapper<ID3D11ShaderResourceView> d3d_shader_view;

			public:
				d3d9_texture();

				ANG_DECLARE_INTERFACE();


				void abc()
				{
					//has_runtime_type_info<d3d9_texture>::value
				}
			public: //overrides
				string resource_name()const override;
				textures::tex_type_t tex_type()const override;
				textures::tex_format_t tex_format()const override;
				graphics::size<float> tex_dimentions()const override;

			public: //internal
				bool load(d3d9_texture_loader_t, xml::ixml_node_t);
				bool use_texture(d3d9_driver_t, index idx)const;

				bool attach(com_wrapper<ID3D11ShaderResourceView> _view);

				inline ID3D11Texture1D* D3D11Texture1D()const { return static_cast<ID3D11Texture1D*>(d3d_raw_resource.get()); }
				inline ID3D11Texture2D* D3D11Texture2D()const { return static_cast<ID3D11Texture2D*>(d3d_raw_resource.get()); }
				inline ID3D11Texture3D* D3D11Texture3D()const { return static_cast<ID3D11Texture3D*>(d3d_raw_resource.get()); }
				inline ID3D11ShaderResourceView* D3D11ShaderResourceView()const { return d3d_shader_view.get(); }
				inline DXGI_FORMAT D3D11Format()const { return _tex_format; }

			private:
				virtual~d3d9_texture();
			};


			class d3d9_texture_loader
				: public object
				, public textures::itexture_loader
			{
			public:
				static ibuffer_t load_dds(core::files::input_binary_file_t, tex_file_info_t& header);
				static ibuffer_t load_tga(core::files::input_binary_file_t, uint& width, uint& height, uint& format);
				static bool create_texture(d3d9_driver_t, tex_file_info_t const*, ibuffer_t, ID3D11Resource**, ID3D11ShaderResourceView**);
				static bool create_array_texture(d3d9_driver_t, tex_file_info_t const*, collections::vector<ibuffer_t> const&, ID3D11Resource**, ID3D11ShaderResourceView**);
				static bool load_texture(d3d9_driver_t, core::files::input_binary_file_t, tex_file_info_t& info, ID3D11Resource**, ID3D11ShaderResourceView**, bool isCube = false);
				static bool load_texture(d3d9_driver_t, collections::vector<core::files::input_binary_file_t> const&, tex_file_info_t& info, ID3D11Resource**, ID3D11ShaderResourceView**, bool isCube = false);

			private:
				core::async::mutex_ptr_t main_mutex;
				d3d9_driver_t _driver;
				core::files::ifile_system_t _fs;
				core::async::idispatcher_t _work_thead;
				collections::unordered_map<string, wstring> _source_map;
				collections::unordered_map<string, xml::ixml_node_t> _texture_info_map;
				collections::unordered_map<string, weak_ptr<d3d9_texture>> _textures;
			public:
				d3d9_texture_loader(d3d9_driver_t parent);

				ANG_DECLARE_INTERFACE();

			public: //overrides
				bool load_sources(xml::ixml_node_t) override;
				void set_file_system(core::files::ifile_system_t) override;
				bool load_library(xml::ixml_node_t) override;
				core::async::iasync_t<textures::itexture_loader_t> load_library_async(xml::ixml_node_t) override;
				textures::itexture_t load_texture(string) override;
				textures::itexture_t load_texture(xml::ixml_node_t) override;
				core::async::iasync_t<textures::itexture_t> load_texture_async(string) override;
				core::async::iasync_t<textures::itexture_t> load_texture_async(xml::ixml_node_t) override;
				textures::itexture_t find_texture(cstr_t)const override;
				textures::itexture_t find_texture(cwstr_t)const override;

			public: //internal
				inline core::files::ifile_system* get_file_system()const {
					return _fs.is_empty() ? core::files::ifile_system::get_file_system() : _fs.get();
				}
				inline d3d9_driver* driver()const { return _driver.get(); }
				inline wstring find_file(cstr_t sid)const {
					core::async::scope_locker<core::async::mutex_ptr_t> lock = main_mutex;
					if (_source_map.is_empty())
						return "";
					auto it = _source_map->find(sid);
					return it.is_valid() ? it->value : L"";
				}
				inline wstring find_file(cwstr_t sid)const {
					core::async::scope_locker<core::async::mutex_ptr_t> lock = main_mutex;
					if (_source_map.is_empty())
						return "";
					auto it = _source_map->find(sid);
					return it.is_valid() ? it->value : L"";
				}

			private:
				virtual~d3d9_texture_loader();
			};
		}
	}
}

inline DXGI_FORMAT ang::graphics::d3d9::traslate_format_dx(ang::graphics::textures::tex_format_t format)
{
	switch (format)
	{
	case textures::tex_format::R8G8B8A8: return DXGI_FORMAT_R8G8B8A8_UNORM;
	case textures::tex_format::D24S8: return DXGI_FORMAT_D24_UNORM_S8_UINT;
	case textures::tex_format::D16: return DXGI_FORMAT_D16_UNORM;
	}
	return DXGI_FORMAT_UNKNOWN;
}

inline ang::graphics::textures::tex_format_t ang::graphics::d3d9::traslate_format_dx(DXGI_FORMAT format)
{
	switch (format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM: return textures::tex_format::R8G8B8A8;
	case DXGI_FORMAT_D24_UNORM_S8_UINT: return textures::tex_format::D24S8;
	case DXGI_FORMAT_D16_UNORM: return textures::tex_format::D16;
	}
	return textures::tex_format::null;
}

inline int ang::graphics::d3d9::pixel_size_by_format_dx(ang::graphics::textures::tex_format format)
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

inline D3D11_SRV_DIMENSION ang::graphics::d3d9::traslate_type_dx1(ang::graphics::textures::tex_type_t type)
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

inline ang::graphics::textures::tex_type_t ang::graphics::d3d9::traslate_type_dx1(D3D11_SRV_DIMENSION type)
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

inline D3D11_RESOURCE_DIMENSION ang::graphics::d3d9::traslate_type_dx2(ang::graphics::textures::tex_type_t type)
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

inline ang::graphics::textures::tex_type_t ang::graphics::d3d9::traslate_type_dx2(D3D11_RESOURCE_DIMENSION type)
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
