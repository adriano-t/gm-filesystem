
#include <string>
#include <memory>
#include <fstream>

#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#ifdef _DEBUG
#include <Windows.h>
#endif

#include "inireader.h"
#include "../GMFile/standard_functions.h"

static std::string RetString;
static std::unique_ptr<ini_data> ini_file = nullptr;
std::deque<std::unique_ptr<ini_data> >& ini_data::OpenIniFiles() {
	static std::deque<std::unique_ptr<ini_data> > result;
	return result;
}
std::deque<int>& ini_data::OpenSpots() {
	static std::deque<int> result;
	return result;
}

bool ini_data::IsValidIndex(int index) 
{
	return index >= 0 && OpenIniFiles().size() > (unsigned int)index && OpenIniFiles()[index] != nullptr;
}



inline const char* ReturnString(const std::string& val)
{
	RetString = val;
	return RetString.c_str();
}
int ini_data::InsertIniDataToVector(std::unique_ptr<ini_data>&& ini)
{
	int ind;
	if (OpenSpots().empty()) {
			ind = OpenIniFiles().size();
			OpenIniFiles().push_back(std::forward< decltype(ini) >(ini));
	} else {
			ind = OpenSpots().back();
			OpenSpots().pop_back();
			OpenIniFiles()[ind] = std::move(ini);
	}
	return ind;

}
void ini_data::DeleteIniData(int ind)
{
	if (IsValidIndex(ind)) { //safety measure
		OpenIniFiles()[ind].reset(nullptr);
		OpenSpots().push_back(ind);
	}
}

ini_data::ini_data(const os_string_type& fname)
	: filename(fname),
	constructed(false),
	data(new ini_tree())
{
	load_ini();
}
ini_data::~ini_data()
{
	if (constructed) {
		save_ini();
	}
}


void ini_data::load_ini()
{
	{
		std::basic_fstream<os_char_type> fs(filename);
		if (!fs.is_open()) {
			fs.open(filename, std::ios_base::out);
		}
		fs.flush();
		fs.close();
	}
	//std::basic_ifstream<os_char_type> fs(filename);
	try {
		std::basic_ifstream<char> fs(filename);
		boost::property_tree::read_ini(fs, *data);
		constructed = true;
	} catch (boost::property_tree::ini_parser_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBox(0, string_convert<os_string_type>(tstr).c_str(), _T("ini loading fail"),MB_ICONERROR);
#endif
	}
	
}
void ini_data::save_ini()
{
	std::basic_ofstream<char> fs(filename);
	boost::property_tree::write_ini(fs, *data);
}

ini_data::ini_tree::path_type ini_data::GetPath(const key_type& section, const key_type& key)
{
	ini_tree::path_type p(section, L'=');
	p /= ini_tree::path_type(key, L'=');
	return p;
}



void DisplayAllNodes(const ini_data::ini_tree& t, std::string prefix = "")
{
	//decltype(*t.begin()) v;

	std::for_each(t.begin(), t.end(), [&] (decltype(*t.begin()) v) {
		std::cout << prefix << string_convert<std::string>(v.first) <<std::endl;
		if (v.second.size() > 0 ) {
			DisplayAllNodes(v.second, prefix + "---");
		}
	});
}



ini_data::value_type ini_data::read_string(const key_type& section, const key_type& key, const value_type& def) const
{
	ini_tree::path_type p(GetPath(section, key));
	return data->get(p, def);
}
double ini_data::read_real(const key_type& section, const key_type& key, double def) const 
{

	try {
		value_type d(boost::lexical_cast<value_type>(def));
		value_type str(data->get(GetPath(section,key), d));
		return boost::lexical_cast<double>(str);
	} catch (boost::bad_lexical_cast& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBox(0, string_convert<os_string_type>(tstr).c_str(), _T("data conversion fail"),MB_ICONERROR);
#endif
	}
	return 0;
}
void ini_data::write_string(const key_type& section, const key_type& key, const value_type& value)
{
	try {
		data->put(GetPath(section, key), value);
	} catch (boost::property_tree::ptree_bad_data& err) {
		
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBox(0, string_convert<os_string_type>(tstr).c_str(), _T("ini string writing fail"),MB_ICONERROR);
#endif
	}
}
void ini_data::write_real(const key_type& section, const key_type& key, double value)
{
	
	try {
		
		data->put(GetPath(section, key), boost::lexical_cast<value_type>(value));
	} catch (boost::property_tree::ptree_bad_data& err) {
		
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBox(0, string_convert<os_string_type>(tstr).c_str(), _T("ini real writing fail"),MB_ICONERROR);
#endif
	} catch (boost::bad_lexical_cast& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBox(0, string_convert<os_string_type>(tstr).c_str(), _T("data conversion fail"),MB_ICONERROR);
#endif
	}
}
bool ini_data::key_exists(const key_type& section, const key_type& key) const 
{
	auto child(data->get_child_optional(GetPath(section, key)));
	return !!child;
}
void ini_data::key_delete(const key_type& section, const key_type& key)
{
	if (key_exists(section, key)) {
		auto& s(data->get_child(section));
		s.erase(key);
	}
}
bool ini_data::section_exists(const key_type& section) const 
{
	auto child(data->get_child_optional(section));
	return !!child;
}
void ini_data::section_delete(const key_type& section)
{
	data->erase(section);
}




const char* _ini_read_string(const ini_data& ini, const char* section, const char* key, const char* def)
{
	auto dat =  ini.read_string(string_convert<ini_data::key_type>(section),string_convert<ini_data::key_type>(key), string_convert<ini_data::value_type>(def));
	return ReturnString(string_convert<std::string>(dat));
}
double _ini_read_real(const ini_data& ini, const char* section, const char* key, double def)
{
	return ini.read_real(string_convert<ini_data::key_type>(section), string_convert<ini_data::key_type>(key), def);
}
double _ini_write_string(ini_data& ini, const char* section, const char* key, const char* value)
{
	ini.write_string(string_convert<ini_data::key_type>(section), string_convert<ini_data::key_type>(key), string_convert<ini_data::value_type>(value));
	return 0;
}
double _ini_write_real(ini_data& ini, const char* section, const char* key, double value)
{
	ini.write_real(string_convert<ini_data::key_type>(section), string_convert<ini_data::key_type>(key), value);
	return 0;
}
double _ini_key_exists(const ini_data& ini, const char* section, const char* key)
{
	return ini.key_exists(string_convert<ini_data::key_type>(section), string_convert<ini_data::key_type>(key));
}
double _ini_key_delete(ini_data& ini, const char* section, const char* key)
{
	ini.key_delete(string_convert<ini_data::key_type>(section), string_convert<ini_data::key_type>(key));
	return 0;
}
double _ini_section_exists(const ini_data& ini, const char* section)
{
	return ini.section_exists(string_convert<ini_data::key_type>(section));
}
double _ini_section_delete(ini_data& ini, const char* section)
{
	ini.section_delete(string_convert<ini_data::key_type>(section));
	return 0;
}



GMEXPORT double ini_open(const char* filename)
{
	auto path(MakeRichPath(string_convert<os_string_type>(filename)));
	ini_file.reset(new ini_data(path));
	return 0;
}
GMEXPORT double ini_close()
{
	ini_file.reset(nullptr);
	return 0;
}
GMEXPORT const char* ini_read_string(const char* section, const char* key, const char* def)
{
	if (ini_file)
		return _ini_read_string(*ini_file, section, key, def);
	return "";
}
GMEXPORT double ini_read_real(const char* section, const char* key, double def)
{
	if (ini_file)
		return _ini_read_real(*ini_file, section, key, def);
	return 0;
}
GMEXPORT double ini_write_string(const char* section, const char* key, const char* value)
{
	if (ini_file)
		return _ini_write_string(*ini_file, section, key, value);
	return 0;
}
GMEXPORT double ini_write_real(const char* section, const char* key, double value)
{
	if (ini_file)
		return _ini_write_real(*ini_file, section, key, value);
	return 0;
}
GMEXPORT double ini_key_exists(const char* section, const char* key)
{
	if (ini_file)
		return _ini_key_exists(*ini_file, section, key);
	return 0;
}
GMEXPORT double ini_key_delete(const char* section, const char* key)
{
	if (ini_file)
		return _ini_key_delete(*ini_file, section, key);
	return 0;
}
GMEXPORT double ini_section_exists(const char* section)
{
	if (ini_file)
		return _ini_section_exists(*ini_file, section);
	return 0;
}
GMEXPORT double ini_section_delete(const char* section)
{
	if (ini_file)
		return _ini_section_delete(*ini_file, section);
	return 0;
}



GMEXPORT double ini_open_ext(const char* filename)
{
	auto path(MakeRichPath(string_convert<os_string_type>(filename)));
	return ini_data::InsertIniDataToVector(std::unique_ptr<ini_data>(new ini_data(path)));
}

GMEXPORT double ini_close_ext(double ini)
{
	ini_data::DeleteIniData(static_cast<int>(ini));
	return 0;
}
GMEXPORT const char* ini_read_string_ext(double ini,const char* section, const char* key, const char* def)
{
	int ind = static_cast<int>(ini);
	if (ini_data::IsValidIndex(ind)) {
		return _ini_read_string(*ini_data::OpenIniFiles()[ind], section, key, def);
	}
	return ReturnString(def);
}
GMEXPORT double ini_read_real_ext(double ini, const char* section, const char* key, double def)
{
	int ind = static_cast<int>(ini);
	if (ini_data::IsValidIndex(ind)) {
		return _ini_read_real(*ini_data::OpenIniFiles()[ind], section, key, def);
	}
	return def;
}
GMEXPORT double ini_write_string_ext(double ini, const char* section, const char* key, const char* value)
{
	int ind = static_cast<int>(ini);
	if (ini_data::IsValidIndex(ind)) {
		_ini_write_string(*ini_data::OpenIniFiles()[ind], section, key, value);
	}
	return 0;
}
GMEXPORT double ini_write_real_ext(double ini, const char* section, const char* key, double value)
{
	int ind = static_cast<int>(ini);
	if (ini_data::IsValidIndex(ind)) {
		_ini_write_real(*ini_data::OpenIniFiles()[ind], section, key, value);
	}
	return 0;
}
GMEXPORT double ini_key_exists_ext(double ini, const char* section, const char* key)
{
	int ind = static_cast<int>(ini);
	if (ini_data::IsValidIndex(ind)) {
		return _ini_key_exists(*ini_data::OpenIniFiles()[ind], section, key);
	}
	return 0;
}
GMEXPORT double ini_key_delete_ext(double ini, const char* section, const char* key)
{
	int ind = static_cast<int>(ini);
	if (ini_data::IsValidIndex(ind)) {
		_ini_key_delete(*ini_data::OpenIniFiles()[ind], section, key);
	}
	return 0;
}
GMEXPORT double ini_section_exists_ext(double ini, const char* section)
{
	int ind = static_cast<int>(ini);
	if (ini_data::IsValidIndex(ind)) {
		return _ini_section_exists(*ini_data::OpenIniFiles()[ind], section);
	}
	return 0;
}
GMEXPORT double ini_section_delete_ext(double ini, const char* section)
{
	int ind = static_cast<int>(ini);
	if (ini_data::IsValidIndex(ind)) {
		_ini_section_delete(*ini_data::OpenIniFiles()[ind], section);
	}
	return 0;
}