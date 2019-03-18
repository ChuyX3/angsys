#include "pch.h"
#include "resources/resources.h"

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::resources;

resource::resource(ilibrary* lib)
	: m_library(lib)
{

}

resource::~resource()
{

}

ANG_IMPLEMENT_INTERFACE_RUNTIME_INFO(ang::graphics::resources::resource);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::resources::resource, object, dom::xml::ixml_serializable, iresource);
//ANG_IMPLEMENT_INTERFACE_QUERY_INTERFACE(ang::graphics::resources::resource, object, dom::xml::ixml_serializable, iresource);

bool resource::query_interface(rtti_t const& type, unknown_ptr_t out)
{
	if (type.type_id() == resource::class_info().type_id())
	{
		if (out == null)
			return false;
		*out = reinterpret_cast<unknown_t>(this);
		return true;
	}
	else if (object::query_interface(type, out))
	{
		return true;
	}
	else if (type.type_id() == dom::xml::ixml_serializable::class_info().type_id())
	{
		if (out == null)
			return false;
		*out = static_cast<dom::xml::ixml_serializable*>(this);
		return true;
	}
	else if (type.type_id() == iresource::class_info().type_id())
	{
		if (out == null)
			return false;
		*out = static_cast<iresource*>(this);
		return true;
	}
	else if (!m_resource.is_empty())
	{
		switch (m_resource->resource_type())
		{
		case resource_type::effect:
			if (type.type_id() == effects::ieffect::class_info().type_id())
			{
				if (out == null)
					return false;
				*out = m_resource->to_effect().get();
				return true;
			}
			break;
		case resource_type::shaders:
			if (type.type_id() == effects::ishaders::class_info().type_id())
			{
				if (out == null)
					return false;
				*out = m_resource->to_shaders().get();
				return true;
			}
			break;
		case resource_type::texture:
			if (type.type_id() == textures::itexture::class_info().type_id())
			{
				if (out == null)
					return false;
				*out = m_resource->to_texture().get();
				return true;
			}
			break;
		case resource_type::index_buffer:
			if (type.type_id() == buffers::iindex_buffer::class_info().type_id())
			{
				if (out == null)
					return false;
				*out = m_resource->to_index_buffer().get();
				return true;
			}
			break;
		case resource_type::vertex_buffer:
			if (type.type_id() == buffers::ivertex_buffer::class_info().type_id())
			{
				if (out == null)
					return false;
				*out = m_resource->to_vertex_buffer().get();
				return true;
			}
			break;
		case resource_type::frame_buffer:
			if (type.type_id() == iframe_buffer::class_info().type_id())
			{
				if (out == null)
					return false;
				*out = m_resource->to_frame_buffer().get();
				return true;
			}
			break;
		}
	}
}

void resource::clear()
{
	m_library = null;
	m_resource = null;
}

resource_type_t resource::resource_type()const
{
	return m_resource.is_empty() ? resource_type::none : m_resource->resource_type().get();
}

string resource::resource_sid()const
{
	return m_resource.is_empty() ? null : m_resource->resource_sid();
}

void resource::resource_sid(cstr_t sid)
{
	if (!m_resource.is_empty())
		m_resource->resource_sid(sid);
}

effects::ieffect_t resource::to_effect()
{
	return m_resource.is_empty() ? null : m_resource->to_effect();
}

effects::ishaders_t resource::to_shaders()
{
	return m_resource.is_empty() ? null : m_resource->to_shaders();
}

textures::itexture_t resource::to_texture()
{
	return m_resource.is_empty() ? null : m_resource->to_texture();
}

iframe_buffer_t resource::to_frame_buffer()
{
	return m_resource.is_empty() ? null : m_resource->to_frame_buffer();
}

buffers::iindex_buffer_t resource::to_index_buffer() 
{
	return m_resource.is_empty() ? null : m_resource->to_index_buffer();
}

buffers::ivertex_buffer_t resource::to_vertex_buffer() 
{
	return m_resource.is_empty() ? null : m_resource->to_vertex_buffer();
}

bool resource::load(dom::xml::xml_node_t node)
{
	cstr_t name = node->xml_name();
	if (name == "technique")
	{
		return static_cast<effects::technique*>(this)->load(node);
	}
	else if (name == "effect")
	{
		effects::effect_t effect = new effects::effect(m_library.lock());
		if (!effect->load(node))
			return false;
		m_resource = effect.get();
	}
	else if (name == "texture")
	{
		return static_cast<textures::texture*>(this)->load(node);
	}
	else if (name == "frame_buffer")
	{
		return static_cast<buffers::frame_buffer*>(this)->load(node);
	}
	else if (name == "index_buffer")
	{
		return static_cast<buffers::index_buffer*>(this)->load(node);
	}
	else if (name == "vertex_buffer")
	{
		return static_cast<buffers::vertex_buffer*>(this)->load(node);
	}
	return false;
}

bool resource::save(dom::xml::xml_document_t node)const
{
	//TODO:
	return false;
}

bool effects::technique::load(dom::xml::xml_node_t node)
{
	using namespace ang::dom;

	if (node.is_empty() || !node->xml_has_children())
		return false;
	string log;
	shader_info vertex;
	shader_info pixel;
	bool res = true;
	string sid = node->xml_attributes()["sid"]->xml_as<cstr_t>();
	for (xml::xml_node_t node : node->xml_children())
	{
		auto name = node->xml_name()->xml_as<cstr_t>();
		if (name == "vertex_shader")
			res&= load_shader(node, vertex);
		else if (name == "pixel_shader")
			res &= load_shader(node, pixel);
	}
	if (!res)
		return false;
	
	auto factory = m_library.lock()->factory();
	auto shaders = factory->compile_shaders(vertex, pixel, sid, &log);
	if (shaders.is_empty())
	{
		//LOGS: log->cstr();
		return false;
	}
	m_resource = shaders->resource();
	return true;
}

bool effects::technique::load_shader(dom::xml::xml_node_t shader, shader_info_t& out)
{
	using namespace ang::dom;
	if (shader->xml_has_children())
	{
		for (xml::xml_node_t node : shader->xml_children())
		{
			auto name = node->xml_name()->xml_as<cstr_t>();
			if (name == "entry")
				out.entry = node->xml_value()->xml_as<cstr_t>();
			else if (name == "file")
				out.file = node->xml_value()->xml_as<cstr_t>();
			else if (name == "code") {
				if (node->xml_has_children())
				{
					try {
						out.code = node->xml_children()["CDATA"]->xml_value()->xml_as<cstr_t>();
					}
					catch (...) { }
				}
				else
				{
					out.code = node->xml_value()->xml_as<cstr_t>();
				}		
			}
			else if (name == "compile_config")
				out.compile_config = node->xml_value()->xml_as<cstr_t>();
		}
		return true;
	}
	else if(shader->xml_has_value())
	{
		int i = 0;
		for (string& str : shader->xml_value()->split(";"))
		{
			switch (i)
			{
			case 0: out.file = str; break;
			case 1: out.entry = str; break;
			case 2: out.compile_config = str; break;
			}
			i++;
		}
		return true;
	}
	return false;
}

bool textures::texture::load(dom::xml::xml_node_t node)
{
	using namespace ang::dom;
	if (node.is_empty())
		return false;

	tex_type_t type;

	string sid = node->xml_attributes()["sid"]->xml_as<cstr_t>();
	if (node->xml_has_children())
	{
		bool res = true;
		auto att = node->xml_attributes();

		type = att["type"]->xml_as<textures::tex_type_t>();
		collections::vector<string> files;

		for (xml::xml_node_t tex_node : node->xml_children())
		{
			cstr_t name = tex_node->xml_name();
			if (name != "texture")
				continue;
			files += tex_node->xml_value()->xml_as<cstr_t>();
		}

		auto factory = m_library.lock()->factory();
		auto tex = factory->load_texture(files, type, sid);
		if (tex.is_empty())
			return false;
		m_resource = tex->resource();
		return true;
	}
	else
	{
		bool res = true;
		auto att = node->xml_attributes();

		type = att["type"]->xml_as<textures::tex_type_t>();
		string file = node->xml_value()->xml_as<cstr_t>();

		auto factory = m_library.lock()->factory();
		auto tex = factory->load_texture(file, type, sid);
		if (tex.is_empty())
			return false;
		m_resource = tex->resource();
		return true;
	}
}

bool buffers::frame_buffer::load(dom::xml::xml_node_t node)
{
	using namespace ang::dom;
	if (node.is_empty() || !node->xml_children())
		return false;
	frame_buffer_desc_t desc;
	string sid = node->xml_attributes()["sid"]->xml_as<cstr_t>();
	for (dom::xml::xml_node_t item : node->xml_children())
	{
		cstr_t name = item->xml_name();
		if (name == "dimentions")
		{
			desc.dimentions = item->xml_value()->xml_as<size<uint>>();
		}
		else if (name == "color_buffer")
		{
			for (dom::xml::xml_node_t format : item->xml_children())
			{
				cstr_t name = format->xml_name();
				if (name != "format")
					continue;
				desc.color_format += format->xml_as<textures::tex_format_t>();
			}
		}
		else if (name == "depth_stencil_buffer")
		{
			desc.depth_stencil_format = item["format"]->xml_value()->xml_as<textures::tex_format_t>();
		}
	}

	auto factory = m_library.lock()->factory();
	auto fbo = factory->create_frame_buffer(
		desc.color_format,
		desc.depth_stencil_format,
		desc.dimentions,
		sid);

	if (fbo.is_empty())
		return false;
	m_resource = fbo->resource();
	return true;
}

bool buffers::index_buffer::load(dom::xml::xml_node_t node)
{
	if (!node->xml_has_children())
		return false;

	iindex_buffer_t ibo;
	auto factory = m_library.lock()->factory();

#if defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
	/*nedded for engine editor to read data from buffers*/
	buffers::buffer_usage_t usage = buffers::buffer_usage::staging;
#else
	buffers::buffer_usage_t usage = buffers::buffer_usage::def;
#endif

	string sid = node->xml_attributes()["sid"]->xml_as<cstr_t>();
	auto type_node = node["data_type"];
	auto data_node = node["data"];

	reflect::var_type_t  type = type_node ? type_node["type"]->xml_value()->xml_as<reflect::var_type_t>() : (reflect::var_type_t)reflect::var_type::u32;
	uint count = data_node["count"]->xml_value()->xml_as<uint>();

	streams::itext_input_stream_t stream = new streams::string_input_stream(data_node->xml_value().get());
	if (stream->is_eos())
		return false;

	switch (type.get())
	{
	case reflect::var_type::u16: {
		//out.index_type = reflect::var_type::u16;
		array<ushort> arr = new collections::array_buffer<ushort>(count);
		for (ushort& item : arr)
			stream >> item >> " , "_s;
		ibo = factory->create_index_buffer(usage, arr, sid);
	}break;

	case reflect::var_type::s16: {
		//out.index_type = reflect::var_type::s16;
		array<short> arr = new collections::array_buffer<short>(count);
		for (short& item : arr)
			stream >> item >> " , "_s;
		ibo = factory->create_index_buffer(usage, arr, sid);
	}break;
	case reflect::var_type::s32: {
		//out.index_type = reflect::var_type::s32;
		array<int> arr = new collections::array_buffer<int>(count);
		for (int& item : arr)
			stream >> item >> " , "_s;
		ibo = factory->create_index_buffer(usage, arr, sid);
	}break;
	default:
	case reflect::var_type::u32: {
		//out.index_type = reflect::var_type::u32;
		array<uint> arr = new collections::array_buffer<uint>(count);
		for (uint& item : arr)
			stream >> item >> " , "_s;
		ibo = factory->create_index_buffer(usage, arr, sid);
	}break;
	}

	if (ibo.is_empty())
		return false;
	m_resource = ibo->resource();
	return true;
}

bool buffers::vertex_buffer::load(dom::xml::xml_node_t node)
{
	if (!node->xml_has_children())
		return false;

	ivertex_buffer_t vbo;
	auto factory = m_library.lock()->factory();

#if defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
	/*nedded for engine editor to read data from buffers*/
	buffers::buffer_usage_t usage = buffers::buffer_usage::staging;
#else
	buffers::buffer_usage_t usage = buffers::buffer_usage::def;
#endif

	string sid = node->xml_attributes()["sid"]->xml_as<cstr_t>();
	auto data_layout = node["data_layout"_s];
	auto data_node = node["data"_s];

	vector<reflect::attribute_desc> vertex_desc;

	if (!reflect::attribute_desc::load(data_layout, vertex_desc))
		return false;

	wsize stride = reflect::attribute_desc::get_size_in_bytes(vertex_desc, 16U);
	wsize stride4 = reflect::attribute_desc::get_size_in_bytes(vertex_desc, 4U);
	if (stride == stride4)
	{
		wsize count = data_node["vertex_count"_s]->xml_value()->xml_as<uint>() * stride / sizeof(float);

		streams::itext_input_stream_t stream = new streams::string_input_stream(data_node->xml_value().get());
		if (stream->is_eos())
			return false;

		array<float, memory::aligned16_allocator> data = new collections::array_buffer<float, memory::aligned16_allocator>();
		data->size(count);
		
		for (float& item : data)
			stream >> item >> " , "_s;

		vbo = factory->create_vertex_buffer(usage, vertex_desc, count * sizeof(float) / stride, data.get(), sid);
	}
	else
	{
		wsize count = data_node["vertex_count"_s]->xml_value()->xml_as<uint>() * stride / sizeof(float);

		streams::itext_input_stream_t stream = new streams::string_input_stream(data_node->xml_value().get());
		if (stream->is_eos())
			return false;

		array<float, memory::aligned16_allocator> data = new collections::array_buffer<float, memory::aligned16_allocator>();
		data->size(count);

		wsize in_size = stride4 / sizeof(float);
		wsize out_size = stride / sizeof(float);

		wsize aligment = 16u;
		for (index i = 0U; i < count; )
		{
			wsize total = 0;
			wsize size = 0;
			wsize temp = 0;
			wsize res = 0;

			for (index j = 0u, c = vertex_desc->counter(); j < c; j++)
			{
				reflect::attribute_desc const& desc = vertex_desc[j];
				size = desc.get_size_in_bytes();
				if (size == 0)continue;//next item
				temp = (total % aligment);
				res = aligment - temp;
				if (res < aligment)
				{
					if (res > size)
						total += align_up(size, temp) - temp;
					else if (res < size)
						total += res;
				}
				for (index k = 0U, d = size / sizeof(float); k < d; ++k)
				{
					stream >> data[i + total / sizeof(float) + k] >> " , "_s;
				}
				total += size;
			}
			i += align_up(aligment, total) / sizeof(float);
		}

		vbo = factory->create_vertex_buffer(usage, vertex_desc, count * sizeof(float) / stride, data.get(), sid);
	}

	if (vbo.is_empty())
		return false;
	m_resource = vbo->resource();
	return true;
}
