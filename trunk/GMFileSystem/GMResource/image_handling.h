#pragma once
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#define GMEXPORT extern "C" __declspec (dllexport)


std::string getOutputDir();

std::string _copy_using_opencv(const std::string& filename);
std::string _copy_keep_extension(const std::string& filename, const std::string& extension);
boost::filesystem::path find_unique_fname(const std::string&  extension = ".png");
boost::filesystem::path find_unique_fname(const boost::filesystem::path directory, const std::string&  extension);

bool export_direct(const std::string& tempfile, const std::string& filename);
bool export_image_opencv(const std::string& tempfile, const std::string& filename,const std::vector<int>& params = std::vector<int>());

GMEXPORT double set_working_directory(const char* filename);
GMEXPORT double clean_temporary();

GMEXPORT const char*  find_unique_fname(const char* directory, const char* extension);

GMEXPORT const char* copy_convert(const char* filename); 
GMEXPORT const char* copy_convert_fast(const char* filename); 


GMEXPORT double export_image(const char* tempfile, const char* filename); 
GMEXPORT double export_image_adv(const char* tempfile, const char* filename, double param);
GMEXPORT double export_sound(const char* tempfile, const char* filename); 
GMEXPORT double set_gm_save_area(const char* directory);