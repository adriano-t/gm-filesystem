
#include <ctime>
#include <iostream>
#include <Windows.h>

#include "image_handling.h"
#include "allresources.h"
#include "../GMFile/standard_functions.h"



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

int main(int argc, char* argv[])
{
	//_T("test");
	::set_working_directory("%appdata%\\fs_test");
	::set_gm_save_area("%appdata%\\fs_test");
	
	::export_image("C:\\Users\\user\\AppData\\Roaming\\fs_test\\\xD0\x92.jpg", "c:\\\xD0\x94\xD2\x8A.bmp");
	
}
#endif