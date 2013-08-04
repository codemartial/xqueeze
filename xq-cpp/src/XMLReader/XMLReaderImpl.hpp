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

/** \file XMLReaderImpl.hpp
 * \brief XMLReader Implementation Class
 */
#ifndef XQML_XMLREADERIMPL_HPP
#define XQML_XMLREADERIMPL_HPP

#include <map>
#include <stack>
#include <xqml/xqmlsymbol.hpp>
#include <xqml/XMLReader.hpp>
#include <xqml/XqMap.hpp>

namespace xqML
{  
  struct XMLReader::XMLReaderImpl
  {
    XMLReaderImpl(XqaHandler & xh);
    ContentHandler & getContentHandler();
    ErrorHandler & getErrorHandler();
    bool getFeature(const string & feature);
    void parse(istream & input);
    void parse(const string & system_id);
    void setContentHandler(ContentHandler & ch);
    void setErrorHandler(ErrorHandler & eh);
    void setFeature(const string & name, bool value);
  private:
    typedef std::map<string, xqML::XqA *> XqaMap;
    typedef std::map<string, std::map<uint32_t, string> > DynMap;
    typedef std::map<xqML::xqMLSymbol, string> URIMap;
    typedef std::map<string, string> PfxMap;
    typedef std::stack<std::pair<string, string> > OpenELStack;
    
    ContentHandler * content_handler;
    ErrorHandler * error_handler;
    XqaHandler & xqa_handler;
    bool namespaces;
    bool namespace_prefixes;
    istream * input;
    string current_uri;
    xqMLSymbol s;
    XqaMap xqa_map;
    DynMap dyn_map;
    URIMap uri_map;
    PfxMap pfx_map;
    OpenELStack open_el_stack;
    
    void parseProlog();
    unsigned int parseElement();
    void parseEpilog();
    bool parsePI();
    bool parseDecl();
    bool parsePrefix();
    bool parseReg();
    void parseSTag();
    bool parseAttributes(Attributes & atts);
    
    unsigned int parseContent();
    unsigned int parseETag();
    void closeElement();
    string sync();
    string lookup(const string &, uint32_t);
    
  };

}
// End of Include Guard
#endif
