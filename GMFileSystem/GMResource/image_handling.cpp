

#include "autoconfig.h"

#include <string>
#include <array>
#include <deque>
#include <algorithm>

#include <Windows.h>

#include <boost/filesystem.hpp>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <boost/gil/extension/io/jpeg_io.hpp>
#include <boost/gil/extension/io/tiff_io.hpp>



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


GMEXPORT const char* copy_convert(const char* filename) 
{
	std::string file(MakeRichPath(filename));
	std::FILE *fp = std::fopen(file.c_str(), "rb");

	if (!fp) {
		return ReturnString("");
	}
	std::array<unsigned char, 8> header;
	std::fread(&header, 1, 8, fp);
	std::fclose(fp);
	if (std::search(header.begin(), header.end(), png_header.begin(), png_header.end()) == header.begin()) {
		//png
		return ReturnString(_copy_png(file));
	} else if (std::search(header.begin(), header.end(), jpeg_header.begin(), jpeg_header.end()) == header.begin()) {
		//jpeg
		return ReturnString(_copy_jpeg(file));
	} else if (std::search(header.begin(), header.end(), tiffII_header.begin(), tiffII_header.end()) == header.begin() 
		|| std::search(header.begin(), header.end(), tiffMM_header.begin(), tiffMM_header.end()) == header.begin() ) {
		return ReturnString(_copy_tiff(file));
	}

	return ReturnString("");
}
GMEXPORT const char* copy_convert_fast(const char* filename) 
{
	std::string file(MakeRichPath(filename));
	std::string ext( file.substr(file.rfind('.')));
	return ReturnString(_copy_keep_extension(file, ext));
}
GMEXPORT double export_convert(const char* tempfile, const char* filename)
{
	std::string importfile(MakeRichPath(tempfile));
	std::string exportfile(MakeRichPath(filename));
	std::string ext( exportfile.substr(exportfile.rfind('.')));
	boost::filesystem::path temppath =  save_dir / tempfile;
	bool ret;
	if (ext == ".jpg") {
		ret = export_jpeg(importfile, filename);
	} else {
		ret = export_direct(importfile, filename);
	}
	return ret;
}

bool export_jpeg(const std::string& tempfile, const std::string& filename) 
{
	//boost::filesystem::path temppath =  save_dir / tempfile;
	try {
		boost::gil::rgb8_image_t im;
		boost::gil::png_read_image(tempfile, im);
		boost::gil::png_write_view(filename, boost::gil::view(im));
	} catch (...) {
		return false;
	}
	return true;
}
bool export_direct(const std::string& tempfile, const std::string& filename) 
{
	boost::filesystem::path temppath =  /*save_dir /*/ tempfile;
	::MessageBoxA(0, temppath.string().c_str(), "filename: ", MB_ICONASTERISK);
	try {
		boost::filesystem::create_hard_link(temppath, filename); //create hard link should work....
	} catch (boost::filesystem::filesystem_error err) {
		auto c(err.code().value());
		if (c == boost::system::errc::too_many_links || c == 1142) {
			try {
				boost::filesystem::copy(temppath, filename); //fallback option to copy
			} catch (...) {
				return false;
			}
		} else {
			return false;
		}
	}
	return true;
}


boost::filesystem::path find_unique_fname(const std::string& extension) 
{
	boost::filesystem::path newfilename;
	std::string fname("%%%%");
	do {
		fname += "-%%%%";
		newfilename = boost::filesystem::unique_path(working_dir / (fname + extension));
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
	} catch (boost::filesystem::filesystem_error err) {
		auto c(err.code().value());
		if (c == boost::system::errc::too_many_links || c == 1142) {
			try {
				boost::filesystem::copy(filename, newfilename); //fallback option to copy
				added_files.push_back(newfilename);
			} catch (...) {
				return "";
			}
		} else {
			return "";
		}
	}
	return newfilename.string();
}

std::string _copy_png(const std::string& filename) 
{
	return _copy_keep_extension(filename, ".png");
}
std::string _copy_jpeg(const std::string& filename)
{
	std::string newfilename;
	try {
		boost::gil::rgb8_image_t im;
		boost::gil::jpeg_read_image(filename, im);
		newfilename =find_unique_fname().string();
		boost::gil::png_write_view(newfilename, boost::gil::view(im));
		added_files.push_back(newfilename);
	} catch (...) {
		return "";
	}
	return newfilename;
}
std::string _copy_tiff(const std::string& filename)
{
	//boost::filesystem::path newfilename;
	//try {
	//	boost::gil::rgb8_image_t im;
	//	boost::gil::tiff_read_image(filename, im);
	//	auto v(boost::gil::view(im));
	//	newfilename =find_unique_fname();
	//	boost::gil::png_write_view(newfilename.string(), v);
	//	added_files.push_back(newfilename);
	//} catch (...) {
	//	return "";
	//}
	//return newfilename.string();
	return "";
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

