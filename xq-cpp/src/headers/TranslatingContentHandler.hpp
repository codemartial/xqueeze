/*
 * Author: Tahir Hashmi
 * 
 * Copyright (C) 2003, Xqueeze Developers
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

/** \file TranslatingContentHandler.hpp
 *  \brief Translating Xqueeze ContentHandler (implements xqML::ContentHandler)
 */
#ifndef XQML_TRANSLATING_CONTENT_HANDLER_H
#define XQML_TRANSLATING_CONTENT_HANDLER_H

#include <string>
#include <iostream>
#include <memory>
#include <xqml/ContentHandler.hpp>
#include <xqml/XqaHandler.hpp>

namespace xqML
{
  using std::istream;
  using std::ostream;
  using std::string;
  
  struct CHContext;
  
  /** \class TranslatingContentHandler ContentHandler.hpp xqml/TranslatingContentHandler.hpp
      \brief A ContentHandler that generates xqML documents with strings to symbol translation
  */
  class TranslatingContentHandler : public ContentHandler
  {
  public:
    //! Constructor takes an ostream (default std::cout) for
    //! the generated output.
    TranslatingContentHandler(XqaHandler & xqa_handler, ostream & doc = std::cout, bool dynamic = false);

    //! Copy Constructor
    TranslatingContentHandler(TranslatingContentHandler & t);
    
    void characters(const string &);

    void endDocument();

    void endElement(const string & namespaceURI, const string & localName, const string & qName);

    void endPrefixMapping(const string & prefix);
    
    void ignorableWhitespace(const string & spaces);
    
    void processingInstruction(const string & target, const string & data);

    void setDocumentLocator(Locator & locator);

    void skippedEntity(const string & name);

    void startDocument();

    void startDocument(const string & encoding);

    void startElement(const string & uri, const string & localName, const string & qName, const Attributes & attributes);

    void startPrefixMapping(const string & prefix, const string & uri);
    
    virtual ~TranslatingContentHandler();
  protected:
   std::auto_ptr<CHContext> context_data;
  };
} // Namespace xqML

// End of Include Guard
#endif
