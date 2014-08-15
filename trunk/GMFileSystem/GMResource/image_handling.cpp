
#include <string>
#include <array>
#include <deque>
#include <vector>
#include <algorithm>
#include <exception>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

#ifdef _DEBUG
#include <Windows.h>
#include <boost\lexical_cast.hpp>
#endif

#include "../GMFile/standard_functions.h"
#include "image_handling.h"
#include "allresources.h"


static std::string RetString;
///ONLY THANKS TO A MISSING FEATURE IN GM THIS HAS TO BE DONE

const static std::array<unsigned char, 8> png_header = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
const static std::array<unsigned char, 2> jpeg_header = {0xFF, 0xD8};
const static std::array<unsigned char, 4> tiffII_header = {0x49, 0x49, 0x2A, 0x00};
const static std::array<unsigned char, 4> tiffMM_header = {0x4D, 0x4D, 0x00, 0x2A};

inline const char* ReturnString(const std::string& val)
{
	RetString = val;
	return RetString.c_str();
}
class bad_img_exception : public std::runtime_error
{
public:
	bad_img_exception()
		: runtime_error("Incorrectly formatted image")
	{
	}
	virtual const char* what() const
	{
		return std::runtime_error::what();
	}
};


std::vector<unsigned char> file_to_bytemap (const boost::filesystem::path& filename) {
	std::vector<unsigned char> bytemap;
	std::basic_ifstream<unsigned char> ifs(path_to_string(filename), std::ios::binary);
	bytemap.reserve(boost::filesystem::file_size(filename));
	bytemap.assign(std::istreambuf_iterator<unsigned char>( ifs ), std::istreambuf_iterator<unsigned char>());
	return bytemap;
}
void bytemap_to_file (const std::vector<unsigned char>& ivec, const boost::filesystem::path& filename) {
	std::basic_ofstream<unsigned char> ofs(path_to_string(filename), std::ios::binary);
	ofs.write(&ivec[0], ivec.size());
}


void _do_copy_opencv(const boost::filesystem::path& importpath, const boost::filesystem::path& exportpath, 
						const std::string& extension, const std::vector<int>& params) 
{
	if (boost::filesystem::exists(importpath)) {
		std::vector<unsigned char> ibytemap = file_to_bytemap(importpath);
		cv::Mat img(cv::imdecode(cv::Mat(ibytemap), -1));
		if (!img.data) {
			throw bad_img_exception(); //invalid image format
		}
		std::vector<unsigned char> obytemap;
		cv::imencode(extension, img, obytemap, params);
		bytemap_to_file(obytemap, exportpath);
	}
	
}



bool export_image_opencv(const boost::filesystem::path& tempfile, const boost::filesystem::path& filename, const std::vector<int>& params)
{
	added_files.push_back(tempfile);
	try {
		boost::filesystem::path ext( filename.extension());
		_do_copy_opencv(tempfile, filename, ext.string(), params);
	} catch (cv::Exception& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBox(0, string_convert<os_string_type>(tstr).c_str(), _T("OpenCV fail"),MB_ICONERROR);
#endif
		return false;
	} catch (::bad_img_exception& ) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += "could not load data from file ";
		tstr += filename.string();
		::MessageBox(0, string_convert<os_string_type>(tstr).c_str(), _T("OpenCV import fail"),MB_ICONERROR);
#endif
		return false;
	}
	return true;
}


boost::filesystem::path _copy_using_opencv(const boost::filesystem::path& filename)
{
	boost::filesystem::path newfilename(find_unique_fname(_T(".png")));
	try {
		_do_copy_opencv(filename, newfilename);
		added_files.push_back(newfilename);
	} catch (cv::Exception& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBox(0, string_convert<os_string_type>(tstr).c_str(), _T("OpenCV fail"),MB_ICONERROR);
#endif
		return boost::filesystem::path();
	} catch (::bad_img_exception& ) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += "could not load data from file ";
		tstr += filename.string();
		::MessageBox(0, string_convert<os_string_type>(tstr).c_str(), _T("OpenCV import fail"),MB_ICONERROR);
#endif
		return boost::filesystem::path();
	}
	return newfilename;
}


bool file_has_png_header(const boost::filesystem::path& filename) {
	std::ifstream filestream(path_to_string(filename), std::ios_base::binary);
	filestream >> std::noskipws;
	if (filestream.bad()) {
		#ifdef _DEBUG
			std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
			tstr += "could open file ";
			tstr += filename.string();
			::MessageBox(0, string_convert<os_string_type>(tstr).c_str(), _T("Generic File Fail"),MB_ICONERROR);
		#endif
		return false;
	}
	std::array<unsigned char, 8> header;
	std::copy_n(std::istream_iterator<unsigned char>(filestream), 8, header.begin());

	return std::equal(header.begin(), header.end(), png_header.begin());
}

GMEXPORT const char* import_image(const char* filename) 
{
	boost::filesystem::path importpath(MakeRichPath(string_convert<os_string_type>(filename)));
	boost::filesystem::path exportpath;
	if (file_has_png_header(importpath)) {
		//png
		exportpath = _copy_keep_extension(importpath, _T(".png"));
	} else {
		exportpath = _copy_using_opencv(importpath);
	}
	return ReturnString(exportpath.string());
}


GMEXPORT double export_image(const char* tempfile, const char* filename)
{
	os_string_type importfile(MakeRichPath(string_convert<os_string_type>(tempfile)));
	os_string_type exportfile(MakeRichPath(string_convert<os_string_type>(filename)));
	os_string_type ext( exportfile.substr(exportfile.rfind('.')));
	if (ext == _T(".png")) {
		return export_direct(importfile, exportfile)
	} else {
		return export_image_opencv(importfile, exportfile);
	}
}
GMEXPORT double export_image_adv(const char* tempfile, const char* filename, double param)
{
	os_string_type importfile(MakeRichPath(string_convert<os_string_type>(tempfile)));
	os_string_type exportfile(MakeRichPath(string_convert<os_string_type>(filename)));
	os_string_type ext( exportfile.substr(exportfile.rfind('.')));
	//boost::filesystem::path temppath =  save_dir / tempfile;

	//fast if possible
	if (param == -1 && ext == _T(".png")) {
		return export_direct(importfile, exportfile);
	}

	std::vector<int> compression_parameters;
	int param_type(-1);
	if (ext == _T(".png")) {
		param_type = CV_IMWRITE_PNG_COMPRESSION;	
	} else if (ext == _T(".jpg") || ext == _T(".jpeg") || ext == _T(".jpe")) {
		param_type = CV_IMWRITE_JPEG_QUALITY;
	} else if (ext == _T(".pgm") || ext == _T(".pbm") || ext == _T(".ppm")) {
		param_type = CV_IMWRITE_PXM_BINARY;
	}
	if (param != -1) {
		compression_parameters.push_back(param_type);
		compression_parameters.push_back(static_cast<int>(param));
	}
	return export_image_opencv(importfile, exportfile, compression_parameters);


}




