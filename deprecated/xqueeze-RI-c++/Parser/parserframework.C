/*
 * Author: Tahir Hashmi
 * 
 * Copyright (C) 2002, 2003 Xqueeze Developers
 * 
 * This file is part of Xqueeze Reference Implementation.
 * 
 * Xqueeze Reference Implementation is free software; you can
 * redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 * Xqueeze Reference Implementation is distributed in the hope that
 * it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Xqueeze Reference Implementation; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA
 */

/**\file parserframework.C
 * \brief Implementation of xqMLParser::ParserFramework
 */

#include <xqml/parser.h>
#include <xqml/exceptions.h>
#include <Parser/parserframework.h>
#include <iostream>
#include <string>

namespace xqMLImpl
{
  using xqMLImpl::contextName;
  using xqML::IllegalContext;
  using std::cin;
  using std::istream;
  using std::string;

  /** Sets the input source for the document to parse. The source
   *  stream may contain one or more xqML documents.
   *  \promises The parser will be set to parse from the 
   *  supplied input
   *  stream amd all previous state information will be cleared.
   */
  void ParserFramework::setSource(istream & source)
  {
    document = source;
    context = ready;
  }
  
  /** Parses xqML document(s) in the input stream set by a call to
   *  setSource(). For the same parser object and same source, this
   *  function may be called several times if the stream contains
   *  several documents.
   *  \promises
   *     - Will parse one xqML document in an input stream
   *     - Will report syntax errors through exceptions
   *     - Will return only when one complete document has been
   *     successfully parsed
   *     .
   *  \requires
   *     - The caller does not call this function while a parse was
   *     already underway
   *     - The caller handles all the standard system exceptions
   *     .
   *  \throws xqML::IllegalContext
   *  \throws xqML::ParseError
   *  \throws [misc] standard system exceptions
   */
  void ParserFramework::parseDocument()
  {
    if(context == end) context = ready;
    if(context != ready) throw IllegalContext("not ready", "", "");
    while(document.get() != 0x1E);
    document>>s;
    parseProlog();
    parseElement();
    while(document && s.getCode() = 0x20) parsePI();
    context = end;
    return;
  }
  
  /** Returns if prolog is not found, otherwise parses the prolog,
   *  emitting the xqML version, doctype declaration and any comments
   *  or PIs found in the prolog section.
   *
   *  Consumes an xqML Symbol before returning
   */
  void ParserFramework::parseProlog()
  {
    while(context == prolog && document) {
      switch (s.getCode()) {
      case 0x02: //xqML Declaration
	parsexqMLDecl();
	break;
      case 0x1A: //NS Prefix - I expect this to be an element start tag
	context = start_tag;
	document>>s;
	break;
      case 0x20: //PI
	parsePI();
	break;
      case 0x22: //Comment
	parseComment();
	break;
      case 0x2A: //EE Start Tag
	context = start_tag;
	document>>s;
	break;
      case 0x2C: //Doctype Declaration
	getline(document, buffer, '\x1E');
	outstream<<"\n<!DOCTYPE "<<buffer<<">\n";
	document>>s;
	break;
      case 0x2E: //DTD Section
	getline(document, buffer, '\x1E');
	outstream<<'\n'<<buffer;
	document>>s;
	break;
      default:
	if (s.getCode() < 256) throw IllegalContext(contextName(context), s.getCode());
	context = start_tag;
	break;
      }
    }
  }

/** This function is the work-horse of the parser. There is only one
 *  do... while statement in this function. At the end of each
 *  iteration of the do... while statement, an xqML Symbol is consumed
 *  that is checked for or value, if it is a reserved
 *  symbol and the appropriate action is taken depending on it's type
 *  or value. The do... while loop terminates when there are no
 *  elements left in openELstack.
 *
 *  Consumes an xqML Symbol before returning
 */
  void ParserFramework::parseElement()
  {
    do {
      //std::cerr<<"Iterating through the do-while. Open elements on stack: "<<openELstack.size()<<'\n';
      switch (s.getCode()) 
	{
	case 0x16:
	  parseATAttribute();
	  break;
	case 0x20:
	  parsePI();
	  break;
	case 0x22:
	  parseComment();
	  break;
	case 0x24: //Entity Reference
	  if (context !=open_attribute) context = element;
	  document>>s;
	  outstream<<'&'<<mapping.getName(s.getCode())<<';';
	  parseChars();
	  break;
	case 0x26: //Character Reference
	  unsigned int i;
	  document>>i;
	  if (document.get() != '\x1E') throw IllegalContext(contextName(context), s.getCode());
	  if (i < 128)
	    outstream<<static_cast<char>(i);
	  else
	    outstream<<'?'; // wchar support not provided yet
	  parseChars();
	  break;
	case 0x28: //CDATA Section
	  getline(document, buffer, '\x1E');
	  outstream<<"<![CDATA["<<buffer<<"]]>";
	  parseChars();
	  break;
	case 0x2A:
	  isEmpty = true;
	  context = start_tag;
	  document>>s;
	  break;
	case 0x3E:
	  closePendingElements();
	  break;
	default:
	  if(s.getCode() < 256) throw IllegalContext(contextName(context), s.getCode());
	  //std::cerr<<"Found element \""<<mapping.getName(s.getCode())<<"\"... ";
	  if(!isEmpty) openELstack.push(mapping.getName(s.getCode()));
	  context = start_tag;
	  outstream<<"<"<<mapping.getName(s.getCode());
	  parseStartTag();
	  break;
	}
      //std::cerr<<"Iterated through do-while loop. Open elements on stack: "<<openELstack.size()<<'\n';
    }
    while(!openELstack.empty() && document);
  }

  /** This function checks whether there is an attribute or namespace
   * declaration ahead and calls the appropriate function if one is
   * found. If there's no attribute or namespace declaration ahead, it
   * sets the context to "element", closes the start tag (or element,
   * if it is an empty element) and calls parseChars(). 
   */
  void ParserFramework::parseStartTag()
  {
    //std::cerr<<"Parsing Start Tag\n";
    char nextChar = document.peek();
    if(nextChar == 0x16) {
      //std::cerr<<"found ATAttribute\n";
      document.ignore();
      parseATAttribute();
    }
    else if(nextChar == 0x18) {
      //std::cerr<<"found APAttribute\n";
      document.ignore();
      parseAPAttribute();
    }
    else if(nextChar == 0x1C) {
      //std::cerr<<"found NS Declaration\n";
      document.ignore();
      parseNSDecl();
    }
    else {
      //std::cerr<<"found "<<static_cast<unsigned int>(nextChar)<<'\n';
      context = element;
      if(isEmpty) {
	outstream<<'/';
	isEmpty = false;
      }
      outstream<<'>'; //Close start tag
      parseChars();
    }
  }

  /** This function terminates an open ATAttribute if the context is
  "open attribute" or starts a new one if the context is "start tag"
  */
  void ParserFramework::parseATAttribute()
  {
    if (context == open_attribute) {
      //std::cerr<<" closed\n";
      outstream<<"\""; //End of ATAttribute value
      context = start_tag;
      parseStartTag();
    }
    else if (context == start_tag) {
      //std::cerr<<"Opening attribute... ";
      document>>s; //Read in the ATAttribute Symbol
      if(s.getCode() == 0x1A) processNSPrefix();
      context = open_attribute;
      //std::cerr<<mapping.getName(s.getCode())<<" opened... ";
      outstream<<" "<<mapping.getName(s.getCode())<<"=\"";
      parseChars();
    }
    else 
      throw IllegalContext(contextName(context), mapping.getName(s.getCode()), "attribute");
  }

  void ParserFramework::parseAPAttribute()
  {
    if(context != start_tag)
      throw IllegalContext(contextName(context), mapping.getName(s.getCode()), "attribute");
    document>>s; //Read in the APAttribute Symbol
    if(s.getCode() == 0x1A) processNSPrefix();
    outstream<<" "<<mapping.getName(s.getCode())<<"=\"";
    document>>s; //Read in the APAttribute value 
    outstream<<mapping.getName(s.getCode())<<"\"";
    parseStartTag();
  }

  void ParserFramework::parseNSDecl()
  {
    if(context != start_tag)
      throw IllegalContext(contextName(context), mapping.getName(s.getCode()), "namspace declaration");
    outstream<<" xmlns";
    getline(document, buffer, '\x1E');
    if(buffer.size())
      outstream<<':'<<buffer;
    getline(document, buffer, '\x1E');
    outstream<<"=\""<<buffer<<"\"";
    parseStartTag();
  }

  void ParserFramework::processNSPrefix() {
    std::cerr<<"Parser: I don't know how to process NS Prefix\n";
  }

  void ParserFramework::parsePI()
  {
    if(context == open_attribute)
      throw IllegalContext(contextName(context), mapping.getName(s.getCode()), "PI");
    if(context == start_tag) {
      context = element;
      if(isEmpty) outstream<<'/';
      outstream<<'>'; //Close start tag
      parseChars();
    }
    outstream<<"<?";
    getline(document, buffer, '\x1E');
    outstream<<buffer;
    getline(document, buffer, '\x1E');
    outstream<<" "<<buffer<<"?>";
    parseChars();
  }
  
    
  void ParserFramework::parseComment()
  {
    getline(document, buffer, '\x1E');
    outstream<<"\n<!--"<<buffer<<"-->";
    parseChars();
  }
  
  void ParserFramework::parsexqMLDecl()
  {
    getline(document, buffer, '\x1E');
    size_t hyphen_pos = buffer.find('-');
    if(hyphen_pos < buffer.size())
      outstream<<"<?xqML version=\""<<buffer.substr(0, hyphen_pos)<<"\""
	       <<" encoding=\""<<buffer.substr(hyphen_pos + 1, buffer.size() - hyphen_pos - 1)<<"\"?>";
    else
      outstream<<"<?xqML version=\""<<buffer<<"\"?>";
    document>>s;
  }
  

/** Reads upto the first occurence of the markup flag  and consumes the xqML
    Symbol immediately following the flag
 */
  void ParserFramework::parseChars()
  {
    getline(document, buffer, '\x1E');
    outstream<<buffer;
    document>>s;
  }

/** Pops off as many elements as indicated by the 3rd byte of a closing tag from
 * openELstack.
 *
 * \sideeffects
 *    - Document context is set to "element"
 *    - calls parseChars() to consume the next xqML Symbol
 */
  void ParserFramework::closePendingElements()
  {
    unsigned char elements_to_close = document.get();
    //std::cerr<<"About to close "<<static_cast<unsigned short int>(elements_to_close)<<" open elements\n";
    while(elements_to_close-- && !openELstack.empty()) {
      outstream<<"</"<<openELstack.top()<<'>';
      openELstack.pop();
      //std::cerr<<static_cast<unsigned short int>(elements_to_close)<<" more to go\n";
      //std::cerr<<"Elements on stack: "<<openELstack.size()<<'\n';
    }
    context = element;
    parseChars();
  }
}
