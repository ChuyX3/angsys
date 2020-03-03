#include "pch.h"

using namespace ang;
using namespace Platform;

// The main function is only used to initialize our IFrameworkView class.
[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	platform::windows::core_app::instance()->run();
	return 0;
}