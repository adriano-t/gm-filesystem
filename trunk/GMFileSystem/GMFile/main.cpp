#include <iostream>
#include <locale>

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
		auto f = file_text_open_read("c:\\test.txt");
		auto str = file_text_read_string(f);

		/*auto file = ::file_text_open_read("testfile.txt");
		auto tmp(::file_text_read_char(file, 4));
		std::cout << tmp << std::endl;

		file_text_unread(file);

		std::cout << file_text_read_real(file) << std::endl;
		file_text_readln(file);
		std::cout << file_text_read_real(file) << std::endl;
		file_text_readln(file);
		std::cout << file_text_read_real(file) << std::endl;
		file_text_readln(file);
		std::cout << file_text_read_real(file) << std::endl;
		file_text_readln(file);
		std::cout << file_text_read_real(file) << std::endl;
		std::cout << file_text_read_real(file) << std::endl;
		file_text_readln(file);
		std::cout << file_text_read_string(file) << std::endl;
		file_text_readln(file);//*/
		std::cout << std::endl << std::endl;
		


		

		
		//std::cout << boost::lexical_cast<double>("1,253") << std::endl;
		return 0;
	}
#endif