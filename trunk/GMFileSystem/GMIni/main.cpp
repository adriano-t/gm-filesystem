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
	double v(::ini_open_ext("/omgtesta/testaaa.ini"));
	std::cout << ini_read_string_ext(v, "player", "name", "noname") << std::endl;
	std::cout << ini_read_real_ext(v, "achievements", "1", 10) << std::endl;

	ini_write_real_ext(v, "player", "omg", 123);

	std::cout << ini_read_string_ext(v, "player", "test", "noname") << std::endl;

	//ini_section_delete("achievements");

	ini_close_ext(v);

	return 0;
}
#endif