#pragma once

#if OPENGL_FAMILY_SUPPORT

struct GLTexformat
{
	GLuint format;
	GLuint type;
};

namespace coffe
{
	namespace graphics
	{
		namespace gl
		{
			coffe_object(gl_texture);

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

			inline GLTexformat traslate_format_gl(textures::tex_format_t format);
			inline textures::tex_format_t traslate_format_gl(GLTexformat format);
			inline int pixel_size_by_format_gl(textures::tex_format format);

			inline GLuint traslate_type_gl(textures::tex_type_t type);
			inline textures::tex_type_t traslate_type_gl(GLuint type);


			class gl_texture
				: public smart<gl_texture
				, system_object
				, textures::itexture
				, resources::iresource>
			{
			private:
				string m_resource_sid;
				textures::tex_format_t m_tex_format;
				buffers::buffer_usage_t m_usage;
				buffers::buffer_bind_flag_t m_bind_flags;
				textures::tex_type_t m_tex_type;
				size3d<uint> m_tex_dimentions;
				GLuint m_gl_resource;

			public:
				gl_texture();

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
				bool create(gl_driver_t driver, textures::tex_type_t type, textures::tex_format_t color_format, buffers::buffer_usage_t usage, buffers::buffer_bind_flag_t flags, size3d<uint> dimentions, string sid = null);
				bool load(gl_driver_t, string filename, textures::tex_type_t type, string sid = null);
				//bool load(gl_driver_t, array<string> files, textures::tex_type_t type);
				bool use_texture(gl_driver_t, windex idx)const;
				bool attach(GLuint resource, gl_driver_t driver = null, string sid = null);
				void clear();

				inline GLuint GLResource()const { return m_gl_resource; }
				inline GLTexformat GLFormat()const { return traslate_format_gl(m_tex_format); }
			};


			struct gl_texture_loader
			{
			public:
				static ibuffer_t load_dds(core::files::input_binary_file_t, tex_file_info_t& header);
				static ibuffer_t load_tga(core::files::input_binary_file_t, uint& width, uint& height, uint& format);
				static bool create_texture(gl_driver_t, tex_file_info_t const*, ibuffer_t, GLuint*);
				static bool create_array_texture(gl_driver_t, tex_file_info_t const*, collections::vector<ibuffer_t> const&, GLuint*);
				static bool load_texture(gl_driver_t, core::files::input_binary_file_t, tex_file_info_t& info, GLuint*, bool isCube = false);
				static bool load_texture(gl_driver_t, collections::vector<core::files::input_binary_file_t> const&, tex_file_info_t& info, GLuint*, bool isCube = false);

			};
		}
	}
}

inline GLTexformat coffe::graphics::gl::traslate_format_gl(coffe::graphics::textures::tex_format_t format)
{
	switch (format)
	{
	case textures::tex_format::R32G32B32: return { GL_RGB, GL_UNSIGNED_INT };
	case textures::tex_format::R32G32B32A32: return { GL_RGBA, GL_UNSIGNED_INT };
	case textures::tex_format::R10G310B10A2: return { GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV };
	case textures::tex_format::R8G8B8: return { GL_RGB, GL_UNSIGNED_BYTE };
	case textures::tex_format::R8G8B8A8: return { GL_RGBA, GL_UNSIGNED_BYTE };
	case textures::tex_format::D24S8:return { GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8 };
	case textures::tex_format::D16: return { GL_DEPTH_COMPONENT , GL_UNSIGNED_SHORT };
	}
	return { 0,0 };
}

inline coffe::graphics::textures::tex_format_t coffe::graphics::gl::traslate_format_gl(GLTexformat format)
{
	switch (format.format)
	{
	case GL_RGB:
		return format.type == GL_UNSIGNED_INT ? textures::tex_format::R32G32B32 : textures::tex_format::R8G8B8;
	case GL_RGBA:
		return format.type == GL_UNSIGNED_INT ? textures::tex_format::R32G32B32A32 : textures::tex_format::R8G8B8A8;
	case GL_DEPTH_STENCIL:
		return textures::tex_format::D24S8;
	case GL_DEPTH_COMPONENT:
		return textures::tex_format::D16;
	}
	return textures::tex_format::null;
}

inline int coffe::graphics::gl::pixel_size_by_format_gl(coffe::graphics::textures::tex_format format)
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

inline GLuint coffe::graphics::gl::traslate_type_gl(coffe::graphics::textures::tex_type_t type)
{
#ifndef GL_TEXTURE_1D
#define GL_TEXTURE_1D 0x0DE0
#endif

	switch (type)
	{
	case textures::tex_type::tex1D:
		return GL_TEXTURE_1D;
		break;
	case textures::tex_type::tex2D:
		return GL_TEXTURE_2D;
		break;
	case textures::tex_type::tex3D:
		return GL_TEXTURE_3D;
	case textures::tex_type::texCube:
		return GL_TEXTURE_CUBE_MAP;
	}
	return 0;
}

inline coffe::graphics::textures::tex_type_t coffe::graphics::gl::traslate_type_gl(GLuint type)
{
	switch (type)
	{
	case GL_TEXTURE_1D:
		return textures::tex_type::tex1D;
	case GL_TEXTURE_2D:
		return textures::tex_type::tex2D;
	case GL_TEXTURE_3D:
		return textures::tex_type::tex3D;
	case GL_TEXTURE_CUBE_MAP:
		return textures::tex_type::texCube;
	}
	return textures::tex_type::null;
}

#endif
