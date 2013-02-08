#include <iostream>
#include <boost/filesystem.hpp>

#include <windows.h>
//#include <stack>

#include "standard_functions.h"
#include "file_handling.h"
#include "file_binary_handling.h"
#include "filesystem.h"
#include "possix_file_find.h"


//std::FILE

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
int main() {

	std::cout << ::directory_exists("c:\\windddows") << std::endl;

	//int i(0);
	//std::string s(::file_find_first("%localappdata%\\fs_test\\*test*", GMdirectory));
	//while (s != "") {
	//	std::cout <<i++ << " " << s << std::endl;
	//	s = file_find_next();
	//}
	//file_find_close();


	////std::cout << (::_get_attributes("%localappdata%\\fs_test\\fsystest") & GMdirectory)<< std::endl;

	//

	//system("PAUSE");
	return 0;
}
#endif