
#include <iostream>
#include <Windows.h>

#include <tinyxml2.h>

#include "xmlinterface.h"
#include "xmldata.h"

//#include <yaml-cpp\yaml.h>
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
	//::YAML::Node y(YAML::LoadFile("testyaml.txt"));
	YAML::Node n(YAML::Load("YAML: \"test\""));
	YAML::Emitter em;
	em << n;	
	std::string mystr(em.c_str());
	std::cout << mystr << std::endl;
}
#endif