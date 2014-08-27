#include <memory>
#include <string>
#include <fstream>
#include <ios>
#include <deque>

#include "file_binary_handling.h"
#include "standard_functions.h"


std::deque<pfbstream_t>& OpenFileBinstreams() {
	static std::deque<pfbstream_t>* result = new std::deque<pfbstream_t>;
	return *result;
}
std::deque<int>& OpenBinSpots() {
	static std::deque<int>* result = new std::deque<int>;
	return *result;
}

int InsertFileStreamToBinVector(pfbstream_t&& file) 
{
	int ind;
	if (OpenBinSpots().empty()) {
			ind = OpenFileBinstreams().size();
			OpenFileBinstreams().push_back(std::forward<pfbstream_t>(file));
	} else {
			ind = OpenBinSpots().back();
			OpenBinSpots().pop_back();
			OpenFileBinstreams()[ind] = std::move(file);
	}
	return ind;

}
inline bool isValidBinIndex(int index) 
{
	return index >= 0 && OpenFileBinstreams().size() > (unsigned int)index && OpenFileBinstreams()[index] != nullptr;
}


int _file_bin_open(boost::filesystem::path filename, std::ios_base::open_mode mode) 
{
	os_string_type fname_string;
#ifdef _WIN32
	fname_string = string_convert<os_string_type> (filename.wstring());
#else
	fname_string = string_convert<os_string_type> (filename.string());
#endif
	return _file_bin_open(fname_string, mode);
}
int _file_bin_open(const os_string_type& filename, std::ios_base::open_mode mode) 
{
	int newindex(-1);
	pfbstream_t file(new fbstream_t(filename, std::ios_base::binary | mode));
	if (!file->fail()) {
		file->unsetf(std::ios_base::skipws);
		newindex = InsertFileStreamToBinVector(std::move(file));
	}
	return newindex;
}

unsigned char _file_bin_read_byte(int file)
{
	char ret(0);
	if (isValidBinIndex(file) && OpenFileBinstreams()[file]->good()) {
		ret = OpenFileBinstreams()[file]->get();
		return reinterpret_cast<unsigned char&>(ret);
	}
	return 0;
}
void _file_bin_write_byte(int file, unsigned char byte) {
	char b(reinterpret_cast<char&>(byte));
	OpenFileBinstreams()[file]->put(b);
}
unsigned short _file_bin_read_word(int file)
{
	unsigned short ret;
	if (isValidBinIndex(file) && OpenFileBinstreams()[file]->good()) {
		OpenFileBinstreams()[file]->read(reinterpret_cast<char *>(&ret), sizeof(ret));
		return ret;
	}
	return 0;
}
void _file_bin_write_word(int file,unsigned short input) {
	if (isValidBinIndex(file) && OpenFileBinstreams()[file]->good()) {
		OpenFileBinstreams()[file]->write(reinterpret_cast<const char *>(&input), sizeof(input));
	}
}
unsigned long _file_bin_read_dword(int file)
{
	unsigned long  ret;
	if (isValidBinIndex(file) && OpenFileBinstreams()[file]->good()) {
		OpenFileBinstreams()[file]->read(reinterpret_cast<char *>(&ret), sizeof(ret));
		return ret;
	}
	return 0;
}
void _file_bin_write_dword(int file, unsigned long input) {
	if (isValidBinIndex(file) && OpenFileBinstreams()[file]->good()) {
		OpenFileBinstreams()[file]->write(reinterpret_cast<const char *>(&input), sizeof(input));
	}
}

auto _file_bin_size(int file) -> decltype(OpenFileBinstreams()[file]->tellg())
{
	if (isValidBinIndex(file)) {
		auto old(OpenFileBinstreams()[file]->tellg());
		OpenFileBinstreams()[file]->seekg(0, std::ifstream::end);
		auto ret(OpenFileBinstreams()[file]->tellg());
		OpenFileBinstreams()[file]->seekg(old);
		return ret;
	}
	return 0;
}
auto _file_bin_position(int file) -> decltype(OpenFileBinstreams()[file]->tellg())
{
	if (isValidBinIndex(file)) {
		return OpenFileBinstreams()[file]->tellp();
	}
	return 0;
}
void _file_bin_seek(int file, int offset, std::ios_base::seekdir dir)
{
	if (isValidBinIndex(file)) {
		OpenFileBinstreams()[file]->seekp(offset, dir);
	}
}
bool _file_bin_eof(int file)
{
	if (isValidBinIndex(file)) {
		return OpenFileBinstreams()[file]->eof();
	}
	return false;
}
bool _file_bin_good(int file)
{
	if (isValidBinIndex(file)) {
		return OpenFileBinstreams()[file]->good();
	}
	return false;
}
bool _file_bin_fail(int file)
{
	if (isValidBinIndex(file)) {
		return OpenFileBinstreams()[file]->fail();
	}
	return false;
}
bool _file_bin_bad(int file)
{
	if (isValidBinIndex(file)) {
		return OpenFileBinstreams()[file]->bad();
	}
	return false;
}
void _file_bin_clear_fail(int file)
{
	if (isValidBinIndex(file)) {
		OpenFileBinstreams()[file]->clear(OpenFileBinstreams()[file]->rdstate() & ~std::ios::failbit);
	}
}
void _file_bin_clear_bad(int file)
{
	if (isValidBinIndex(file)) {
		OpenFileBinstreams()[file]->clear(OpenFileBinstreams()[file]->rdstate() & ~std::ios::badbit);
	}
}
void _file_bin_write_flush(int file) 
{
	if (isValidBinIndex(file) && OpenFileBinstreams()[file]->good()) {
		OpenFileBinstreams()[file]->flush();
	}
}
void _file_bin_close(int file)
{
	if (isValidBinIndex(file)) {
		OpenFileBinstreams()[file].reset(nullptr);
		OpenBinSpots().push_back(file);
	}
}

GMEXPORT double file_bin_open(const char* filename, double mode)
{
	auto os_filename(string_convert<os_string_type>(filename));
	std::ios_base::open_mode openmode;
	switch (static_cast<int>(mode)) {
	case 0:
		openmode = std::ios_base::in;
		break;
	case 1:
		openmode = std::ios_base::out;
		break;
	case 2:
		openmode = std::ios_base::in | std::ios_base::out;
		break;
	}
	return _file_bin_open(MakeRichPath(os_filename),openmode);
}

GMEXPORT double file_bin_read_byte(double file)
{
	return _file_bin_read_byte(static_cast<int>(file));
}
GMEXPORT double  file_bin_write_byte(double file, double byte)
{
	_file_bin_write_byte(static_cast<int>(file),static_cast<char>(byte));
	return 0;
}
GMEXPORT double file_bin_read_word(double file)
{
	return _file_bin_read_word(static_cast<int>(file));
}
GMEXPORT double  file_bin_write_word(double file, double byte)
{
	_file_bin_write_word(static_cast<int>(file),static_cast<unsigned short>(byte));
	return 0;
}
GMEXPORT double file_bin_read_dword(double file)
{
	return _file_bin_read_dword(static_cast<int>(file));
}
GMEXPORT double file_bin_write_dword(double file, double byte)
{
	_file_bin_write_dword(static_cast<int>(file),static_cast<unsigned long>(byte));
	return 0;
}

GMEXPORT double file_bin_size(double file)
{
	return static_cast<double>(_file_bin_size(static_cast<int>(file)));
}
GMEXPORT double file_bin_position(double file)
{
	return static_cast<double>(_file_bin_position(static_cast<int>(file)));
}
GMEXPORT double file_bin_seek(double file, double position)
{
	_file_bin_seek(static_cast<int>(file), static_cast<int>(position));
	return 0;
}
GMEXPORT double file_bin_seek_relative(double file, double offset,double relative)
{
	std::ios_base::seekdir dir;
	switch (static_cast<int>(relative)) {
	case 0:
		dir = std::ios_base::beg;
		break;
	case 1:
		dir = std::ios_base::end;
		break;
	case 2:
		dir = std::ios_base::cur;
		break;
	}
	_file_bin_seek(static_cast<int>(file), static_cast<int>(offset));
	return 0;
}
GMEXPORT double file_bin_eof(double file)
{
	return _file_bin_eof(static_cast<int>(file));
}
GMEXPORT double file_bin_good(double file)
{
	return _file_bin_good(static_cast<int>(file));
}
GMEXPORT double file_bin_fail(double file)
{
	return _file_bin_fail(static_cast<int>(file));
}
GMEXPORT double file_bin_bad(double file)
{
	return _file_bin_bad(static_cast<int>(file));
}
GMEXPORT double file_bin_clear_fail(double file)
{
	_file_bin_clear_fail(static_cast<int>(file));
	return 0;
}
GMEXPORT double file_bin_clear_bad(double file)
{
	_file_bin_clear_bad(static_cast<int>(file));
	return 0;
}
GMEXPORT double file_bin_write_flush(double file)
{
	_file_bin_write_flush(static_cast<int>(file));
	return 0.0;
}
GMEXPORT double file_bin_close(double file)
{
	_file_bin_close(static_cast<int>(file));
	return 0.0;
}
