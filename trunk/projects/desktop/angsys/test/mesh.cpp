#include "pch.h"
#include "test.h"

using namespace ang;
using namespace ang::graphics;

template<typename T> void parse_vector(cstr_t const& code, windex& i, T&);

template<> void parse_vector(cstr_t const& code, windex& i, maths::float4& out)
{
    out.set<0>(text::parser::to_floating(code, i));
    text::parser::seek(code, i, " , ");
    out.set<1>(text::parser::to_floating(code, i));
    text::parser::seek(code, i, " , ");
    out.set<2>(text::parser::to_floating(code, i));
    text::parser::seek(code, i, " , ");
    out.set<3>(text::parser::to_floating(code, i));
}

template<> void parse_vector(cstr_t const& code, windex& i, maths::float3& out)
{
    out.set<0>(text::parser::to_floating(code, i));
    text::parser::seek(code, i, " , ");
    out.set<1>(text::parser::to_floating(code, i));
    text::parser::seek(code, i, " , ");
    out.set<2>(text::parser::to_floating(code, i));
}

template<> void parse_vector(cstr_t const& code, windex& i, maths::float2& out)
{
    out.set<0>(text::parser::to_floating(code, i));
    text::parser::seek(code, i, " , ");
    out.set<1>(text::parser::to_floating(code, i));
}


gl_mesh::gl_mesh()
	: m_index_count(0)
	, m_vertex_count(0)
	, m_index_buffer_id(0)
	, m_vertex_buffer_id(0)
{

}

void gl_mesh::dispose()
{
	if (m_index_buffer_id || m_vertex_buffer_id && m_context.is_valid()) {
		m_context.lock()->create_task<void>([&]()
		{
            core::async::scope_locker<gl_context_t> lock = m_context.lock();
			if (m_index_buffer_id)
			{
				glDeleteBuffers(1, &m_index_buffer_id);
				m_index_buffer_id = NULL;
			}
			if (m_vertex_buffer_id)
			{
				glDeleteBuffers(1, &m_vertex_buffer_id);
				m_vertex_buffer_id = NULL;
			}
		})->result();
	}
	m_index_count = 0;
	m_vertex_count = 0;
	m_context = null;
}

error gl_mesh::init(gl_context_t context, string path)
{
    core::files::input_text_file_t file;
    if (!core::files::ifile_system::instance()->open(path, &file, "$(Data)"_r))
        return error_code::file_not_found;

    array<vertex> vertices;
    array<uint> indices;
    if (!file->map2([&](string data)->bool
    {
        cstr_t code = data;
        windex i = 0, c = 0;

        enum {
            VERTICES = 0,
            INDICES,
            END,
        }state = VERTICES;
        while (state != END) {
            switch (state)
            {
            case VERTICES:
                if (vertices.is_empty()) {
                    text::parser::seek(code, i, "NrVertices:");
                    m_vertex_count = text::parser::to_signed(code, i);
                    vertices.size(m_vertex_count);
                }
                else
                {
                    vertex v;
                    c = text::parser::to_unsigned(code, i);
                    text::parser::seek(code, i, ". pos:[");
                    parse_vector(code, i, v.position);
                    text::parser::seek(code, i, "]; norm:[");
                    parse_vector(code, i, v.normal);
                    text::parser::seek(code, i, "]; binorm:[");
                    parse_vector(code, i, v.binormal);
                    text::parser::seek(code, i, "]; tgt:[");
                    parse_vector(code, i, v.tangent);
                    text::parser::seek(code, i, "]; uv:[");
                    parse_vector(code, i, v.texcoord);
                    text::parser::seek(code, i, "]; \n");
                    vertices[c] = v;

                    if ((c + 1) >= vertices.size())
                    {
                        state = INDICES; // vertices is already readed..                                                                                          
                    }
                }
                break;
            case INDICES:
                if (indices.is_empty()) {
                    text::parser::seek(code, i, "NrIndices:");
                    m_index_count = text::parser::to_signed(code, i);
                    indices.size(m_index_count);
                }
                else
                {
                    c = text::parser::to_unsigned(code, i);
                    text::parser::seek(code, i, ". ");
                    indices[c * 3] = text::parser::to_unsigned(code, i);
                    text::parser::seek(code, i, ", ");
                    indices[c * 3 + 1] = text::parser::to_unsigned(code, i);
                    text::parser::seek(code, i, ", ");
                    indices[c * 3 + 2] = text::parser::to_unsigned(code, i);
                    text::parser::seek(code, i, "\n");

                    if ((c + 1) >= indices.size() / 3)
                    {
                        state = END; // vertices is already readed..                                                                                          
                    }
                }
                break;
            }
        }
        return true;
    }))
        return error_code::unknown;

    auto ar = to_array((uint*)&indices[0], (uint*)&indices[0] + m_index_count);

    core::async::scope_locker<gl_context_t> lock = context;
    glGenBuffers(1, &m_vertex_buffer_id);// VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, m_vertex_count * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &m_index_buffer_id);// IBO
    glBindBuffer(GL_ARRAY_BUFFER, m_index_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, (m_index_count) * sizeof(uint), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return error_code::success;
    
}

core::async::iasync_op<gl_mesh_t> gl_mesh::init_async(gl_context_t context, string path)
{
    m_context = context;
    return context->create_task<optional<gl_mesh_t>>([=]()->optional<gl_mesh_t>
    {
        core::files::input_text_file_t file;
        if (!core::files::ifile_system::instance()->open(path, &file, "$(Data)"_r))
            return error_code::file_not_found;

        array<vertex> vertices;
        array<uint> indices;
        if (!file->map2([&](string data)->bool
        {
            cstr_t code = data;
            windex i = 0, c = 0;

            enum {
                VERTICES = 0,
                INDICES,
                END,
            }state = VERTICES;
            while (state != END) {
                switch (state)
                {
                case VERTICES:
                    if (vertices.is_empty()) {
                        text::parser::seek(code, i, "NrVertices:");
                        m_vertex_count = text::parser::to_signed(code, i);
                        vertices.size(m_vertex_count);
                    }
                    else
                    {
                        vertex v;
                        c = text::parser::to_unsigned(code, i);
                        text::parser::seek(code, i, ". pos:[");
                        parse_vector(code, i, v.position);
                        text::parser::seek(code, i, "]; norm:[");
                        parse_vector(code, i, v.normal);
                        text::parser::seek(code, i, "]; binorm:[");
                        parse_vector(code, i, v.binormal);
                        text::parser::seek(code, i, "]; tgt:[");
                        parse_vector(code, i, v.tangent);
                        text::parser::seek(code, i, "]; uv:[");
                        parse_vector(code, i, v.texcoord);
                        text::parser::seek(code, i, "]; \n");
                        vertices[c] = v;

                        if ((c + 1) >= vertices.size())
                        {
                            state = INDICES; // vertices is already readed..                                                                                          
                        }
                    }
                    break;
                case INDICES:
                    if (indices.is_empty()) {
                        text::parser::seek(code, i, "NrIndices:");
                        m_index_count = text::parser::to_signed(code, i);
                        indices.size(m_index_count);
                    }
                    else
                    {
                        c = text::parser::to_unsigned(code, i);
                        text::parser::seek(code, i, ". ");
                        indices[c * 3] = text::parser::to_unsigned(code, i);
                        text::parser::seek(code, i, ", ");
                        indices[c * 3 + 1] = text::parser::to_unsigned(code, i);
                        text::parser::seek(code, i, ", ");
                        indices[c * 3 + 2] = text::parser::to_unsigned(code, i);
                        text::parser::seek(code, i, "\n");
                 
                        if ((c + 1) >= indices.size() / 3)
                        {
                            state = END; // vertices is already readed..                                                                                          
                        }
                    }
                    break;
                }
            }
            return true;
        }))
            return error_code::unknown;
        
        auto ar = to_array((uint*)&indices[0], (uint*)&indices[0] + m_index_count);

        core::async::scope_locker<gl_context_t> lock = context;

        glGenBuffers(1, &m_vertex_buffer_id);// VBO
        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, m_vertex_count * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &m_index_buffer_id);// IBO
        glBindBuffer(GL_ARRAY_BUFFER, m_index_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, (m_index_count) * sizeof(uint), &indices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return this;
    });
}

uint gl_mesh::index_count()const
{
	return m_index_count;
}

uint gl_mesh::vertex_count()const
{
	return m_vertex_count;
}

uint gl_mesh::index_buffer()const
{
	return m_index_buffer_id;
}

uint gl_mesh::vertex_buffer()const
{
	return m_vertex_buffer_id;
}
