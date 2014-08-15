#pragma once
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#define GMEXPORT extern "C" __declspec (dllexport)


//std::string getOutputDir();

bool file_has_png_header(const boost::filesystem::path& filename);
boost::filesystem::path _copy_using_opencv(const boost::filesystem::path& filename);




bool export_image_opencv(const boost::filesystem::path& tempfile, const boost::filesystem::path& filename,const std::vector<int>& params = std::vector<int>());
std::vector<unsigned char> file_to_bytemap (const boost::filesystem::path& filename);
void bytemap_to_file (const std::vector<unsigned char>& ivec, const boost::filesystem::path& filename);
void _do_copy_opencv(const boost::filesystem::path& importpath, const boost::filesystem::path& exportpath, 
						const std::string& extension = ".png", const std::vector<int>& params = std::vector<int>());


GMEXPORT const char* import_image(const char* filename); 



GMEXPORT double export_image(const char* tempfile, const char* filename); 
GMEXPORT double export_image_adv(const char* tempfile, const char* filename, double param);

