/*
 * Author: Tahir Hashmi
 * 
 * Copyright (C) 2002 2003, Xqueeze Developers
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

/** \file writer.h
 * \brief Interface specifications for xqML::Writer.
 */
#ifndef XQML_WRITER_H
#define XQML_WRITER_H
#include <string>

//! Namespace for implementation specific interfaces for Writer
namespace xqMLWriter
{
  class WriterFramework;
}

//! Namespace for client accessible interfaces
namespace xqML 
{
  using std::string;
  using std::istream;
  using std::ostream;
  /** \defgroup writer Xqueeze Writer
   *  \brief xqML Generation API
   *  
   *  Xqueeze Writer provides API for generation of xqML documents
   *  against a given xqA specification. The API allows generation of
   *  xqML documents through a set of SAX-like calls that write
   *  various XML structures in xqML encoding.
   *  @{
   */
  //! Provides API to the client for generation of xqML Documents
  class Writer 
  {
  public:
//! Constructor takes an istream (default std::cin) for
//! Xqueeze Association Specs and an ostream (default std::cout) for
//! the generated output.
    explicit Writer(istream & = std::cin, ostream & = std::cout);

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

    ~Writer();

  private:
    xqMLWriter::WriterFramework* writer_;
  } ;
  /** @} */  
} //Namespace xqML

// End of Include Guard
#endif
