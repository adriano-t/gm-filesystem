#pragma once
#include <string>
#include <ios>
#include <boost/filesystem.hpp>

#include "file_handling.h"

int _file_bin_open(const os_string_type& filename, std::ios_base::open_mode mode);
int _file_bin_open(boost::filesystem::path filename, std::ios_base::open_mode mode);

unsigned char _file_bin_read_byte(int file);
void _file_bin_write_byte(int file, unsigned char byte);
unsigned short _file_bin_read_word(int file);
void _file_bin_write_word(int file, unsigned short input);
unsigned long _file_bin_read_dword(int file);
void _file_bin_write_dword(int file, unsigned long input);

auto _file_bin_size(int file) -> decltype(OpenFilestreams()[file]->tellg());
auto _file_bin_position(int file) -> decltype(OpenFilestreams()[file]->tellg());
void _file_bin_seek(int file, int offset, std::ios_base::seekdir dir = std::ios_base::beg);


GMEXPORT double file_bin_open(const char* filename, double mode);

GMEXPORT double file_bin_read_byte(double file);
GMEXPORT double  file_bin_write_byte(double file, double byte);
GMEXPORT double file_bin_read_word(double file);
GMEXPORT double  file_bin_write_word(double file, double byte);
GMEXPORT double file_bin_read_dword(double file);
GMEXPORT double  file_bin_write_dword(double file, double byte);


GMEXPORT double file_bin_size(double file);
GMEXPORT double file_bin_position(double file);
GMEXPORT double file_bin_seek(double file, double position);
GMEXPORT double file_bin_seek_relative(double file, double offset, double relative);
