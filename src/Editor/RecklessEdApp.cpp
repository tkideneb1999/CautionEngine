#include <vector>
#include <string>

#include "Widgets/Application.h"

#include "Renderer/D3D12API.h"

// configs
bool g_applicationRunning = true;
const wchar_t CLASS_NAME[] = L"Reckless Editor Class";
const wchar_t WINDOW_NAME[] = L"Reckless Editor";


int main(int argc, char** argv)
{
	std::vector<std::string> args(argc);
	for (int i = 0; i < argc; i++) 
	{
		args[i] = argv[i];
	}
	Reckless::Application editor(CLASS_NAME, WINDOW_NAME, args);

	while (g_applicationRunning)
	{
		if (!editor.Update())
			return 0;
	}

	return 0;
}