// TinyXMLClientApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "tinyxml2.h"
#include <string>
#include <iostream>
#include <sstream>


using namespace tinyxml2;
using namespace std;


// Forward declarations
bool OpenFile(string& fn, XMLDocument& doc, ostringstream& errmsg);
void DisplayDocToScreen(string& fn,XMLDocument& doc);
void ExploreDoc(XMLDocument& doc);
void ExploreFromElem(string& srchToStr, XMLElement* elem);
bool HasFilteredElement(string& AttribName, string& AttribValue, XMLElement* current);
void ShowFilteredElements(XMLElement* elem, string& nextToken, string& attribName, string& attribValue);
bool GetAttributes(string& srchToStr, size_t pos, string& nextToken, string& attribName, string& attribValue);

XMLElement* FindElement(string& ElemName, XMLDocument& doc);
XMLElement* FindElement(string& ElemName, XMLElement* curElem);
XMLElement* FindFilteredElement(string& ElemName, string& AttribName, string& AttribValue,XMLElement* curElem);

int _tmain(int argc, _TCHAR* argv[])
{
	XMLDocument doc;	// Create an instance to load and parse an XML document

	// This file is in the project folder which is the default debug location
	// We may have to be more specific about this file later if we distribute the program
	
	std::string fn = "Sample.xml";
	ostringstream errmsg;
	
	bool IsDocOpen = OpenFile(fn, doc, errmsg);
	cout << errmsg.str() << endl << endl;
	if (!IsDocOpen)
		return -1;

	DisplayDocToScreen(fn, doc);
	ExploreDoc(doc);

	cout << endl << "Using XPath type logic to Comedies" << endl;
	std::string srchToStr = "Movies/Movie/Title[@Type='Comedy']";
	XMLElement* elem = doc.RootElement();
	ExploreFromElem(srchToStr, elem);

	cout << "End of sample TinyXML application" << endl;

	system("PAUSE");
};


void ExploreFromElem(string& srchToStr, XMLElement* elem)
{

	size_t pos = srchToStr.find_first_of('/');
	if (pos > srchToStr.length())
	{
		XMLElement* x = FindElement(srchToStr, elem);
		pos = srchToStr.find_first_of('[');
		if (pos < srchToStr.length())
		{
			// Has Attributes
			string nextToken, attribName, attribValue;
			nextToken = attribName = attribValue = "";
			if (GetAttributes(srchToStr, pos, nextToken, attribName, attribValue))
			{
				elem = FindElement(nextToken, elem);
				ShowFilteredElements(elem, nextToken, attribName, attribValue);
			}
		}
		else
		{
			elem = FindElement(srchToStr, elem);
			cout << elem->Name() << " is " << elem->GetText() << endl;
		}
		

		
		return;
	}
	else
	{
		string remStr = srchToStr.substr(pos+1);
		string nextToken = srchToStr.substr(0, pos);
		elem = FindElement(nextToken, elem);
		//	For Debugging: cout << nextToken << endl;
		ExploreFromElem(remStr, elem);

	}

}

bool GetAttributes(string& srchToStr, size_t pos, string& nextToken, string& attribName, string& attribValue)
{
	string attribClause = srchToStr.substr(pos);
	// 2 is the first parameter because the syntax is [@
	attribClause = attribClause.substr(2, attribClause.length() - 3);
	pos = attribClause.find_first_of('=');

	bool gotAttributes = false;
	if (pos < attribClause.length())
	{
		nextToken = srchToStr.substr(0, pos + 1);
		attribName = attribClause.substr(0, pos);
		attribValue = attribClause.substr(pos + 1);
		if (attribValue[0] == '\'')
			attribValue = attribValue.substr(1, attribValue.length() - 2);

		gotAttributes = true;

	}

	return gotAttributes;
}

void ShowFilteredElements(XMLElement* elem, string& nextToken, string& attribName, string& attribValue)
{
	XMLElement* current = (XMLElement*)elem->Parent();
	while (current)
	{
		XMLElement* cur1 = FindElement(nextToken, current);
		if (HasFilteredElement(attribName, attribValue, cur1))
			cout << cur1->Name() << " is " << cur1->GetText() << endl;

		current = current->NextSiblingElement();
	}
}
void ExploreDoc(XMLDocument& doc)
{
	XMLElement* x = FindElement(string("Author"), doc);
	cout << x->Name() << " is " << x->GetText() << endl;

	x = FindElement(string("DateWritten"), doc);
	cout << x->Name() << " is " << x->GetText() << endl << endl;


	x = FindElement(string("Movies"), doc);
	if (x->NoChildren())
		cout << x->Name() << " is " << x->GetText() << endl;
	else
	{
		// Creating strings here rather than in the function call in order
		// to avoid stepping through string construction when making the calls
		// and so making debugging easier.

		string Title = string("Title");
		string SType = string("Type");
		string TypeVal = string("Comedy");

		string banner = string(40, '*');

		cout << banner << endl << "All Movies" << endl << banner << endl;
		
		for(XMLElement* curElem = x->FirstChildElement(); curElem != nullptr; curElem = curElem->NextSiblingElement())
		{
			XMLElement* curMovie = FindElement(Title, curElem);
			cout << curMovie->GetText() << endl;
		}

		cout << banner << endl << endl << "All Comedies" << endl << banner << endl;
		for(XMLElement* curElem = x->FirstChildElement(); curElem != nullptr; curElem = curElem->NextSiblingElement())
		{
			XMLElement* curMovie = FindFilteredElement(Title, SType, TypeVal,curElem);
			
			if (curMovie != nullptr)
				cout << curMovie->GetText() << endl;
		}
		cout << banner << endl;

		cout << banner << endl << endl << "All Male Leads in All Comedies" << endl << banner << endl;
		for(XMLElement* curElem = x->FirstChildElement(); curElem != nullptr; curElem = curElem->NextSiblingElement())
		{
			XMLElement* curMovie = FindFilteredElement(Title, SType, TypeVal,curElem);
			
			if (curMovie != nullptr)
			{
				XMLElement* cast = FindElement(string("Actors"), curElem);
				cout << curMovie->GetText() << endl;

				XMLElement* xMaleLead = FindFilteredElement(string("Actor"), string("Role"), string("Male Lead"),cast);
						
				cout << "Male Lead:" << xMaleLead->GetText() << endl;
			}
		}


	}
}

XMLElement* FindElement(string& ElemName, XMLDocument& doc)
{

	// NOTE: FirstChild returns an XMLNode which XMLElement inherits from
	// FirstChildElement returns an XMLElement

	XMLElement* current = doc.RootElement()->FirstChildElement();
	while (current != nullptr &&
		current->Name() != ElemName)
	{
		current = (XMLElement*) current->NextSibling();
	}

	return current;
}

XMLElement* FindElement(string& ElemName, XMLElement* curElem)
{
	XMLElement* current = curElem->FirstChildElement();

	// Cycle through all the elements
	while (current != nullptr &&
		current->Name() != ElemName)
	{
		current = (XMLElement*) current->NextSibling();
	}

	return current;

}


XMLElement* FindFilteredElement(string& ElemName, string& AttribName, string& AttribValue,XMLElement* curElem)
{
	XMLElement* current = FindElement(ElemName, curElem);

	const XMLAttribute* curAttrib = current->FirstAttribute();
	if (!curAttrib)
		return nullptr;


	// Cycle through all of the attributes	
	while (curAttrib &&
		curAttrib->Name() != AttribName)
	{
		curAttrib = curAttrib->Next();
	}

	if (curAttrib && curAttrib->Value() == AttribValue)
		return current;
	else
		return nullptr;

}

bool HasFilteredElement(string& AttribName, string& AttribValue, XMLElement* current)
{
	

	const XMLAttribute* curAttrib = current->FirstAttribute();
	if (!curAttrib)
		return nullptr;


	// Cycle through all of the attributes	
	while (curAttrib &&
		curAttrib->Name() != AttribName)
	{
		curAttrib = curAttrib->Next();
	}

	if (curAttrib && curAttrib->Value() == AttribValue)
		return true;
	else
		return false;

}


void DisplayDocToScreen(string& fn,XMLDocument& doc)
{
	cout << "Printing file " << fn << " to screen" << endl;

	string banner = string(40, '*');
	cout << endl << banner << endl;
	doc.Print();
	cout << banner << endl;
}

bool OpenFile(string& fn, XMLDocument& doc, ostringstream& errmsg)
{

	XMLError err = doc.LoadFile(fn.c_str());  
	
	if (err == 0)
		errmsg << "Program has successfully opened the file:" << fn << ends;
	else
	{
		errmsg << "Program could not open the file:" << fn << ends;
		return false;
	}
	
	return true;

}

