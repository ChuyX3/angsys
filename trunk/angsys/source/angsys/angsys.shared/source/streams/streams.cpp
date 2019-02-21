#include "pch.h"
#include <ang/streams.h>

using namespace ang;
using namespace ang::streams;

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::streams::istream, interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::streams::iinput_stream, interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::streams::ioutput_stream, interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::streams::itext_input_stream, iinput_stream);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::streams::itext_output_stream, ioutput_stream);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::streams::ibinary_input_stream, iinput_stream);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::streams::ibinary_output_stream, ioutput_stream);

safe_enum_rrti2(ang::streams, stream_mode);
safe_enum_rrti2(ang::streams, special_command);
safe_enum_rrti2(ang::streams, stream_reference);

