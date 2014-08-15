#pragma once
#include <string>
#include <deque>
#include <boost/filesystem.hpp>
#include "../GMFile/standard_functions.h"

#define GMEXPORT extern "C" __declspec (dllexport)

os_path_type find_unique_fname(const os_string_type&  extension);
os_path_type find_unique_fname(const os_path_type& directory, const os_string_type&  extension);

boost::filesystem::path _copy_keep_extension(const boost::filesystem::path& filename, const os_string_type& extension);

bool export_direct(const boost::filesystem::path& temppath, const boost::filesystem::path& filename);

GMEXPORT const char* copy_fast(const char* filename); 

GMEXPORT double set_gm_save_area(const char* directory);
GMEXPORT double set_working_directory(const char* filename);
GMEXPORT double clean_temporary();
GMEXPORT const char*  find_unique_fname(const char* directory, const char* extension);

GMEXPORT double export_sound(const char* tempfile, const char* filename); 
GMEXPORT double export_raw(const char* tempfile, const char* filename); 


extern std::deque<os_path_type> added_files;
extern os_path_type working_dir;
extern os_path_type save_dir;