#include <fstream>
#include <locale>
#include <codecvt>
#include <boost/regex.hpp>
#include <regex>

#include "filestream_organizer.h"


std::u32string getDefEndl() {
	std::u32string ret;
#ifdef _WIN32
	ret+= 0xD;
#endif
	ret += 0xA;
	return ret;
}


const char filestream_organizer::LF8 = 0xA;
const char16_t filestream_organizer::LF16_BE = 0xA;
const char32_t filestream_organizer::LF32_BE = 0xA;
const char filestream_organizer::CR8 = 0xD;
const char16_t filestream_organizer::CR16_BE = 0xD;
const char32_t filestream_organizer::CR32_BE = 0xD;

const char16_t filestream_organizer::LF16_LE = 0x0A00;
const char32_t filestream_organizer::LF32_LE = 0x0A000000;
const char16_t filestream_organizer::CR16_LE = 0x0D00;
const char32_t filestream_organizer::CR32_LE = 0x0D000000;

const filestream_organizer::mystring_t& filestream_organizer::DEFAULT_LINE_ENDING() {
	static mystring_t* endl = new mystring_t(getDefEndl());
	return *endl;
}




filestream_organizer::filestream_organizer(const os_string_type& fname, std::ios::openmode mode, encoding e, bool forced)
	: narrow_filestream(fname, std::ios::binary | mode),
	open_mode(std::ios::binary | mode),
	end_header_pos(0),
	endianfix(&no_swap_endian<mychar_t>),
	enc(e),
	wide_filestream(__initialize(forced)),
	opened(true),
	line_ending(DEFAULT_LINE_ENDING())
{
	if (enc == ENC_UTF32_BE) {
		endianfix = &swap_endian<mychar_t>;
	}
}
filestream_organizer::filestream_organizer(const os_string_type& fname, std::ios::openmode mode, std::string endl, encoding e, bool forced)
	: narrow_filestream(fname, std::ios::binary | mode),
	open_mode(std::ios::binary | mode),
	end_header_pos(0),
	endianfix(&no_swap_endian<mychar_t>),
	enc(e),
	wide_filestream(__initialize(forced)),
	opened(true),
	line_ending(from_utf8(endl))
{
	if (enc == ENC_UTF32_BE) {
		endianfix = &swap_endian<mychar_t>;
	}
}


inline unsigned int __consume_header_chars(filestream_organizer::narrowstream_t& ins)
{
	
	std::basic_istream<char>::sentry se(ins, true);
	auto sb = ins.rdbuf();
	unsigned int ret = 0;
	for (int i(0); i < 4; ++i) {
		auto c = sb->sbumpc();
		if (c == EOF) {
			//ins.setstate(std::ios::eofbit);
			return ret;
		} else {
			ret += c << i*8;
		}
	}

	return ret;
}
filestream_organizer::encoding filestream_organizer::interpret_header()
{
	end_header_pos = narrow_filestream.tellg();
	auto c = __consume_header_chars(narrow_filestream);
	auto e(this->enc);
	if ( (c & 0x00FFFFFF) == 0x00BFBBEF) {
		end_header_pos += 3;
		e = ENC_UTF8;
	} else if (c == 0xFFFE0000) {
		end_header_pos += 4;
		e = ENC_UTF32_BE;
	} else if (c == 0x0000FEFF) {
		end_header_pos += 4;
		
		e = ENC_UTF32_LE;
	}  else if ((c & 0x0000FFFF) == 0x0000FFFE){
		end_header_pos += 2;
		e = ENC_UTF16_BE;
	} else if ((c & 0x0000FFFF) == 0x0000FEFF){
		end_header_pos += 2;
		e = ENC_UTF16_LE;
	}
	narrow_filestream.seekg(end_header_pos);
	return e;
}

std::string filestream_organizer::get() 
{
	mychar_t c = _get();
	if (c == EOF) {
		return std::string();
	}
	return to_utf8(mystring_t(1, c));
}
void filestream_organizer::unget() 
{
	wide_filestream.unget();
}
filestream_organizer::mychar_t filestream_organizer::_get() {
	mychar_t c = wide_filestream.get();
	endianfix(c);
	if (c == CR32_BE) {
		mychar_t cc(wide_filestream.peek());
		endianfix(cc);
		if (cc == LF32_BE) {
			wide_filestream.get();
			c = LF32_BE;
		}
	}
	return c;
}
std::string filestream_organizer::peek()
{
	mychar_t c = _peek();
	if (c == EOF) {
		return std::string();
	}
	return to_utf8(mystring_t(1, c));
}
filestream_organizer::mychar_t filestream_organizer::_peek()
{
	mychar_t c = wide_filestream.peek();
	endianfix(c);
	if (c == CR32_BE) {
		wide_filestream.get();
		mychar_t cc(wide_filestream.peek());
		endianfix(cc);
		if (cc == LF32_BE) {
			c = LF32_BE;
		}
		wide_filestream.unget();
		wide_filestream.clear(wide_filestream.rdstate() & !std::ios::eofbit);
	}
	return c;
}
std::string filestream_organizer::getline() 
{
	mystring_t t;
    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::basic_istream<mychar_t>::sentry se(wide_filestream, true);
    auto sb = wide_filestream.rdbuf();

    for(;;) {
        mychar_t c = sb->sbumpc();
		endianfix(c);
        switch (c) {
        case LF32_BE:
			sb->sungetc();
            return to_utf8(t);
        case CR32_BE:
			{
			mychar_t cc(sb->sgetc());
			endianfix(cc);
            if(cc != LF32_BE)
                sb->sungetc();
            return to_utf8(t);
			}
		case EOF:
            // Also handle the case when the last line has no line ending
            wide_filestream.setstate(std::ios::eofbit);
            return to_utf8(t);
        default:
            t += c;
        }
    }
	return to_utf8(t);
}
std::string filestream_organizer::read(std::streamsize count)
{
	mystring_t t;

    std::basic_istream<mychar_t>::sentry se(wide_filestream, true);
    auto sb = wide_filestream.rdbuf();

    for(;count > 0; --count) {
        mychar_t c = sb->sbumpc();
		endianfix(c);
		if (c == CR32_BE) {
			mychar_t cc(sb->sgetc());
			endianfix(cc);
            if(cc == LF32_BE) {
                sb->sbumpc();
				c = LF32_BE;
			}
		} else if (c == EOF) {
            // Also handle the case when the last line has no line ending
            wide_filestream.setstate(std::ios::eofbit);
            return to_utf8(t);
        }
		t += c;
    }
	return to_utf8(t);
}

void filestream_organizer::readln()
{
    std::basic_istream<mychar_t>::sentry se(wide_filestream, true);
    auto sb = wide_filestream.rdbuf();
    for(;;) {
        mychar_t c = sb->sbumpc();
		endianfix(c);
		if (c == LF32_BE || EOF) {
			return;
		} else if (c == CR32_BE) {
			mychar_t cc(sb->sgetc());
			endianfix(cc);
            if(cc == LF32_BE)
                sb->sbumpc();
			return;
		}
    }
}

void filestream_organizer::write_BOM() 
{
	switch (enc) {
	case ENC_UTF8:
		write_BOM_UTF8();
		break;
	case ENC_UTF16_LE:
		write_BOM_UTF16_LE();
		break;
	case ENC_UTF16_BE:
		write_BOM_UTF16_BE();
		break;
	case ENC_UTF32_LE:
		write_BOM_UTF32_LE();
		break;
	case ENC_UTF32_BE:
		write_BOM_UTF32_BE();
		break;
	}
}
void filestream_organizer::write_BOM_UTF8() 
{
	char* bom = "\xEF\xBB\xBF";
	narrow_filestream.write(bom, 3);
}
void filestream_organizer::write_BOM_UTF16_LE() 
{
	char* bom = "\xFF\xFE";
	narrow_filestream.write(bom, 2);
}
void filestream_organizer::write_BOM_UTF16_BE() 
{
	char* bom = "\xFE\xFF";
	narrow_filestream.write(bom, 2);
}
void filestream_organizer::write_BOM_UTF32_LE() 
{
	char* bom = "\xFF\xFE\x0\x0";
	narrow_filestream.write(bom, 4);
}
void filestream_organizer::write_BOM_UTF32_BE() 
{
	char* bom = "\x0\x0\xFE\xFF";
	narrow_filestream.write(bom, 4);
}
void filestream_organizer::write_string(const std::string& input) 
{
	mystring_t in(from_utf8(input));
	auto s = line_ending.size();


	

	for (auto iter(in.begin()); iter != in.end() && wide_filestream.good(); ++iter) {
		auto c(*iter);
		endianfix(c);
	}

	std::basic_istream<mychar_t>::sentry se(wide_filestream, true);
    auto sb = wide_filestream.rdbuf();

	for (auto iter(in.begin()); iter != in.end() && wide_filestream.good(); ) {
		auto c(*iter);
		++iter;
		bool handled (false);
		if (iter != in.end())  {
			auto n(*iter);
			if (c == LF32_BE || (c == CR32_BE && n == LF32_BE) ) {
				std::for_each(line_ending.begin(), line_ending.end(), [&] (mychar_t cc) {
					if (sb->sputc(cc) == std::char_traits<mychar_t>::eof()) {
						this->wide_filestream.setstate(std::ios::badbit);
					}
				});
				handled = true;
				if (c == CR32_BE) {
					++iter;
				}
			} 
		}

		if (!handled) {
			if (sb->sputc(c) == std::char_traits<mychar_t>::eof()) {
				wide_filestream.setstate(std::ios::badbit);
			}
		}
	}
}
void filestream_organizer::writeln() 
{
	wide_filestream << line_ending;
}


bool filestream_organizer::eof() const
{
	return wide_filestream.eof() || narrow_filestream.eof();
}
bool filestream_organizer::eoln()
{
	return _peek() == LF32_BE;
}
bool filestream_organizer::fail() const
{
	return wide_filestream.fail() || narrow_filestream.fail();
}
bool filestream_organizer::bad() const
{
	return wide_filestream.bad() || narrow_filestream.bad();
}
bool filestream_organizer::good() const
{
	return wide_filestream.good() && narrow_filestream.good();
}
void filestream_organizer::set_fail()
{
	narrow_filestream.setstate(std::ios::failbit);
	wide_filestream.setstate(std::ios::failbit);
}
void filestream_organizer::set_bad()
{
	narrow_filestream.setstate(std::ios::badbit);
	wide_filestream.setstate(std::ios::badbit);
}
void filestream_organizer::clear_fail()
{
	auto f1 = narrow_filestream.rdstate() & ~std::ios::failbit;
	auto f2 = wide_filestream.rdstate() & ~std::ios::failbit;
	narrow_filestream.clear(f1);
	wide_filestream.clear(f2);
}
void filestream_organizer::clear_bad()
{
	auto f1 = narrow_filestream.rdstate() & ~std::ios::badbit;
	auto f2 = wide_filestream.rdstate() & ~std::ios::badbit;
	narrow_filestream.clear(f1);
	wide_filestream.clear(f2);
}


std::string filestream_organizer::to_utf8(const filestream_organizer::mystring_t& in)
{
	static conv_t cvt_32_8;
	return cvt_32_8.to_bytes(in);
}
filestream_organizer::mystring_t filestream_organizer::from_utf8(const std::string& in) {
	static conv_t cvt_32_8;
	return cvt_32_8.from_bytes(in);
}


filestream_organizer::widebuf_t* filestream_organizer::__initialize(bool forced)
{
	if (!(open_mode  & (std::ios::out | std::ios::trunc))) {
		encoding e(interpret_header());
		if (!forced) {
			enc = e;
		}
	}
	switch (enc) {
	case ENC_UTF8:
		{
		return new std::wbuffer_convert<std::codecvt_utf8<mychar_t >, mychar_t>(narrow_filestream.rdbuf());
		break;
		}
	case ENC_UTF16_BE:
		{
		return new std::wbuffer_convert<std::codecvt_utf16<mychar_t>, mychar_t>(narrow_filestream.rdbuf());
		break;
		}
	case ENC_UTF16_LE:
		{
		return new std::wbuffer_convert<std::codecvt_utf16<mychar_t,
							   0x10ffff,
							   std::little_endian
						 >, mychar_t>(narrow_filestream.rdbuf());
		break;
		}
	default:
		{
		//auto cvt1(narrow_filestream.rdbuf());
		return new std::wbuffer_convert<std::codecvt<mychar_t, char, std::mbstate_t>, mychar_t>(narrow_filestream.rdbuf());
		break;
		}
	}
}


std::streampos filestream_organizer::tell()
{
	return narrow_filestream.tellg();
}
void filestream_organizer::seek(std::streampos p)
{
	narrow_filestream.seekg(p);
}


void filestream_organizer::flush()
{
	wide_filestream.flush();
	narrow_filestream.flush();
}