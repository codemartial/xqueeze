/*
 * Author: Tahir Hashmi
 * 
 * Copyright (C) 2003 Xqueeze Developers
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

/** \file saxwriter.h
 * \brief Interface specifications for xqML::SAXWriter.
 */
#ifndef XQML_SAXWRITER_H
#define XQML_SAXWRITER_H
#include <string>
#include <xqml/writer.h>

namespace xqML 
{
  using std::string;
  using std::ostream;

  //! A Writer offering SAX interface for generation of documents
  class SAXWriter: public xqML::Writer, public xqML::SAX
  {
  public:
    
    //! Create a SAXWriter that would write to the supplied ostream
    explicit SAXWriter(ostream & = std::cout);

    //! Set the EntityResolver object that should be used
    virtual void setEntityResolver(const xqML::EntityResolver &);
    
    //! Set the ErrorHandler object that should be used
    virtual void setErrorHandler(const xqML::ErrorHandler &);
    
    //! Writes an attribute.
    void attribute (const string &, const string &, bool = false);

    //! Writes a CDATA section that is passed as a string argument.
    void cdataSection (const string &);

    //! Writes the character reference for the integer argument.
    void characterReference (int);

    //! Writes a comment
    void comment (const string &);

    //! Closes an open attribute
    void endAttribute ();

    //! Ends an element. The string argument is redundant and defaults to blank.
    void endElement (const string & = "");

    //! Writes an Entity Reference that is passed as the string argument
    void entityReference (const string &);

    //! Writes a Processing instruction
    void processingInstruction (const string &, const string &);

    //! Writes a prolog, declaring the given string argument as doctype
    void prolog(const string &);

    //! Starts an Attribute.
    void startAttribute (const string &);

    //! Starts an Element.
    void startElement (const string &, bool = false);

    ~SAXWriter();
  } ;
} //Namespace xqML

// End of Include Guard
#endif
