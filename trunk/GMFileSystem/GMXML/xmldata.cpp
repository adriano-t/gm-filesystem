#include "xmldata.h"

xml_data::xml_data(const char* fname, tinyxml2::Whitespace white) 
	: filename(""),
	doc(true, white)
{
	auto err(doc.LoadFile(fname));
	filename = fname;
}
xml_data::xml_data(const std::string& fname, tinyxml2::Whitespace white ) 
	: filename(""),
	doc(true, white)
{
	doc.LoadFile(fname.c_str());
	filename = fname;
}
xml_data::~xml_data()
{
	if (filename != "") {
		doc.SaveFile(filename.c_str());
	}
}


xml_data::cpNode xml_data::firstChild(cpNode parent_node)  const
{
	return parent_node->FirstChild();
}
xml_data::cpNode xml_data::lastChild(cpNode parent_node) const
{
	return parent_node->LastChild();
}
xml_data::cpNode xml_data::nextChild(cpNode node) const
{
	return node->NextSibling();
}
xml_data::cpNode xml_data::prevChild(cpNode node) const
{
	return node->PreviousSibling();
}

bool xml_data::NodeIsType(cpNode node, ChildType type) const
{
	switch (type){
	case CT_UNKNOWN:
		return node->ToUnknown() != nullptr;
		break;
	case CT_TEXT:
		return node->ToText() != nullptr;
		break;
	case CT_ELEMENT:
		return node->ToElement() != nullptr;
		break;
	case CT_DOCUMENT:
		return node->ToDocument() != nullptr;
		break;
	case CT_DECLARATION:
		return node->ToDeclaration() != nullptr;
		break;
	case CT_COMMENT:
		return node->ToComment() != nullptr;
		break;
	default:
		return false;
	}

}
xml_data::ChildType xml_data::getNodeType(cpNode node) const
{
	if (node->ToElement() != nullptr) return CT_ELEMENT;
	if (node->ToText() != nullptr) return CT_TEXT;
	if (node->ToComment() != nullptr) return CT_COMMENT;
	if (node->ToDocument() != nullptr) return CT_DECLARATION;
	if (node->ToDocument() != nullptr) return CT_DOCUMENT;
	return CT_UNKNOWN;
}
inline xml_data::cpElem xml_data::ConvertToElement(cpNode node) const
{
	return node->ToElement();
}
inline xml_data::pElem xml_data::ConvertToElement(pNode node)
{
	return const_cast<pElem>( static_cast<const xml_data &>( *this ).ConvertToElement(node) );
}

int xml_data::NumChildElems(cpNode parent_node) const
{
	int count(0);
	for( auto ChildElem = parent_node->FirstChildElement(); ChildElem; ChildElem = ChildElem->NextSiblingElement() ) {
		++count;
	}
	return count;
}
int xml_data::NumChildNodes(cpNode parent_node) const
{
	int count(0);
	for( auto Child = parent_node->FirstChild(); Child; Child = Child->NextSibling() ) {
		++count;
	}
	return count;
}
//element access
xml_data::cpElem xml_data::topNode() const
{
	auto t(doc.FirstChild());
	return doc.FirstChildElement();
}
xml_data::cpElem xml_data::firstChildElem(cpNode parent_node)  const
{
	return parent_node->FirstChildElement();
}
xml_data::cpElem xml_data::lastChildElem(cpNode parent_node) const
{
	return parent_node->LastChildElement();
}
xml_data::cpElem xml_data::nextChildElem(cpElem node) const
{
	return node->NextSiblingElement();
}
xml_data::cpElem xml_data::prevChildElem(cpElem node) const
{
	return node->PreviousSiblingElement();
}
xml_data::cpElem xml_data::firstNamedChildElem(cpNode parent_node, const std::string& name) const
{
	return parent_node->FirstChildElement(name.c_str());
}
xml_data::cpElem xml_data::lastNamedChildElem(cpNode parent_node, const std::string& name) const
{
	return parent_node->LastChildElement(name.c_str());
}
xml_data::cpElem xml_data::nextNamedChildElem(cpElem node) const
{
	return node->NextSiblingElement(node->Name());
}
xml_data::cpElem xml_data::prevNamedChildElem(cpElem node) const
{
	return node->PreviousSiblingElement(node->Name());
}
xml_data::cpElem xml_data::getParentElem(cpNode node) const
{
	return node->Parent()->ToElement();
}




xml_data::pNode xml_data::firstChild(cpNode parent_node)
{
	return const_cast<pNode>( static_cast<const xml_data &>( *this ).firstChild(parent_node) );
}
xml_data::pNode xml_data::lastChild(cpNode parent_node)
{
	return const_cast<pNode>( static_cast<const xml_data &>( *this ).lastChild(parent_node) );
}
xml_data::pNode xml_data::nextChild(cpNode node)
{
	return const_cast<pNode>( static_cast<const xml_data &>( *this ).nextChild(node) );
}
xml_data::pNode xml_data::prevChild(cpNode node)
{
	return const_cast<pNode>( static_cast<const xml_data &>( *this ).prevChild(node) );
}


xml_data::pElem xml_data::topNode()
{
	return const_cast<pElem>( static_cast<const xml_data &>( *this ).topNode() );
}
xml_data::pElem xml_data::firstChildElem(cpNode parent_node)
{
	return const_cast<pElem>( static_cast<const xml_data &>( *this ).firstChildElem(parent_node) );
}
xml_data::pElem xml_data::lastChildElem(cpNode parent_node)
{
	return const_cast<pElem>( static_cast<const xml_data &>( *this ).lastChildElem(parent_node) );
}
xml_data::pElem xml_data::nextChildElem(cpElem node)
{
	return const_cast<pElem>( static_cast<const xml_data &>( *this ).nextChildElem(node) );
}
xml_data::pElem xml_data::prevChildElem(cpElem node)
{
	return const_cast<pElem>( static_cast<const xml_data &>( *this ).prevChildElem(node) );
}
xml_data::pElem xml_data::firstNamedChildElem(cpNode parent_node, const std::string& name)
{
	return const_cast<pElem>( static_cast<const xml_data &>( *this ).firstNamedChildElem(parent_node, name) );
}
xml_data::pElem xml_data::lastNamedChildElem(cpNode parent_node, const std::string& name)
{
	return const_cast<pElem>( static_cast<const xml_data &>( *this ).lastNamedChildElem(parent_node, name) );
}
xml_data::pElem xml_data::nextNamedChildElem(cpElem node)
{
	return const_cast<pElem>( static_cast<const xml_data &>( *this ).nextNamedChildElem(node) );
}
xml_data::pElem xml_data::prevNamedChildElem(cpElem node)
{
	return const_cast<pElem>( static_cast<const xml_data &>( *this ).prevNamedChildElem(node) );
}
xml_data::pElem xml_data::getParentElem(cpNode node)
{
	return const_cast<pElem>( static_cast<const xml_data &>( *this ).getParentElem(node) );
}


std::string xml_data::NodeName(cpElem node) const
{
	auto p = node->Name();
	return p ? p : "";
}
std::string xml_data::NodeData(cpElem node) const
{
	auto p = node->GetText();
	return p ? p : "";
}
std::string xml_data::NodeRawValue(cpNode node) const
{
	auto p = node->Value();
	return p ? p : "";
}
void xml_data::setName(pElem node, const std::string& name)
{
	node->SetName(name.c_str());
}
void xml_data::setValue(pElem node, const std::string& value)
{
	//get first child
	auto child = node->FirstChild();
	if (child->ToText()) {
		child->SetValue(value.c_str());
	} else {
		node->InsertFirstChild(doc.NewText(value.c_str()));
	}
}
void xml_data::setRawValue(pNode node, const std::string& value)
{
	node->SetValue(value.c_str());
}



xml_data::pElem xml_data::pushbackChildElem(pElem node, const std::string& name, const std::string& value)
{
	pElem newelem = CreateNewElem(name, value);
	node->InsertEndChild(newelem);
	return newelem;
}
xml_data::pElem xml_data::pushfrontChildElem(pElem node, const std::string& name, const std::string& value)
{
	pElem newelem = CreateNewElem(name, value);
	node->InsertFirstChild(newelem);
	return newelem;
}
xml_data::pElem xml_data::pushChildElem(pElem node, pNode after, const std::string& name, const std::string& value)
{
	pElem newelem = CreateNewElem(name, value);
	node->InsertAfterChild(after, newelem);
	return newelem;
}
void xml_data::deleteNode(pNode parent_node, pNode node)
{
	parent_node->DeleteChild(node);
}
void xml_data::deleteNodes(pNode parent_node)
{
	parent_node->DeleteChildren();
}
void xml_data::setAttribute(pElem node, const std::string& name, const std::string& value)
{
	node->SetAttribute(name.c_str(), value.c_str());
}
void xml_data::deleteAttribute(pElem node, const std::string& name)
{
	node->DeleteAttribute(name.c_str());
}
//rawnode
xml_data::pNode xml_data::pushbackChild(pNode node, ChildType type, const std::string& name)
{
	pNode n = createNewNode(type, name);
	node->InsertEndChild(n);
	return n;
}
xml_data::pNode xml_data::pushfrontChild(pNode node, ChildType type, const std::string& name)
{
	pNode n = createNewNode(type, name);
	node->InsertFirstChild(n);
	return n;
}
xml_data::pNode xml_data::pushChild(pNode node, pNode after, ChildType type, const std::string& name)
{
	pNode n = createNewNode(type, name);
	node->InsertAfterChild(after, n);
	return n;
}


//attributes
int xml_data::NumAttributes(cpElem node) const
{
	int count(0);
	for( auto attribute = node->FirstAttribute(); attribute; attribute = attribute->Next() ) {
		++count;
	}
	return count;
}
std::string xml_data::GetAttribute(cpElem node, const std::string& name) const
{
	auto p = node->Attribute(name.c_str());
	return p ? p : "";
}
xml_data::cpAttr xml_data::firstAttribute(cpElem parent_node) const
{
	return parent_node->FirstAttribute();
}
xml_data::cpAttr xml_data::lastAttribute(cpElem parent_node) const
{
	auto prev(parent_node->FirstAttribute());
	for( auto attribute = prev; attribute; attribute = attribute->Next() ) {
		prev = attribute;
	}
	return prev;
}
xml_data::cpAttr xml_data::nextAttribute(cpAttr attribute) const
{
	return attribute->Next();
}
xml_data::pAttr xml_data::firstAttribute(cpElem parent_node)
{
	return const_cast<pAttr>( static_cast<const xml_data &>( *this ).firstAttribute(parent_node) );
}
xml_data::pAttr xml_data::lastAttribute(cpElem parent_node)
{
	return const_cast<pAttr>( static_cast<const xml_data &>( *this ).lastAttribute(parent_node) );
}
xml_data::pAttr xml_data::nextAttribute(cpAttr attribute)
{
	return const_cast<pAttr>( static_cast<const xml_data &>( *this ).nextAttribute(attribute) );
}

//attribute
std::string xml_data::AttributeName(cpAttr attribute) const
{
	return attribute->Name();
}
std::string xml_data::AttributeValue(cpAttr attribute) const
{
	return attribute->Value();
}

xml_data::pNode xml_data::createNewNode(ChildType type, const std::string& name)
{
	pNode n;
	switch (type){
	case CT_TEXT:
		n = doc.NewText(name.c_str());
		break;
	case CT_ELEMENT:
		n = doc.NewElement(name.c_str());
		break;
	case CT_DECLARATION:
		n = doc.NewDeclaration(name.c_str());
		break;
	case CT_COMMENT:
		n = doc.NewComment(name.c_str());
		break;
	default:
		n = doc.NewUnknown(name.c_str());
		break;
	}
	n->SetValue(name.c_str());
	return n;
}
xml_data::pElem xml_data::CreateNewElem(const std::string& name, const std::string& value)
{
	pElem newelem = doc.NewElement(name.c_str());
	newelem->InsertFirstChild(doc.NewText(value.c_str()));
	return newelem;
}
inline std::deque<std::unique_ptr<xml_data> >& xml_data::OpenXMLFiles()
{
	static std::deque<std::unique_ptr<xml_data> > result;
	return result;
}
inline std::deque<int>& xml_data::OpenSpots()
{
	static std::deque<int> result;
	return result;
}
int xml_data::InsertXMLDataToVector(std::unique_ptr<xml_data>&& xml)
{
	int ind;
	if (OpenSpots().empty()) {
		ind = OpenXMLFiles().size();
		OpenXMLFiles().push_back(std::forward< decltype(xml) >(xml));
	} else {
		ind = OpenSpots().back();
		OpenSpots().pop_back();
		OpenXMLFiles()[ind] = std::move(xml);
	}
	return ind;
}
void xml_data::DeleteXMLData(int index)
{
	if (isValidXMLObject(index)) { //safety measure
		OpenXMLFiles()[index].reset(nullptr);
		OpenSpots().push_back(index);
	}
}
inline bool xml_data::isValidXMLObject(int index)
{
	return index >= 0 && OpenXMLFiles().size() > (unsigned int)index && OpenXMLFiles()[index] != nullptr;
}