#include "pch.h"
#include "console.h"

using namespace ang;
using namespace ang::platform::console;


//////////////////////////////////////////////////////////////////

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::events::iconsole_key_event_args, ang::platform::events::imsg_event_args);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::events::iconsole_mouse_event_args, ang::platform::events::imsg_event_args);

namespace ang
{
	namespace platform
	{
		namespace events
		{
			class console_mouse_event_args
				: public object
				, iconsole_mouse_event_args
			{

			};
		}
	}

}


//////////////////////////////////////////////////////////////////

app::app()
	: input_handle(GetStdHandle(STD_INPUT_HANDLE))
	, output_handle(GetStdHandle(STD_OUTPUT_HANDLE))
	, output(null)
{
	output = new console::output();
}

app::~app()
{
	output.set(null);
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::console::app);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::console::app, object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::console::app, object);

int app::run(array<string> args)
{
	if (init() != 0) 
		return -1;

	while (loop());

	return exit();
}

int app::init()
{
	return 0;
}

bool app::loop()
{
	dword readed;
	INPUT_RECORD event;
	if (ReadConsoleInputA(input_handle, &event, 1, &readed) == FALSE)
		return false;
	
	switch (event.EventType)
	{
	case MOUSE_EVENT:
		on_mouse_event(&event.Event.MenuEvent);
		break;
	case KEY_EVENT:
		on_key_event(&event.Event.KeyEvent);
		break;
	case FOCUS_EVENT:
		break;
	default:
		break;
	}

	return true;
}

int app::exit()
{
	return 0;
}

void app::on_mouse_event(pointer e)
{
	auto event = reinterpret_cast<PMOUSE_EVENT_RECORD>(e);

	
}

void app::on_key_event(pointer e)
{
	auto event = reinterpret_cast<PKEY_EVENT_RECORD>(e);
}