#include <stdexcept>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

#include <tinyxml2.h>

#ifdef _DEBUG
#include <Windows.h>
#endif

#include "xmlinterface.h"
#include "xmldata.h"
#include "../GMFile/standard_functions.h"


static std::string RetString;
inline const char* ReturnString(const std::string& val)
{
	RetString = val;
	return RetString.c_str();
}

double pointer_to_double(void* p) 
{
	double ret;
	unsigned long long tlong(reinterpret_cast<unsigned long long>(p));
	memcpy(&ret, &tlong, sizeof(tlong));
	return ret;
}
double node_to_double(xml_data::pNode p)
{
	return pointer_to_double(p);
}
double elem_to_double(xml_data::pElem p)
{
	return pointer_to_double(p);
}
double attr_to_double(xml_data::pAttr p)
{
	return pointer_to_double(p);
}
void* double_to_pointer(double v)
{
	unsigned long long tlong;
	memcpy(&tlong, &v, sizeof(v));
	return reinterpret_cast<void* >(tlong);
}
xml_data::pNode double_to_node(double v)
{
	if (v == 0) {
		throw xml_error("Bad node pointer");
	}
	void* p(double_to_pointer(v));
	return static_cast<xml_data::pNode>(p);
}
xml_data::pElem double_to_elem(double v)
{
	if (v == 0) {
		throw xml_error("Bad element pointer");
	}
	void* p(double_to_pointer(v));
	return static_cast<xml_data::pElem>(p);
}
xml_data::pAttr double_to_attr(double v)
{
	if (v == 0) {
		throw xml_error("Bad attribute pointer");
	}
	void* p(double_to_pointer(v));
	return static_cast<xml_data::pAttr>(p);
}
xml_data* double_to_xml(double xml)
{
	int p(static_cast<int>(xml));
	if (xml_data::isValidXMLObject(p))
		return xml_data::OpenXMLFiles()[p].get();

	throw xml_error("Incorrect XML id");
	return nullptr;
}

xml_data::pNode _xml_find_elem_under(xml_data* xml, xml_data::pNode node, 
											token_iterator begin, 
											const token_iterator& end)
{
	for (;begin != end && node != nullptr; ++begin) {
		node = xml->firstNamedChildElem(node, *begin);
	}
	return node;
}

xml_data::ChildType to_childtype(int v) {
	switch (v) {
	case xml_data::CT_COMMENT:
		return xml_data::CT_COMMENT;
	case xml_data::CT_DECLARATION:
		return xml_data::CT_DECLARATION;
	case xml_data::CT_DOCUMENT:
		return xml_data::CT_DOCUMENT;
	case xml_data::CT_ELEMENT:
		return xml_data::CT_ELEMENT;
	case xml_data::CT_TEXT:
		return xml_data::CT_TEXT;
	default:
		return xml_data::CT_UNKNOWN;
	}
}
xml_data::ChildType to_childtype(double v) {
	return to_childtype(static_cast<int>(v));
}

double xml_same_node(double node_left, double node_right) 
{

	auto pLeftNode(double_to_node(node_left));
	auto pRightNode(double_to_node(node_right));

	
#if _DEBUG
	std::string n1(  pLeftNode->Value());
	std::string n2( pRightNode->Value());
	::MessageBoxA(0,(n1 + " -- " + n2).c_str(), "bad node id",MB_ICONERROR);
#endif
	return  static_cast<double>(pLeftNode == pRightNode);
}

 double xml_open(const char* filename)
{
	std::string path(MakeRichPath(filename));
	return xml_data::InsertXMLDataToVector(std::unique_ptr<xml_data>(new xml_data(path)));
}
 double xml_open_ext(const char* filename, double whitespace)
{
	std::string path(MakeRichPath(filename));
	int white_int = static_cast<int>(whitespace);
	tinyxml2::Whitespace white;
	switch (white_int) {
	case tinyxml2::COLLAPSE_WHITESPACE:
		white = tinyxml2::COLLAPSE_WHITESPACE;
		break;
	default:
		white = tinyxml2::PRESERVE_WHITESPACE;
		break;
	};
	return xml_data::InsertXMLDataToVector(std::unique_ptr<xml_data>(new xml_data(path.c_str(), white )));
}
 double xml_close(double xml)
{
	try {
		xml_data::DeleteXMLData(static_cast<int>(xml));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	}
	return 0;
}
 double xml_get_node_type(double xml, double node)
{
	try {
		return static_cast<double>(double_to_xml(xml)->getNodeType(double_to_node(node)));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return -1;
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return -1;
	}
}
 double xml_node_make_element(double xml, double node)
{
	try {
		double_to_xml(xml)->getNodeType(double_to_node(node));
		return 0;
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return -1;
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return -1;
	}
}
 double xml_num_elem(double xml, double parent_node)
{
	try {
		return static_cast<double>(double_to_xml(xml)->NumChildElems(double_to_node(parent_node)));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return -1;
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return -1;
	}
}
 double xml_num_node(double xml, double parent_node)
{
	try {
		return static_cast<double>(double_to_xml(xml)->NumChildNodes(double_to_node(parent_node)));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return -1;
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return -1;
	}
}
	//raw node access
 double xml_node_first(double xml, double parent_node)
{
	try {
		return node_to_double(double_to_xml(xml)->firstChild(double_to_node(parent_node)));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return 0;
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return 0;
	}
}
 double xml_node_last(double xml, double parent_node)
{
	try {
		return node_to_double(double_to_xml(xml)->lastChild(double_to_node(parent_node)));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return 0;
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return 0;
	}
}
 double xml_node_next(double xml, double node)
{
	try {
		return node_to_double(double_to_xml(xml)->nextChild(double_to_node(node)));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return 0;
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return 0;
	}
}
 double xml_node_previous(double xml, double node)
{
	try {
		return node_to_double(double_to_xml(xml)->prevChild(double_to_node(node)));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return 0;
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return 0;
	}
}
 double xml_root_element(double xml )
{
	try {
		return elem_to_double(double_to_xml(xml)->topNode());
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return 0;
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return 0;
	}
}
 double xml_elem_first(double xml, double parent_node)
{
	try {
		return elem_to_double(double_to_xml(xml)->firstChildElem(double_to_node(parent_node)));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return 0;
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return 0;
	}
}
 double xml_elem_last(double xml, double parent_node)
{
	try {
		return elem_to_double(double_to_xml(xml)->lastChildElem(double_to_node(parent_node)));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return 0;
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return 0;
	}
}
 double xml_elem_next(double xml, double elem)
{
	try {
		return elem_to_double(double_to_xml(xml)->nextChildElem(double_to_elem(elem)));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return 0;
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return 0;
	}
}
 double xml_elem_previous(double xml, double elem)
{
	try {
		return elem_to_double(double_to_xml(xml)->prevChildElem(double_to_elem(elem)));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return 0;
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return 0;
	}
}
 double xml_named_elem_first(double xml, double parent_node, const char* name)
{
	try {
		return elem_to_double(double_to_xml(xml)->firstNamedChildElem(double_to_node(parent_node), name));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return 0;
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return 0;
	}
}

double xml_find_elem(double xml, const char* path)
{
	try {
		auto pxml(double_to_xml(xml));
		boost::escaped_list_separator<char> sep('\\', '.', '\"');
		std::string p(path);
		tokenizer tok(p, sep);
		auto t(_xml_find_elem_under(pxml, pxml->getDoc(), tok.begin(), tok.end()));
		if (t) {
			return elem_to_double(t->ToElement());
		}
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return 0;
}
double xml_find_elem_under(double xml, double elem, const char* path)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pelem(double_to_elem(elem));
		boost::escaped_list_separator<char> sep('\\', '.', '\"');
		std::string p(path);
		tokenizer tok(p, sep);
		auto t(_xml_find_elem_under(pxml, pelem, tok.begin(), tok.end()));
		return elem_to_double(t->ToElement());
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return 0;
 }

 double xml_named_elem_last(double xml, double parent_node, const char* name)
{
	try {
		return elem_to_double(double_to_xml(xml)->lastNamedChildElem(double_to_node(parent_node), name));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return 0;
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return 0;
	}
}
 double xml_named_elem_next(double xml, double elem)
{
	try {
		return elem_to_double(double_to_xml(xml)->nextNamedChildElem(double_to_elem(elem)));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return 0;
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return 0;
	}
}
 double xml_named_elem_previous(double xml, double elem)
{
	try {
		return elem_to_double(double_to_xml(xml)->prevNamedChildElem(double_to_elem(elem)));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return 0;
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return 0;
	}
}

 double xml_parent_elem(double xml, double node)
{
	try {
		return elem_to_double(double_to_xml(xml)->getParentElem(double_to_node(node)));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return 0;
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return 0;
	}
}


//cpElem
 const char* xml_get_elem_name(double xml, double elem)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pelem(double_to_elem(elem));
		return ReturnString(pxml->NodeName(pelem));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return "";
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return "";
	}
}
 const char* xml_get_elem_data(double xml, double elem)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pelem(double_to_elem(elem));
		return ReturnString(pxml->NodeData(pelem));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return "";
}
 const char* xml_get_node_raw_data(double xml, double node)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pnode(double_to_node(node));
		return ReturnString(pxml->NodeRawValue(pnode));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
		return "";
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
		return "";
	}
}
 double xml_set_elem_name(double xml, double elem, const char* name)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pelem(double_to_elem(elem));
		pxml->setName(pelem, name);
		return 0;
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return -1;
}
 double xml_set_elem_value(double xml, double elem, const char* value)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pelem(double_to_elem(elem));
		pxml->setValue(pelem, value);
		return 0;
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return -1;
}
 double xml_set_node_raw_data(double xml, double elem, const char* data)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pnode(double_to_node(elem));
		pxml->setRawValue(pnode, data);
		return 0;
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return -1;
}
 double xml_insert_end_elem(double xml, double parent_elem, const char* name, const char* value)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pelem(double_to_elem(parent_elem));
		return elem_to_double(pxml->pushbackChildElem(pelem, name, value));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return 0;
}
 double xml_insert_begin_elem(double xml, double parent_elem, const char* name, const char* value)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pelem(double_to_elem(parent_elem));
		return elem_to_double(pxml->pushfrontChildElem(pelem, name, value));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return 0;
}
 double xml_insert_elem(double xml, double parent_elem, double after_node, const char* name, const char* value)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pelem(double_to_elem(parent_elem));
		auto panode(double_to_node(after_node));
		return elem_to_double(pxml->pushChildElem(pelem, panode, name, value));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return 0;
}
 double xml_delete_node(double xml, double parent_node, double node)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pparnode(double_to_node(parent_node));
		auto pnode(double_to_node(node));
		pxml->deleteNode(pparnode, pnode);
		return 0;
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return -1;
}
 double xml_clear(double xml, double parent_node)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pparnode(double_to_node(parent_node));
		pxml->deleteNodes(pparnode);
		return 0;
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return -1;
}
 double xml_set_attribute(double xml, double elem, const char* name, const char* value)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pelem(double_to_elem(elem));
		pxml->setAttribute(pelem, name, value);
		return 0;
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return -1;
}
 double xml_delete_attribute(double xml, double elem, const char* name)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pelem(double_to_elem(elem));
		pxml->deleteAttribute(pelem, name);
		return 0;
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return -1;
}
//rawnode
 double xml_insert_end_node(double xml, double parent_node, double type, const char* name)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pnode(double_to_node(parent_node));
		return node_to_double(pxml->pushbackChild(pnode, to_childtype(type), name));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return 0;
}
 double xml_insert_begin_node(double xml, double parent_node, double type, const char* name)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pnode(double_to_node(parent_node));
		return node_to_double(pxml->pushfrontChild(pnode, to_childtype(type), name));
		return 0;
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return 0;
}
 double xml_insert_node(double xml, double parent_node, double after_node, double type, const char* name)
	
{
	try {
		auto pxml(double_to_xml(xml));
		auto pnode(double_to_node(parent_node));
		auto panode(double_to_node(after_node));
		return node_to_double(pxml->pushChild(pnode, panode, to_childtype(type), name));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return 0;
}


	//attribute access
 double xml_num_attributes(double xml, double elem)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pelem(double_to_elem(elem));
		return pxml->NumAttributes(pelem);
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return -1;
}
 const char* xml_get_attribute(double xml, double elem, const char* name)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pelem(double_to_elem(elem));
		return ReturnString(pxml->GetAttribute(pelem, name));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return "";
}
 double xml_attribute_first(double xml, double elem)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pelem(double_to_elem(elem));
		return attr_to_double(pxml->firstAttribute(pelem));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return 0;
}
 double xml_attribute_last(double xml, double elem)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pelem(double_to_elem(elem));
		return attr_to_double(pxml->lastAttribute(pelem));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return 0;
}
 double xml_attribute_next(double xml, double attribute)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pattr(double_to_attr(attribute));
		return attr_to_double(pxml->nextAttribute(pattr));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return 0;
}

	//attribute
 const char* xml_attribute_get_name(double xml, double attribute)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pattr(double_to_attr(attribute));
		return ReturnString(pxml->AttributeName(pattr));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return "";
}
 const char* xml_attribute_get_value(double xml, double attribute)
{
	try {
		auto pxml(double_to_xml(xml));
		auto pattr(double_to_attr(attribute));
		return ReturnString(pxml->AttributeValue(pattr));
	} catch (xml_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "xml error",MB_ICONERROR);
#endif
	} catch (std::out_of_range& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "bad node id",MB_ICONERROR);
#endif
	}
	return "";
}