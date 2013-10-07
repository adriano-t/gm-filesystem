#pragma once
#include <string>
#include <deque>
#include <boost/filesystem.hpp>

#define GMEXPORT extern "C" __declspec (dllexport)

boost::filesystem::path find_unique_fname(const std::string&  extension);
boost::filesystem::path find_unique_fname(const boost::filesystem::path directory, const std::string&  extension);

std::string _copy_keep_extension(const std::string& filename, const std::string& extension);

bool export_direct(const std::string& tempfile, const std::string& filename);

GMEXPORT const char* copy_fast(const char* filename); 

GMEXPORT double set_gm_save_area(const char* directory);
GMEXPORT double set_working_directory(const char* filename);
GMEXPORT double clean_temporary();
GMEXPORT const char*  find_unique_fname(const char* directory, const char* extension);

GMEXPORT double export_sound(const char* tempfile, const char* filename); 
GMEXPORT double export_raw(const char* tempfile, const char* filename); 


extern std::deque<boost::filesystem::path> added_files;
extern boost::filesystem::path working_dir;
extern boost::filesystem::path save_dir;