
#include <ctime>
#include <iostream>
#include <Windows.h>

#include "autoconfig.h"
#include "image_handling.h"
#include "../GMFile/standard_functions.h"




#if 0
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

int main(int argc, char* argv[])
{
	_CrtSetBreakAlloc(451);

	//::set_working_directory("%localappdata%\\fs_test\\moartest");
	//std::clock_t begin(std::clock());
	//std::clock_t end(std::clock());

	//int m(0);
	////m=0;
	////begin = std::clock();
	////for (int j(0); j < 1; ++j) {
	////	for (int i(0); i < 1; ++i) {
	////		::_copy_convert("img.png");
	////		
	////		++m;
	////	}
	////	clean_temporary();
	////}
	////end = std::clock();
	////
	////std::cout << "PNG, hardlink: " << end - begin <<  std::endl;
	////std::cout << "     per file: "  << double(end - begin) / m << std::endl;
	////
	//m=0;
	//begin = std::clock();
	//for (int j(0); j < 1; ++j) {
	//	for (int i(0); i < 1; ++i) {
	//		::_copy_convert("img.jpg");
	//		
	//		++m;
	//	}
	//	clean_temporary();
	//}
	::_copy_convert("img.jpg");
	::_copy_convert("img.jpg");
	::_copy_convert("img.jpg");
	clean_temporary();
	//end =std::clock();
	//std::cout << "   TIFF, copy: " << end - begin <<  std::endl;
	//std::cout << "     per file: "  << double(end - begin) / m << std::endl;
	_CrtDumpMemoryLeaks();
}
#endif