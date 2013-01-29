
#include <ctime>
#include <iostream>
#include <Windows.h>

#include "autoconfig.h"
#include "image_handling.h"
#include "../GMFile/standard_functions.h"




#if 1
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

	::set_working_directory("%localappdata%\\fs_test\\moartest");
	std::clock_t begin(std::clock());
	std::clock_t end(std::clock());
	::set_gm_save_area("%localappdata%\\fs_test\\moartest");

	int m(0);
	m=0;
	//begin = std::clock();
	//for (int j(0); j < 1; ++j) {
	//	for (int i(0); i < 10; ++i) {
	//		copy_convert("img.bmp");
	//		++m;
	//	}
	//	clean_temporary();
	//}
	::export_image("2828-60ff.png", "thisisatest.ppm");


	++m;
	end = std::clock();
	
	std::cout << "bmp, opencv: " << end - begin <<  std::endl;
	std::cout << "   per file: "  << double(end - begin) / m << std::endl;
	
}
#endif