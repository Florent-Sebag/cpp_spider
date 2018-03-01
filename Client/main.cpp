#include "Spider.h"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main()
{
	Spider		App;

	App.Run();
	return (0);
}