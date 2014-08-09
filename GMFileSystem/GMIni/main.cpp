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

	const char*  c = "c:\\\xD0\x92.ini";
	ini_data inifile(MakeRichPath(string_convert<os_string_type>(c)));
	const char*  key = "\xD0\xB2";
	auto r2 = inifile.read_string("test", key, "nothing");

	//inifile.load

	double v(::ini_open_ext("test123.ini"));
	std::cout << ini_read_string_ext(v, "prefs", "sound", "-1") << std::endl;
	std::cout << ini_read_string_ext(v, "prefs", "p3.name", "-1") << std::endl;
	std::cout << ini_write_string_ext(v, "prefs", "p3.name", "omg") << std::endl;
	ini_close_ext(v);

	v = (::ini_open("test123.ini"));
	std::cout << ini_read_string( "prefs", "sound", "-1") << std::endl;
	std::cout << ini_read_string("prefs", "p3.name", "-1") << std::endl;
	std::cout << ini_write_string("prefs", "p3.name", "omg") << std::endl;
	ini_close();

	return 0;
}
#endif