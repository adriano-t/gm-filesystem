
#include <iostream>
#include <Windows.h>

#include <tinyxml2.h>

#include "xmlinterface.h"
#include "xmldata.h"



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
	double file(::xml_open("testxml.xml"));

	double top(::xml_root_element(file));
	double sub(::xml_child_find_elem(file, "mymain.sub1"));
	std::cout << ::xml_get_elem_data(file, sub) << std::endl;
	

	::xml_insert_end_elem(file, top, "test", "hello world");
	double inserted(::xml_insert_end_elem(file, sub, "test2", "deepzors"));
	::xml_insert_end_elem(file, inserted, "test2", "deep");
	xml_close(file);
	//s->SetValue("thisisatest");

}
#endif