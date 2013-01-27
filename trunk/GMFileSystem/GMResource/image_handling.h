#pragma once
#include <string>
#include <boost/filesystem.hpp>
#define GMEXPORT extern "C" __declspec (dllexport)


std::string getOutputDir();

std::string _copy_png(const std::string&  filename);
std::string _copy_jpeg(const std::string& filename);
std::string _copy_tiff(const std::string& filename);
std::string _copy_keep_extension(const std::string& filename, const std::string& extension);
boost::filesystem::path find_unique_fname(const std::string&  extension = ".png");

bool export_jpeg(const std::string& tempfile, const std::string& filename);
bool export_direct(const std::string& tempfile, const std::string& filename);

GMEXPORT double set_working_directory(const char* filename);
GMEXPORT double clean_temporary();
GMEXPORT const char* copy_convert(const char* filename); 
GMEXPORT const char* copy_convert_fast(const char* filename); 

GMEXPORT double export_convert(const char* tempfile, const char* filename); 
GMEXPORT double set_gm_save_area(const char* directory);