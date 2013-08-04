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

/** \file XMLReader.hpp
 * \brief SAX2 XMLReader Interface
 */
#ifndef XQML_XMLREADER_HPP
#define XQML_XMLREADER_HPP

#include <string>
#include <memory>
#include <iostream>

using std::string;
using std::istream;

namespace xqML
{  
  class ContentHandler;
  class DTDHandler;
  class EntityResolver;
  class ErrorHandler;
  class XqaHandler;
  
  struct XMLReader
  {
  public:
    XMLReader(XqaHandler & xh);
    ContentHandler & getContentHandler() const;
    DTDHandler & getDTDHandler() const;
    EntityResolver & getEntityResolver() const;
    ErrorHandler & getErrorHandler() const;
    bool getFeature(const string & feature) const;
    bool getProperty(const string & property) const {return false;}
    void parse(istream & input);
    void parse(const string & system_id);
    void setContentHandler(ContentHandler & ch);
    void setDTDHandler(DTDHandler & dh) {}
    void setEntityResolver(EntityResolver & er) {}
    void setErrorHandler(ErrorHandler & eh);
    void setFeature(const string & name, bool value);
    void setProperty(const string & name) {}
    ~XMLReader();
    
  private:
    class XMLReaderImpl;
    XMLReaderImpl * const reader;
  };
  
}

// End of Include Guard
#endif
