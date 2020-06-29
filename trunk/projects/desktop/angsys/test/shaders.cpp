#include "pch.h"
#include "test.h"

using namespace ang;
using namespace ang::graphics;

static optional<uint> load_shader(GLenum type, castr_t code)
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader(type);

	if (shader == 0)
		return 0;

	glShaderSource(shader, 1, (const char**)&code, NULL);

	// Compile the shader
	glCompileShader(shader);

	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			array<char> infoLog(infoLen + 1);
			glGetShaderInfoLog(shader, infoLen, NULL, &infoLog[0]);
			glDeleteShader(shader);
			return error(castr_t(&infoLog[0], infoLog.size() - 1));
		}
		else
		{
			glDeleteShader(shader);
			return error_code::unknown;
		}
	}
	return shader;
}

static optional<uint> load_program(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint programObject;
	GLint linked;

	// Create the program object
	programObject = glCreateProgram();

	if (programObject == 0)
		return error_code::unknown;

	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);

	// Link the program
	glLinkProgram(programObject);

	// Check the link status
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		GLint infoLen = 0;

		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			array<char> infoLog(infoLen + 1);
			glGetProgramInfoLog(programObject, infoLen, NULL, &infoLog[0]);
			glDeleteProgram(programObject);
			return error(castr_t(&infoLog[0], infoLog.size() - 1));
		}
		else
		{
			return error_code::unknown;
		}
	}
	return programObject;
}

gl_shaders::gl_shaders()
	: m_program(0)
	, m_vertex_shader(0)
	, m_fragment_shader(0)
{

}

void gl_shaders::dispose()
{
	if (m_program !=0 && m_context.is_valid())
		m_context.lock()->create_task<void>([=]()->void
	{
		core::async::scope_locker<gl_context_t> lock = m_context.lock();
		glDeleteProgram(m_program);
		glDeleteShader(m_vertex_shader);
		glDeleteShader(m_fragment_shader);
	})->result();
}

error gl_shaders::init(gl_context_t context, string vsfile, string psfile)
{
	m_context = context;

	core::files::input_text_file_t file;
	if (!core::files::ifile_system::instance()->open(vsfile, &file, "$(Data)"_r))
		return error_code::file_not_found;
	auto sz = file->size();
	string vssource = "";
	file->read(vssource, sz);
	file = null;

	if (!core::files::ifile_system::instance()->open(psfile, &file, "$(Data)"_r))
		return error_code::file_not_found;
	sz = file->size();
	string fssource = "";
	file->read(fssource, sz);
	file = null;

	core::async::scope_locker<gl_context_t> lock = context;

	auto vs = load_shader(GL_VERTEX_SHADER, vssource->cstr().cstr<text::encoding::ascii>());
	if (vs.failed()) {
		return vs.error();
	}

	auto fs = load_shader(GL_FRAGMENT_SHADER, fssource->cstr().cstr<text::encoding::ascii>());
	if (fs.failed()) {
		glDeleteShader(vs.get());
		return fs.error();
	}

	auto prog = load_program(vs.get(), fs.get());
	if (prog.failed()) {
		glDeleteShader(vs.get());
		glDeleteShader(fs.get());
		return prog.error();
	}

	m_program = prog.get();
	m_vertex_shader = vs.get();
	m_fragment_shader = fs.get();

	windex beg = invalid_index, end = invalid_index;
	windex idx = vssource->find("attribute"_r);
	windex tex = 0;
	string name = null;
	while (idx != invalid_index) {
		end = vssource->find(";"_r, idx);
		beg = vssource->find_reverse(" "_r, end, idx);
		if (beg != invalid_index) {
			name = vssource->sub_string(beg + 1, end);
			m_attributes[name] = glGetAttribLocation(m_program, name->cstr().cstr<text::encoding::ascii>().cstr());
		}
		idx = vssource->find("attribute"_r, end);
	}

	idx = vssource->find("uniform"_r);
	name = null;
	while (idx != invalid_index) {
		end = vssource->find(";"_r, idx);
		tex = vssource->find("sampler2D"_r, idx, end);
		if (tex != invalid_index) {
			beg = vssource->find_reverse(" "_r, end, idx);
			if (beg != invalid_index) {
				name = vssource->sub_string(beg + 1, end);
				m_samplers[name] = glGetUniformLocation(m_program, name->cstr().cstr<text::encoding::ascii>().cstr());
			}
		}
		else
		{
			tex = vssource->find("samplerCube"_r, idx, end);
			if (tex != invalid_index) {
				beg = vssource->find_reverse(" "_r, end, idx);
				if (beg != invalid_index) {
					name = vssource->sub_string(beg + 1, end);
					m_samplers[name] = glGetUniformLocation(m_program, name->cstr().cstr<text::encoding::ascii>().cstr());
				}
			}
			else
			{
				beg = vssource->find_reverse(" "_r, end, idx);
				if (beg != invalid_index) {
					name = vssource->sub_string(beg + 1, end);
					m_uniforms[name] = glGetUniformLocation(m_program, name->cstr().cstr<text::encoding::ascii>().cstr());
				}
			}
		}
		idx = vssource->find("uniform"_r, end);
	}

	idx = fssource->find("uniform"_r);
	name = null;
	while (idx != invalid_index) {
		end = fssource->find(";"_r, idx);
		tex = fssource->find("sampler2D"_r, idx, end);
		if (tex != invalid_index) {
			beg = fssource->find_reverse(" "_r, end, idx);
			if (beg != invalid_index) {
				name = fssource->sub_string(beg + 1, end);
				m_samplers[name] = glGetUniformLocation(m_program, name->cstr().cstr<text::encoding::ascii>().cstr());
			}
		}
		else
		{
			tex = fssource->find("samplerCube"_r, idx, end);
			if (tex != invalid_index) {
				beg = fssource->find_reverse(" "_r, end, idx);
				if (beg != invalid_index) {
					name = fssource->sub_string(beg + 1, end);
					m_samplers[name] = glGetUniformLocation(m_program, name->cstr().cstr<text::encoding::ascii>().cstr());
				}
			}
			else
			{
				beg = fssource->find_reverse(" "_r, end, idx);
				if (beg != invalid_index) {
					name = fssource->sub_string(beg + 1, end);
					m_uniforms[name] = glGetUniformLocation(m_program, name->cstr().cstr<text::encoding::ascii>().cstr());
				}
			}
		}
		idx = fssource->find("uniform"_r, end);
	}

	return error_code::success;
}

core::async::iasync_op<gl_shaders_t> gl_shaders::init_async(gl_context_t context, string vsfile, string psfile)
{
	m_context = context;
	return context->create_task<optional<gl_shaders_t>>([=]()->optional<gl_shaders_t>
	{
		m_context = context;

		core::files::input_text_file_t file;
		if (!core::files::ifile_system::instance()->open(vsfile, &file, "$(Data)"_r))
			return error_code::file_not_found;
		auto sz = file->size();
		string vssource = "";
		file->read(vssource, sz);
		file = null;

		if (!core::files::ifile_system::instance()->open(psfile, &file, "$(Data)"_r))
			return error_code::file_not_found;
		sz = file->size();
		string fssource = "";
		file->read(fssource, sz);
		file = null;

		core::async::scope_locker<gl_context_t> lock = context;

		auto vs = load_shader(GL_VERTEX_SHADER, vssource->cstr().cstr<text::encoding::ascii>());
		if (vs.failed()) {
			return vs.error();
		}

		auto fs = load_shader(GL_FRAGMENT_SHADER, fssource->cstr().cstr<text::encoding::ascii>());
		if (fs.failed()) {
			glDeleteShader(vs.get());
			return fs.error();
		}

		auto prog = load_program(vs.get(), fs.get());
		if (prog.failed()) {
			glDeleteShader(vs.get());
			glDeleteShader(fs.get());
			return prog.error();
		}

		m_program = prog.get();
		m_vertex_shader = vs.get();
		m_fragment_shader = fs.get();

		windex beg = invalid_index, end = invalid_index;
		windex idx = vssource->find("attribute"_r);
		windex tex = 0;
		string name = null;
		while (idx != invalid_index) {
			end = vssource->find(";"_r, idx);
			beg = vssource->find_reverse(" "_r, end, idx);
			if (beg != invalid_index) {
				name = vssource->sub_string(beg + 1, end);
				m_attributes[name] = glGetAttribLocation(m_program, name->cstr().cstr<text::encoding::ascii>().cstr());
			}
			idx = vssource->find("attribute"_r, end);
		}

		idx = vssource->find("uniform"_r);
		name = null;
		while (idx != invalid_index) {
			end = vssource->find(";"_r, idx);
			tex = vssource->find("sampler2D"_r, idx, end);

			if (tex != invalid_index) {
				beg = vssource->find_reverse(" "_r, end, idx);
				if (beg != invalid_index) {
					name = vssource->sub_string(beg + 1, end);
					m_samplers[name] = glGetUniformLocation(m_program, name->cstr().cstr<text::encoding::ascii>().cstr());
				}
			}
			else
			{
				tex = vssource->find("samplerCube"_r, idx, end);
				if (tex != invalid_index) {
					beg = vssource->find_reverse(" "_r, end, idx);
					if (beg != invalid_index) {
						name = vssource->sub_string(beg + 1, end);
						m_samplers[name] = glGetUniformLocation(m_program, name->cstr().cstr<text::encoding::ascii>().cstr());
					}
				}
				else
				{
					beg = vssource->find_reverse(" "_r, end, idx);
					if (beg != invalid_index) {
						name = vssource->sub_string(beg + 1, end);
						m_uniforms[name] = glGetUniformLocation(m_program, name->cstr().cstr<text::encoding::ascii>().cstr());
					}
				}
			}
			idx = vssource->find("uniform"_r, end);
		}

		idx = fssource->find("uniform"_r);
		name = null;
		while (idx != invalid_index) {
			end = fssource->find(";"_r, idx);
			tex = fssource->find("sampler2D"_r, idx, end);
			if (tex != invalid_index) {
				beg = fssource->find_reverse(" "_r, end, idx);
				if (beg != invalid_index) {
					name = fssource->sub_string(beg + 1, end);
					m_samplers[name] = glGetUniformLocation(m_program, name->cstr().cstr<text::encoding::ascii>().cstr());
				}
			}
			else
			{
				tex = fssource->find("samplerCube"_r, idx, end);
				if (tex != invalid_index) {
					beg = fssource->find_reverse(" "_r, end, idx);
					if (beg != invalid_index) {
						name = fssource->sub_string(beg + 1, end);
						m_samplers[name] = glGetUniformLocation(m_program, name->cstr().cstr<text::encoding::ascii>().cstr());
					}
				}
				else
				{
					beg = fssource->find_reverse(" "_r, end, idx);
					if (beg != invalid_index) {
						name = fssource->sub_string(beg + 1, end);
						m_uniforms[name] = glGetUniformLocation(m_program, name->cstr().cstr<text::encoding::ascii>().cstr());
					}
				}
			}
			idx = fssource->find("uniform"_r, end);
		}

		return this;
	});
}

int gl_shaders::sampler(cstr_t name)const
{
	auto it = m_samplers.find(name);
	return it.is_valid() ? it->value : 0;
}

int gl_shaders::uniform(cstr_t name)const
{
	auto it = m_uniforms.find(name);
	return it.is_valid() ? it->value : 0;
}

int gl_shaders::attribute(cstr_t name)const
{
	auto it = m_attributes.find(name);
	return it.is_valid() ? it->value : 0;
}

void gl_shaders::use_program()const
{
	glUseProgram(m_program);
}