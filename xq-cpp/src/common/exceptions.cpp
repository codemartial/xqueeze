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

/** \file exceptions.cpp
 * \brief Implementation of exception classes
 */

#include <sstream>
#include<xqml/exceptions.hpp>

namespace xqML
{
  using std::string;
  
  IllegalIdentifier::IllegalIdentifier(const string & uri, const string & name)
    : uri_(uri), name_(name) {}

  IllegalIdentifier::IllegalIdentifier(const string & uri, const uint32_t code) {
    std::ostringstream codestring;
    codestring<<code;
    name_ = codestring.str();
    uri_ = uri;
  }

  string IllegalIdentifier::what() const
  {
      return "Illegal Identifier: {" + uri_ + "} " + name_;
  }

  IllegalContext::IllegalContext(const string & ill_context, const string & uri, const string & name)
    : context(ill_context), uri_(uri), name_(name) {}

  IllegalContext::IllegalContext(const string & ill_context, const string & uri, const unsigned int code) {
    context = ill_context;
    uri_ = uri;
    std::ostringstream codestring;
    codestring<<code;
    name_ = codestring.str();
    }

  string IllegalContext::what() const
  {
    return "Illegal Context: " + context + " near {" + uri_ + "} " + name_;
  }

  SAXParseException::SAXParseException(IllegalContext & e)
    : exc(std::auto_ptr<xqException> (&e)) {}

  SAXParseException::SAXParseException(IllegalIdentifier & e)
    : exc(std::auto_ptr<xqException> (&e)) {}

  string SAXParseException::what() const 
  {
    return exc->what();
  }    
  
  SAXNotSupported::SAXNotSupported(const string & message)
    : msg(message){}
  
  string SAXNotSupported::what() const 
  {
    return msg;
  }
  
  SAXNotRecognized::SAXNotRecognized(const string & message)
    : msg(message){}

  string SAXNotRecognized::what() const
  {
    return msg;
  }
  
  XqaNotAvailable::XqaNotAvailable(const string & uri)
    : uri_(uri)
  {}

  string XqaNotAvailable::what() const 
  {
    return "Xqa not available for URI: " + uri_ + "\n";
  }
}
