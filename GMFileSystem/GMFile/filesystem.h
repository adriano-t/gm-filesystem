#include <string>
#include <boost/filesystem.hpp>

//directory_exists
//directory_create

//file_exists
//file_delete
//file_rename
//file_copy
//file_find_first
//file_find_next
//file_find_close
//file_attributes

#define GMEXPORT extern "C" __declspec (dllexport)



GMEXPORT double directory_exists(const char* dirname);
GMEXPORT double directory_create(const char* dirname);
GMEXPORT double directory_delete(const char* filename);
GMEXPORT double file_exists(const char* filename);
GMEXPORT double file_delete(const char* filename);
GMEXPORT double file_rename(const char* filename, const char* newname);
GMEXPORT double file_copy(const char* filename, const char* newname);

GMEXPORT const char* file_find_first(const char* mask, double attributes);
GMEXPORT const char* file_find_next();
GMEXPORT double file_find_close();

GMEXPORT double file_attributes(const char* filename);
