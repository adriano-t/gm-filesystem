#include <iostream>
#include <locale>
#include <codecvt>

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/locale.hpp>


#include <windows.h>
//#include <stack>

#include "standard_functions.h"
#include "file_handling.h"
#include "file_binary_handling.h"
#include "filesystem.h"
#include "possix_file_find.h"
#include "filestream_organizer.h"


//std::FILE
#include <fstream>

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
		_setLocale("");
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

//9 >> 3
//

#else

	int main() {
		auto file = ::file_text_open_append_ext("c:\\test_windows.txt", 0, false);
		//::file_text_write_bom(file);
		file_text_write_string(file, "hello \n woradfasdfld");
		file_text_writeln(file);
		file_text_close(file);
		return 0;
	}
#endif