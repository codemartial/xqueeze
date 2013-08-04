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

/** \file TranslatingContentHandler.cpp
 *  \brief Translating Xqueeze ContentHandler (implements xqML::ContentHandler)
 */

#include <xqml/xqmlsymbol.hpp>
#include <xqml/TranslatingContentHandler.hpp>
#include "ContentHandlerState.hpp"
#include "CHContext.hpp"

namespace xqML
{

  TranslatingContentHandler::TranslatingContentHandler(XqaHandler & xqa_handler, ostream & document, bool dynamic)
    :context_data(new CHContext(xqa_handler, document, dynamic))
  {}
  
  TranslatingContentHandler::TranslatingContentHandler(TranslatingContentHandler & t)
    :context_data(new CHContext(t.context_data->xqa_handler, t.context_data->out, t.context_data->do_dynamic_maps))
  {}
  
  TranslatingContentHandler::~TranslatingContentHandler()
  {}
  
  void TranslatingContentHandler::characters (const string & cdata)
  {
    context_data->ch_state->characters(cdata);
  }

  //! Finishes up the document
  void TranslatingContentHandler::endDocument ()
  {
    context_data->ch_state->endDocument ();
  }

  //! Ends an element. The string argument is redundant and defaults to blank.
  void TranslatingContentHandler::endElement (const string & namespaceURI, const string & localName, const string & qName)
  {
    context_data->ch_state->endElement (namespaceURI, localName, qName);
  }
  
  //! Ends the scope of a prefix-URI mapping
  void TranslatingContentHandler::endPrefixMapping(const string & prefix)
  {
    context_data->ch_state->endPrefixMapping(prefix);
  }
    
  //! Handles ignorable whitespace
  void TranslatingContentHandler::ignorableWhitespace(const string & spaces)
  {
    context_data->ch_state->ignorableWhitespace(spaces);
  }
    
  //! Writes a Processing instruction
  void TranslatingContentHandler::processingInstruction (const string & target, const string & data)
  {
    context_data->ch_state->processingInstruction (target, data);
  }
  
  //! Sets the Locator object for location information
  void TranslatingContentHandler::setDocumentLocator(Locator & locator)
  {
    context_data->ch_state->setDocumentLocator(locator);
  }

  //! Writes an Entity Reference that is passed as the string argument
  void TranslatingContentHandler::skippedEntity (const string & name)
  {
    context_data->ch_state->skippedEntity (name);
  }

  //! Writes a prolog
  void TranslatingContentHandler::startDocument()
  {
    context_data->ch_state->startDocument();
  }
  
  //! Writes a prolog with supplied doctypename
  void TranslatingContentHandler::startDocument(const string & encoding)
  {
    context_data->ch_state->startDocument(encoding);
  }
  
  //! Starts an Element.
  void TranslatingContentHandler::startElement (const string & uri, const string & localName, const string & qName, const Attributes & attributes)
  {
    context_data->ch_state->startElement (uri, localName, qName, attributes);
  }
  
  //! Starts a prefix-to-URI mapping
  void TranslatingContentHandler::startPrefixMapping(const string & prefix, const string & uri)
  {
    context_data->ch_state->startPrefixMapping(prefix, uri);
  }
  
} // Namespace xqML
