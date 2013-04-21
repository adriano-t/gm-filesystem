#include <iostream>
#include <string>

#include <Windows.h>

#include "inireader.h"

#ifdef _WINDLL
BOOL WINAPI DllMain(
  HANDLE hinstDLL, 
  DWORD dwReason, 
  LPVOID lpvReserved
)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#else
int main(int argc, char** argv)
{
	double v(::ini_open_ext("test123.ini"));
	std::cout << ini_read_string_ext(v, "prefs", "sound", "-1") << std::endl;
	std::cout << ini_read_string_ext(v, "prefs", "p3.name", "-1") << std::endl;
	std::cout << ini_write_string_ext(v, "prefs", "p3.name", "omg") << std::endl;
	ini_close_ext(v);

	return 0;
}
#endif