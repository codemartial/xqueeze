/*
 * Author: Tahir Hashmi
 * 
 * Copyright (C) 2004, Xqueeze Developers
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

/** \file XMLReaderImpl.cpp
 * \brief XMLReaderImpl definition
 */

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>
#include <xqml/exceptions.hpp>
#include <xqml/XMLReader.hpp>
#include <xqml/XqaHandler.hpp>
#include <xqml/ContentHandler.hpp>
#include <xqml/Attributes.hpp>
#include "XMLReaderImpl.hpp"

using std::istream;
using std::string;
using std::ifstream;

namespace xqML
{
  XMLReader::XMLReader(XqaHandler & xh)
    : reader(new XMLReaderImpl(xh))
  {}
  
  XMLReader::~XMLReader()
  {
    delete reader;
  }
  
  ContentHandler & XMLReader::getContentHandler() const
  {
    return reader->getContentHandler();
  }
  
//   DTDHandler & XMLReader::getDTDHandler() const
//   {
//   }
  
//   EntityResolver & XMLReader::getEntityResolver() const
//   {
//   }
  
  ErrorHandler & XMLReader::getErrorHandler() const
  {
    return reader->getErrorHandler();
  }
  
  bool XMLReader::getFeature(const string & feature) const
  {
    return reader->getFeature(feature);
  }
  
  void XMLReader::parse(istream & input)
  {
    reader->parse(input);
  }
  
  void XMLReader::parse(const string & system_id)
  {
    reader->parse(system_id);
  }
  
  void XMLReader::setContentHandler(ContentHandler & ch)
  {
    reader->setContentHandler(ch);
  }
  
  void XMLReader::setErrorHandler(ErrorHandler & eh)
  {
    reader->setErrorHandler(eh);
  }
  
  void XMLReader::setFeature(const string & name, bool value)
  {
    reader->setFeature(name, value);
  }
  
  XMLReader::XMLReaderImpl::XMLReaderImpl(XqaHandler & xh)
    : xqa_handler(xh), namespaces(true)
  {}
  
  ContentHandler & XMLReader::XMLReaderImpl::getContentHandler()
  {
    return *content_handler;
  }
  
  ErrorHandler & XMLReader::XMLReaderImpl::getErrorHandler()
  {
    return *error_handler;
  }
  
  bool XMLReader::XMLReaderImpl::getFeature(const string & feature)
  {
    if(feature == "namespaces")
      return namespaces;
    if(feature == "namespace-prefixes")
      return namespace_prefixes;
    throw SAXNotRecognized(feature);
  }
  
  void XMLReader::XMLReaderImpl::parse(istream & i)
  {
    input = &i;
    parseProlog();
    parseElement();
    parseEpilog();
  }
  
  void XMLReader::XMLReaderImpl::parse(const string & system_id)
  {
    ifstream in_file(system_id.c_str(), std::ios_base::binary);
    parse(in_file);
  }
  
  void XMLReader::XMLReaderImpl::setContentHandler(ContentHandler & ch)
  {
    content_handler = &ch;
  }
  
  void XMLReader::XMLReaderImpl::setErrorHandler(ErrorHandler & eh)
  {
    error_handler = &eh;
  }
  
  void XMLReader::XMLReaderImpl::setFeature(const string & feature, bool value)
  {
    if(feature == "namespaces")
      namespaces = value;
    if(feature == "namespace-prefixes")
      namespace_prefixes = value;
    throw SAXNotRecognized(feature);
  }
  
  //
  // Implementation helper functions for parse()
  //

  void XMLReader::XMLReaderImpl::parseProlog()
  {
    int c = 0;
    while(input->get() != 0x1E) {
      ++c;
    }
    
    if(input->get()) {
      SAXParseException exc(*(new IllegalContext("Prolog", current_uri, "Bad format specifier")));
      throw exc;
    }
    if (input->get() != 0x02)
      throw SAXNotSupported("Non - 8-bit encoding"); // 16-bit encoding support not implemented
    if (input->get() != 0x04)
      throw SAXNotSupported("Unknown xqML Version number");
    while(input->get() != 0x1E); // See grammar production 3
    *input>>s;
    while(parsePI());
    if (parseDecl() )
      while(parsePI());
  }
  
  bool XMLReader::XMLReaderImpl::parsePI()
  {
    if (s != 0x20) return false;
    string target, data ;
    getline(*input, target, '\x1E');
    getline(*input, data, '\x1E');
    content_handler->processingInstruction(target, data);
    content_handler->characters(sync()); // Could accept CDATA after a PI in prolog
    return true;
  }
  
  bool XMLReader::XMLReaderImpl::parseDecl()
  {
    switch (s)
      {
      case 0x2C:
	sync();
	break;
      case 0x20:
	xqa_handler.registerXqA("", input);
	//FIXME: Blank URIs from multiple readers will clash
	break;
      case 0x2E:
	sync();
	break;
      default:
	return false;
      }
    return true;
  }
  
  unsigned int XMLReader::XMLReaderImpl::parseElement()
  {
    while(parsePrefix());
    while(parseReg());
    uint32_t flags = s;

    if (flags < 0x0100) {
      if ((flags & 0x30) == 0x30 ) {

	if (flags & 0x04)
	  current_uri = uri_map[input->get()];

	if (flags & 0x08)
	  closeElement();

	*input>>s; parseSTag();

	if(flags & 0x02) {
	  closeElement();
	  content_handler->characters(sync());
	  return 0;
	}
      }
      else {
	SAXParseException exc(*(new IllegalContext("Start Tag", current_uri, s)));
	throw exc;
      }
    }
    else parseSTag();

    content_handler->characters(sync());
    unsigned int to_close = parseContent();
    if (to_close) {
      closeElement();
      return --to_close;
    }

    to_close = parseETag();
    closeElement();
    if(--to_close) return to_close;
    content_handler->characters(sync());
    return 0;
  }
  
  bool XMLReader::XMLReaderImpl::parsePrefix()
  {
    static uint32_t last_code;
    
    if(s == 0x1C) {
      string pfx, uri;
      getline(*input, pfx, '\x1E');
      getline(*input, uri, '\x1E');
      *input>>s;
      
      current_uri = uri;
      last_code = xqMLSymbol::nextSymbol(last_code);
      uri_map[last_code] = uri;
      if(namespace_prefixes) pfx_map[uri_map[last_code]] = pfx;
      xqa_map[uri] = xqa_handler.getXqA(uri);
      content_handler->startPrefixMapping(pfx, uri);
      return true;
    }
    return false;
  }
  
  bool XMLReader::XMLReaderImpl::parseReg()
  {
    string reg_uri = current_uri, buffer;
    switch (s)
      {
      case 0x28:
	*input>>s;
	reg_uri = uri_map[s];
      case 0x2A:
	*input>>s;
	getline(*input, buffer, '\x1E');
	dyn_map[reg_uri][s] = buffer;
	*input>>s;
	return true;
      default:
	return false;
      }
  }
  
  void XMLReader::XMLReaderImpl::parseSTag()
  {
    string el_name = lookup(current_uri, s), q_name = "";
    Attributes atts;
    while(parseAttributes(atts));
    open_el_stack.push(std::pair<string, string>(current_uri, el_name));
    if (namespace_prefixes)
      q_name = pfx_map[current_uri] + ":" + el_name;
    if(namespaces)
      content_handler->startElement(current_uri, el_name, q_name, atts);
    else 
      content_handler->startElement("", "", q_name, atts);
  }
  
  unsigned int XMLReader::XMLReaderImpl::parseContent()
  {
    unsigned int to_close;
    do {
      if (s & 256) {
	if ( (to_close = parseElement()) )
	  return to_close;
	else continue;
      }
      
      switch(s)
	{
	case 0x20:
	  parsePI();
	  break;
	case 0x22:
	  *input>>s;
	  content_handler->characters(lookup(current_uri, s));
	  content_handler->characters(sync());
	  break;
	case 0x26:
	  *input>>s;
	  {
	    std::ostringstream os;
	    os<<((s & 0xFF)>>1);
	    content_handler->characters(os.str());
	  }
	  content_handler->characters(sync());
	  break;	  
	case 0x30:
	  return 0;
	case 0x1C:
	case 0x2A:
	case 0x28:
	case 0x32:
	case 0x34:
	case 0x36:
	case 0x38:
	case 0x3A:
	case 0x3C:
	case 0x3E:
	  if ( (to_close = parseElement()) )
	    return to_close;
	  break;
	default:
	  SAXParseException exc(*(new IllegalContext("Content", current_uri, s)));
	  throw exc;
	}
    } while(!open_el_stack.empty());
    return 0;
  }
  
  bool XMLReader::XMLReaderImpl::parseAttributes(Attributes & atts)
  {
    string att_uri = current_uri;
    string buffer;
    Attribute att;
    switch (input->get())
      {
      case 0x14:
	*input>>s;
	att_uri = uri_map[s];
      case 0x16:
	*input>>s;
	if (namespaces) {
	  att.uri = att_uri;
	  att.name = lookup(att_uri, s);
	}

	if (namespace_prefixes)
	  att.q_name = pfx_map[att_uri] + ":" + lookup(att_uri, s);
	getline(*input, buffer, '\x16');

	if(buffer.find('\x1E') < buffer.size()) {// We may have found a reference
	  xqMLSymbol ref_check;
	  getReservedSymbol(*input, ref_check);
	  if (ref_check != 0x22 && ref_check != 0x26) {
	    SAXParseException exc(*(new IllegalContext("Reference", current_uri, s)));
	    throw exc;
	  }
	  att.value = buffer.substr(0, buffer.find('\x1E'));
	  *input>>s;

	  std::ostringstream os;
	  os<<((s & 0xFF)>>1);
	  ref_check == 22 ?
	    att.value += lookup(att_uri, s) + buffer.substr(buffer.find('\x1E') +1, buffer.size() - buffer.find('\x1E') -1) :
	    att.value += os.str()
	    + buffer.substr(buffer.find('\x1E') +1, buffer.size() - buffer.find('\x1E') -1);
	}

	break;
      case 0x18:
	*input>>s;
	att_uri = uri_map[s];
      case 0x1A:
	*input>>s;
	if (namespaces) {
	  att.uri = att_uri;
	  att.name = lookup(att_uri, s);
	}
	if (namespace_prefixes)
	  att.q_name = pfx_map[att_uri] + ":" + lookup(att_uri, s);
	*input>>s;
	att.value = lookup(att_uri, s);
	break;
      default:
	input->unget();
	return false;
      }
    atts.push_back(att);
    return true;
  }

  unsigned int XMLReader::XMLReaderImpl::parseETag()
  {
    getReservedSymbol(*input, s);
    return s;
  }
  
  void XMLReader::XMLReaderImpl::parseEpilog()
  {
    while(parsePI());
  }
  
  void XMLReader::XMLReaderImpl::closeElement()
  {
    string q_name;
    std::pair<string, string> el = open_el_stack.top();
    if(namespace_prefixes)
      q_name = pfx_map[el.first] + ":" + el.second;
    if(namespaces)
      content_handler->endElement(el.first, el.second, q_name);
    else
      content_handler->endElement("", "", q_name);
    if (open_el_stack.empty()) {
      SAXParseException exc(*(new IllegalContext("End Element", current_uri, "no open elements")));
      throw exc;
    }
    open_el_stack.pop();
  }

  string XMLReader::XMLReaderImpl::lookup(const string & uri, uint32_t symbol)
  {
    XqaMap::iterator i = xqa_map.find(uri);
    if(i != xqa_map.end() && i->second->find(symbol) != i->second->end())
	return (*i->second)[symbol];

    if ( (dyn_map.find(uri) != dyn_map.end() ) && 
	 (dyn_map[uri].find(symbol) != dyn_map[uri].end()) )
      return dyn_map[uri][symbol];

    SAXParseException exc(*(new IllegalIdentifier(uri, symbol)));
    throw exc;
  }
  
  string XMLReader::XMLReaderImpl::sync()
  {
    string characters;
    getline(*input, characters, '\x1E');
    *input>>s;
    return characters;
  }  
    
}
