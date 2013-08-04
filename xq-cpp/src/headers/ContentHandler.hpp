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

/** \file ContentHandler.hpp
 * \brief Interface specifications for xqML::ContentHandler.
 */
#ifndef XQML_CONTENTHANDLER_H
#define XQML_CONTENTHANDLER_H
#include <string>
#include <xqml/Attributes.hpp>

//! Namespace for client accessible interfaces
namespace xqML 
{
  using std::string;
  using std::istream;
  using std::ostream;

  class Locator;
  
  /** \defgroup contenthandler Xqueeze ContentHandler
   *  \brief xqML Handling API
   *  
   *  xqML::ContentHandler Interface is the interface expected by
   *  xqML::XMLReader. This module also contains the Xqueeze
   *  ContentHandler that can receive SAX events from an XMLReader and
   *  generate an xqML Document. Xqueeze ContentHandler can be taken
   *  by xqML::XMLReader as a template parameter, thereby eliminating
   *  virtual function calls and pointer indirections.
   *  @{
   */
  //! The interface expected by xqML::XMLReader for a ContentHandler.
  //! Users should provide a class that provides this interface.
  class ContentHandler 
  {
  public:
    //! Writes the characters received
    virtual void characters (const string &) = 0;

    //! Finishes up the document
    virtual void endDocument () = 0;

    //! Ends an element. The string argument is redundant and defaults to blank.
    virtual void endElement (const string & namespaceURI, const string & localName, const string & qName) = 0;

    //! Ends the scope of a prefix-URI mapping
    virtual void endPrefixMapping(const string & prefix) = 0;
    
    //! Handles ignorable whitespace
    virtual void ignorableWhitespace(const string & spaces) = 0;
    
    //! Writes a Processing instruction
    virtual void processingInstruction (const string & target, const string & data) = 0;

    //! Sets the Locator object for location information
    virtual void setDocumentLocator(Locator & locator) = 0;

    //! Writes an Entity Reference that is passed as the string argument
    virtual void skippedEntity (const string & name) = 0;

    //! Writes a prolog
    virtual void startDocument() = 0;

    //! Starts an Element.
    virtual void startElement (const string & uri, const string & localName, const string & qName, const Attributes & attributes) = 0;

    //! Begins the scope of a prefix-URI namespace mapping
    virtual void startPrefixMapping(const string & prefix, const string & uri) = 0;
    
  } ;
  /** @} */  
} //Namespace xqML

// End of Include Guard
#endif
