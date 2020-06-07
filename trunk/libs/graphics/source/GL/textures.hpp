#pragma once

#include <ang/graphics/angraph.hpp>

#if OPENGL_SUPPORT

namespace ang
{
	namespace graphics
	{
		namespace gl
		{
			class gl_texture;
			class gl_texture_loader;
		
			typedef object_wrapper<gl_texture> gl_texture_t;
			typedef object_wrapper<gl_texture_loader> gl_texture_loader_t;

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

			uint traslate_format_gl(ang::graphics::textures::tex_format_t format);
			textures::tex_format_t traslate_format_gl(uint);
			int pixel_size_by_format_gl(graphics::textures::tex_format format);

			class gl_texture
				: public object
				, public textures::itexture
			{
			private:
				string _texture_name;
				uint _tex_format;
				textures::tex_type_t _tex_type;
				graphics::size<float> _tex_dimentions;
				float _tex_dimentions_depth;
				uint _tex_buffer;

			public:
				gl_texture();

				ANG_DECLARE_INTERFACE();

			public: //overrides
				string resource_name()const override;
				textures::tex_type_t tex_type()const override;
				textures::tex_format_t tex_format()const override;
				graphics::size<float> tex_dimentions()const override;

			public: //internal
				bool load(gl_texture_loader_t, xml::ixml_node_t);
				bool use_texture(gl_driver_t, index idx)const;

				bool attach(uint tex_buffer);

				inline uint texture_object()const { return _tex_buffer; }
				inline uint format()const { return _tex_format; }

			private:
				virtual~gl_texture();
			};


			class gl_texture_loader
				: public object
				, public textures::itexture_loader
			{
			public:
				//static ibuffer_t load_dds(core::files::input_binary_file_t, tex_file_info_t& header);
				static ibuffer_t load_tga(core::files::input_binary_file_t, uint& width, uint& height, uint& format);
				static bool create_texture(gl_driver_t, tex_file_info_t const*, ibuffer_t, uint&);
				static bool create_array_texture(gl_driver_t, tex_file_info_t const*, collections::vector<ibuffer_t> const&, ID3D11Resource**, ID3D11ShaderResourceView**);
				static bool load_texture(gl_driver_t, core::files::input_binary_file_t, tex_file_info_t& info, ID3D11Resource**, ID3D11ShaderResourceView**, bool isCube = false);
				static bool load_texture(gl_driver_t, collections::vector<core::files::input_binary_file_t> const&, tex_file_info_t& info, ID3D11Resource**, ID3D11ShaderResourceView**, bool isCube = false);

			private:
				core::async::mutex_ptr_t main_mutex;
				gl_driver_t _driver;
				core::files::ifile_system_t _fs;
				core::async::idispatcher_t _work_thead;
				collections::unordered_map<string, wstring> _source_map;
				collections::unordered_map<string, xml::ixml_node_t> _texture_info_map;
				collections::unordered_map<string, weak_ptr<gl_texture>> _textures;
			public:
				gl_texture_loader(gl_driver_t parent);

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
				inline gl_driver* driver()const { return _driver.get(); }
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
				virtual~gl_texture_loader();
			};
		}
	}
}

inline uint ang::graphics::gl::traslate_format_gl(ang::graphics::textures::tex_format_t format)
{
	switch (format)
	{
	case textures::tex_format::R8G8B8: return GL_RGB;
	case textures::tex_format::R8G8B8A8: return GL_RGBA;
	case textures::tex_format::R5G6B5: return GL_RGB565;
	case textures::tex_format::R5G5B5A1: return GL_RGB5_A1;
	case textures::tex_format::R4G4B4A4: return GL_RGBA4;
	case textures::tex_format::D16: return GL_DEPTH_COMPONENT16;
	}
	return DXGI_FORMAT_UNKNOWN;
}

inline ang::graphics::textures::tex_format_t ang::graphics::gl::traslate_format_gl(uint format)
{
	switch (format)
	{
	case GL_RGB: return textures::tex_format::R8G8B8A8;
	case GL_RGBA: return textures::tex_format::D24S8;
	case GL_RGB565: return textures::tex_format::R5G6B5;
	case GL_RGB5_A1: return textures::tex_format::R5G5B5A1;
	case GL_RGBA4: return textures::tex_format::R4G4B4A4;
	case GL_DEPTH_COMPONENT16: return textures::tex_format::D16;
	}
	return textures::tex_format::null;
}

inline int ang::graphics::gl::pixel_size_by_format_gl(ang::graphics::textures::tex_format format)
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

#endif
