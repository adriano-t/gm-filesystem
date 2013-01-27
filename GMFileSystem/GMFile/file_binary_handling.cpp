#include <memory>
#include <string>
#include <fstream>
#include <ios>
#include <deque>

#include "file_binary_handling.h"
#include "file_handling.h"
#include "standard_functions.h"

int _file_bin_open(boost::filesystem::path filename, std::ios_base::open_mode mode) 
{
	return _file_bin_open(filename.string(), mode);
}
int _file_bin_open(std::string filename, std::ios_base::open_mode mode) 
{
	int newindex(-1);
	std::unique_ptr<std::fstream> file(new std::fstream(filename, std::ios_base::binary | mode));
	if (!file->fail()) {
		newindex = InsertFileStreamToVector(std::move(file));
	}
	return newindex;
}

char _file_bin_read_byte(int file)
{
	char ret(0);
	if (isValidIndex(file) && OpenFilestreams()[file]->good()) {
		OpenFilestreams()[file]->get(ret);
		return ret;
	}
	return 0;
}
void _file_bin_write_byte(int file, char byte) {
	OpenFilestreams()[file]->put(byte);
}
unsigned short _file_bin_read_word(int file)
{
	unsigned short ret;
	if (isValidIndex(file) && OpenFilestreams()[file]->good()) {
		OpenFilestreams()[file]->read(reinterpret_cast<char *>(&ret), sizeof(ret));
		return ret;
	}
	return 0;
}
void _file_bin_write_word(int file,unsigned short input) {
	OpenFilestreams()[file]->write(reinterpret_cast<const char *>(&input), sizeof(input));
}
unsigned long _file_bin_read_dword(int file)
{
	unsigned long  ret;
	if (isValidIndex(file) && OpenFilestreams()[file]->good()) {
		OpenFilestreams()[file]->read(reinterpret_cast<char *>(&ret), sizeof(ret));
		return ret;
	}
	return 0;
}
void _file_bin_write_dword(int file, unsigned long input) {
	OpenFilestreams()[file]->write(reinterpret_cast<const char *>(&input), sizeof(input));
}

auto _file_bin_size(int file) -> decltype(OpenFilestreams()[file]->tellg())
{
	auto old(OpenFilestreams()[file]->tellg());
	OpenFilestreams()[file]->seekg(0, std::ifstream::end);
	auto ret(OpenFilestreams()[file]->tellg());
	OpenFilestreams()[file]->seekg(old);
	return ret;
}
//auto _file_bin_read_position(int file) -> decltype(OpenFilestreams()[file]->tellg())
//{
//	return OpenFilestreams()[file]->tellg();
//}
auto _file_bin_position(int file) -> decltype(OpenFilestreams()[file]->tellg())
{
	return OpenFilestreams()[file]->tellp();
}
void _file_bin_seek(int file, int offset, std::ios_base::seekdir dir)
{
	OpenFilestreams()[file]->seekp(offset, dir);
}

GMEXPORT double file_bin_open(const char* filename, double mode)
{
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
	return _file_bin_open(MakeRichPath(filename),openmode);
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
//GMEXPORT double file_bin_read_position(double file)
//{
//	return _file_bin_read_position(static_cast<int>(file));
//}
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
