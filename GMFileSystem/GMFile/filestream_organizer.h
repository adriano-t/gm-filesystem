#pragma once
#include <fstream>
#include <locale>
#include <codecvt>
#include <memory>

#include "standard_functions.h"

template <typename charT, typename traitsT = std::char_traits<charT> >
class wcopystream : public std::basic_iostream<charT, traitsT> {
public:
	typedef wcopystream<charT, traitsT> _myt;
	typedef std::basic_iostream<charT, traitsT> _mybase;
	typedef std::basic_streambuf<charT, traitsT> _myfb;

	wcopystream(_myfb* sb) : _mybase(sb),
		internal_buffer(sb)
	{
	}

private:
	std::unique_ptr<_myfb > internal_buffer;
};


class filestream_organizer 
{
public:
	typedef char32_t mychar_t;
	typedef std::u32string mystring_t;
	typedef void (*endianfixer_t)(mychar_t&);
	typedef wcopystream<mychar_t> widestream_t;
	typedef std::basic_fstream<char> narrowstream_t;
	typedef std::basic_filebuf<char> narrowbuf_t;
	typedef std::basic_streambuf<mychar_t> widebuf_t;
	typedef std::wstring_convert<std::codecvt_utf8<mychar_t>, mychar_t> conv_t;

	enum encoding {
		ENC_UTF8,
		ENC_UTF16_LE,
		ENC_UTF16_BE,
		ENC_UTF32_LE,
		ENC_UTF32_BE
	};

	filestream_organizer(const os_string_type& fname, std::ios::openmode mode, encoding e = ENC_UTF8, bool forced = false);
	filestream_organizer(const os_string_type& fname, std::ios::openmode mode, std::string endl, encoding e = ENC_UTF8, bool forced = false);

	//filestream_organizer(filestream_organizer&&);                 // move const
	//filestream_organizer& operator=(filestream_organizer&&);      // move assign

	encoding get_encoding();

	std::string get();
	void unget();
	std::string peek();
	std::string read(std::streamsize count);
	std::string getline();
	void readln();

	void write_BOM();
	void write_string(const std::string& input);
	void writeln();

	bool eof() const;
	bool eoln();
	bool fail() const;
	bool bad() const;
	bool good() const;
	void set_fail();
	void set_bad();
	void clear_fail();
	void clear_bad();

	std::streampos tell();
	void seek(std::streampos p);

	void flush();

	const static mystring_t& DEFAULT_LINE_ENDING();

protected:
	static std::string to_utf8(const mystring_t& in);
	static mystring_t from_utf8(const std::string& in);

	void write_BOM_UTF8();
	void write_BOM_UTF16_LE();
	void write_BOM_UTF16_BE();
	void write_BOM_UTF32_LE();
	void write_BOM_UTF32_BE();


	encoding interpret_header();
	widebuf_t* __initialize(bool forced = false);

	mychar_t _get();
	mychar_t _peek();


	template <typename T>
	static void swap_endian(T& pX);
	template <typename T>
	static void no_swap_endian(T& pX);

	
	const static char LF8;
	const static char16_t LF16_BE;
	const static char32_t LF32_BE;
	const static char CR8;
	const static char16_t CR16_BE;
	const static char32_t CR32_BE;

	const static char16_t LF16_LE;
	const static char32_t LF32_LE;
	const static char16_t CR16_LE;
	const static char32_t CR32_LE;
	

	bool opened;
	std::ios::openmode open_mode;
	encoding enc;
	narrowstream_t narrow_filestream;
	endianfixer_t endianfix;
	std::basic_istream<mychar_t>::pos_type end_header_pos;
	widestream_t wide_filestream;
	mystring_t line_ending;

	filestream_organizer(const filestream_organizer&);                 // Prevent copy-construction
	filestream_organizer& operator=(const filestream_organizer&);

};

template <typename T>
void filestream_organizer::no_swap_endian(T& pX)
{
}
template <typename T>
void filestream_organizer::swap_endian(T& pX)
{
    char& raw = reinterpret_cast<char&>(pX);
    std::reverse(&raw, &raw + sizeof(T));
}
