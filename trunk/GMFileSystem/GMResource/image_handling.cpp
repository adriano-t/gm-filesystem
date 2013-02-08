
#include <string>
#include <array>
#include <deque>
#include <vector>
#include <algorithm>

#include <boost/filesystem.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

#ifdef _DEBUG
#include <Windows.h>
#include <boost\lexical_cast.hpp>
#endif

#include "../GMFile/standard_functions.h"
#include "image_handling.h"


static std::string RetString;
static std::deque<boost::filesystem::path> added_files;
static boost::filesystem::path working_dir;
static boost::filesystem::path save_dir ;///ONLY THANKS TO A MISSING FEATURE IN GM THIS HAS TO BE DONE

const static std::array<unsigned char, 8> png_header = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
const static std::array<unsigned char, 2> jpeg_header = {0xFF, 0xD8};
const static std::array<unsigned char, 4> tiffII_header = {0x49, 0x49, 0x2A, 0x00};
const static std::array<unsigned char, 4> tiffMM_header = {0x4D, 0x4D, 0x00, 0x2A};

inline const char* ReturnString(const std::string& val)
{
	RetString = val;
	return RetString.c_str();
}

bool export_direct(const std::string& tempfile, const std::string& filename) 
{
	boost::filesystem::path temppath = tempfile;
	added_files.push_back(temppath);
	try {
		boost::filesystem::create_hard_link(temppath, filename); //create hard link should work....
	} catch (boost::filesystem::filesystem_error& err) {
		auto c(err.code().value());
		if (c == boost::system::errc::too_many_links || c == 1142) {
			try {
				boost::filesystem::copy(temppath, filename); //fallback option to copy
			} catch (boost::filesystem::filesystem_error& err2) {
#ifdef _DEBUG
				std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
				tstr += err2.what();
				::MessageBoxA(0, tstr.c_str(), "copy fail",MB_ICONERROR);
#endif
				return false;
			}
		} else {
#ifdef _DEBUG
			std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
			tstr += err.what();
			MessageBoxA(0, tstr.c_str(), "hardlink fail" ,MB_ICONERROR);
#endif
			return false;
		}
	}
	return true;
}
bool export_image_opencv(const std::string& tempfile, const std::string& filename, const std::vector<int>& params)
{
	boost::filesystem::path temppath =  tempfile;
	added_files.push_back(temppath);
	try {
		cv::Mat img(cv::imread(temppath.string(), -1));
		if (!img.data) {
			#ifdef _DEBUG
				std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
				tstr += "could not load data from file ";
				tstr += temppath.string();
				::MessageBoxA(0, tstr.c_str(), "OpenCV import fail",MB_ICONERROR);
			#endif
			return false; //invalid image format
		}
		cv::imwrite(filename, img, params);
	} catch (cv::Exception& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "OpenCV fail",MB_ICONERROR);
#endif
		return false;
	}
	return true;
}

boost::filesystem::path find_unique_fname(const std::string& extension) 
{
	return find_unique_fname(working_dir, extension);
}
boost::filesystem::path find_unique_fname(const boost::filesystem::path directory, const std::string&  extension)
{
	boost::filesystem::path newfilename;
	std::string fname("%%%%");
	do {
		fname += "-%%%%";
		newfilename = boost::filesystem::unique_path(directory / (fname + extension));
	} while (boost::filesystem::exists(newfilename));

	assert(boost::filesystem::exists(newfilename) == false);

	return newfilename;
}

std::string _copy_keep_extension(const std::string& filename, const std::string& extension) 
{
	boost::filesystem::path newfilename(find_unique_fname(extension));
	try {
		boost::filesystem::create_hard_link(filename, newfilename); //create hard link should work....
		added_files.push_back(newfilename);
	} catch (boost::filesystem::filesystem_error& err) {
		auto c(err.code().value());
		if (c == boost::system::errc::too_many_links || c == 1142) {
			try {
				boost::filesystem::copy(filename, newfilename); //fallback option to copy
				added_files.push_back(newfilename);
			} catch (boost::filesystem::filesystem_error& err2) {
#ifdef _DEBUG
				std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
				tstr += err2.what();
				::MessageBoxA(0, tstr.c_str(), "copy fail",MB_ICONERROR);
#endif
				return "";
			}
		} else {
#ifdef _DEBUG
			std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
			tstr += err.what();
			MessageBoxA(0, tstr.c_str(), "hardlink fail" ,MB_ICONERROR);
#endif
			return "";
		}
	}
	return newfilename.string();
}
std::string _copy_using_opencv(const std::string& filename)
{
	std::string newfilename(find_unique_fname().string());
	try {
		cv::Mat img(cv::imread(filename, -1));
		if (!img.data) {
			#ifdef _DEBUG
				std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
				tstr += "could not load data from file ";
				tstr += newfilename;
				::MessageBoxA(0, tstr.c_str(), "OpenCV import fail",MB_ICONERROR);
			#endif
			return ""; //invalid image format
		}
		cv::imwrite(newfilename, img);
		added_files.push_back(newfilename);
	} catch (cv::Exception& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "OpenCV fail",MB_ICONERROR);
#endif
		return "";
	}
	return newfilename;
}

GMEXPORT const char* copy_convert(const char* filename) 
{
	std::string file(MakeRichPath(filename));
	std::FILE *fp = std::fopen(file.c_str(), "rb");

	if (!fp) {
		#ifdef _DEBUG
			std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
			tstr += "could open file ";
			tstr += file;
			::MessageBoxA(0, tstr.c_str(), "Generic File Fail",MB_ICONERROR);
		#endif
		return ReturnString("");
	}
	std::array<unsigned char, 8> header;
	std::fread(&header, 1, 8, fp);
	std::fclose(fp);
	if (std::search(header.begin(), header.end(), png_header.begin(), png_header.end()) == header.begin()) {
		//png
		return ReturnString(_copy_keep_extension(file, ".png"));
	} else {
		return ReturnString(_copy_using_opencv(file));
	}

	return ReturnString("");
}
GMEXPORT const char* copy_convert_fast(const char* filename) 
{
	std::string file(MakeRichPath(filename));
	std::string ext( file.substr(file.rfind('.')));
	return ReturnString(_copy_keep_extension(file, ext));
}

GMEXPORT double export_image(const char* tempfile, const char* filename)
{
	std::string importfile(MakeRichPath(tempfile));
	std::string exportfile(MakeRichPath(filename));
	std::string ext( exportfile.substr(exportfile.rfind('.')));
	
	//boost::filesystem::path temppath =  save_dir / tempfile;
	if (ext == ".bmp") {
		return export_direct(importfile, exportfile);
	} else {
		return export_image_opencv(importfile, exportfile);
	}
	return 0;
}
GMEXPORT double export_image_adv(const char* tempfile, const char* filename, double param)
{
	std::string importfile(MakeRichPath(tempfile));
	std::string exportfile(MakeRichPath(filename));
	std::string ext( exportfile.substr(exportfile.rfind('.')));
	//boost::filesystem::path temppath =  save_dir / tempfile;
	std::vector<int> compression_parameters;
	int param_type(-1);
	if (ext == ".bmp") {
		return export_direct(importfile, exportfile);
	} else if (ext == ".png") {
		param_type = CV_IMWRITE_PNG_COMPRESSION;	
	} else if (ext == ".jpg" || ext == ".jpeg" || ext == ".jpe") {
		param_type = CV_IMWRITE_JPEG_QUALITY;
	} else if (ext == ".pgm" || ext == ".pbm" || ext == ".ppm") {
		param_type = CV_IMWRITE_PXM_BINARY;
	}
	if (param != -1) {
		compression_parameters.push_back(param_type);
		compression_parameters.push_back(static_cast<int>(param));
	}
	return export_image_opencv(importfile, exportfile, compression_parameters);


}
GMEXPORT double export_sound(const char* tempfile, const char* filename)
{
	return export_direct(tempfile, filename);
}


GMEXPORT double set_working_directory(const char* filename) 
{
	working_dir = MakeRichPath(filename);
	return 0;
}
GMEXPORT double set_gm_save_area(const char* directory)
{ //ONLY THANKS TO A MISSING FEATURE IN GM THIS HAS TO BE DONE
	save_dir = MakeRichPath(directory);
	return 0;
}
GMEXPORT double clean_temporary()
{
	int n(0);
	boost::system::error_code ec;
	while (!added_files.empty()) {
		auto t(added_files.back().string());
		bool b(boost::filesystem::exists(t));
		try {
			boost::filesystem::remove(t);
		} catch (boost::filesystem::filesystem_error err) {
			auto w(err.what());
			auto c(err.code());
			auto m(c.message());
			auto v(c.value());
		}
		added_files.pop_back();
		++n;
	}
	return n;
}

GMEXPORT const char*  find_unique_fname(const char* directory, const char* extension)
{
	std::string d(directory);
	if (d == "") {
		return ReturnString(find_unique_fname(save_dir, std::string(extension)).string());
	} else {
		return ReturnString(find_unique_fname(boost::filesystem::path(directory), std::string(extension)).string());
	}
}