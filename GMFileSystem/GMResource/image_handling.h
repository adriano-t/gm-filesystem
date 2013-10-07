#pragma once
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#define GMEXPORT extern "C" __declspec (dllexport)


std::string getOutputDir();

std::string _copy_using_opencv(const std::string& filename);




bool export_image_opencv(const std::string& tempfile, const std::string& filename,const std::vector<int>& params = std::vector<int>());



GMEXPORT const char* copy_convert(const char* filename); 



GMEXPORT double export_image(const char* tempfile, const char* filename); 
GMEXPORT double export_image_adv(const char* tempfile, const char* filename, double param);

