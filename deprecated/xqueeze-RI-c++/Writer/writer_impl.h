/*
 * Author: Tahir Hashmi
 * 
 * Copyright (C) 2002, Xqueeze Developers
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

/** \file writer_impl.h
 * \brief Interface specifications for xqMLWriter classes.
 */
#ifndef XQML_WRITER_IMPL_H
#define XQML_WRITER_IMPL_H
#include <iostream>
#include <string>
#include <map>
#include <xqml-build/xqmlsymbol.h>
namespace xqML
{
  class Writer; //Forward declaration for WriterFramework to befriend Writer
}

namespace xqMLWriter
{
  using std::string;
  using std::ostream;
  using std::istream;
  using std::cout;
  using xqML::xqMLSymbol;

//! Document contexts for tracking the structure of the document.
/** This is used by WriterFramework to keep track of the structure
 * of the document being produced.
 */
  enum Context {preamble, //!< Document preamble
		start_tag, //!< (element) start tag
		ee_start_tag, //!< empty element start tag
		element, //!< element
		open_attribute, //!< open attribute
		attribute_w_predef_value //!< open attribute with predefined value
  };

//! Types of xqML Symbols.
/** This is used by Mapping to identify the types of strings for
 * servicing symbol lookup requests.
 */
  enum xqMLType {EL, //!< element
		 EE, //!< empty element
		 AT, //!< attribute
		 AP, //!< attribute with predefined value
		 VA, //!< predefined attribute value
		 EN, //!< entity reference
		 NS  //!< namespace
  };

//! Support class that reads an Xqueeze %Mapping specification and
//! provides name-to-symbol lookup service for that specification.
  class Mapping 
  {
  public:
//! Constructor reads specifications from istream argument and builds
//! a lookup table.
    explicit Mapping(istream &);

//! Return xqMLSymbol of empty element given as argument
    xqMLSymbol GetEmptyElement(const string &);

//! Return xqMLSymbol of element given as argument
    xqMLSymbol GetElement(const string &);

//! Return xqMLSymbol of attribute given as argument
    xqMLSymbol GetAttribute(const string &);

//! Return xqMLSymbol of attribute with predefined values given as argument
    xqMLSymbol GetAttributeWPV(const string &);

//! Return xqMLSymbol of predefined attribute value given as argument
    xqMLSymbol GetAttributeValue(const string &);

//! Return xqMLSymbol of entity reference given as argument
    xqMLSymbol GetEntityReference(const string &);

//! Return xqMLSymbol of namespace given as argument
    xqMLSymbol GetNamespace(const string &);
  private:
    Mapping(); // No mapping without a mapfile
    typedef std::map<const string, unsigned int> SymbolMap;
    SymbolMap EL_map;
    SymbolMap EE_map;
    SymbolMap AT_map;
    SymbolMap AP_map;
    SymbolMap VA_map;
    SymbolMap EN_map;
    bool xqAStart; // True when xqA prolog found, false otherwise.
    void buildMap(istream &);
    unsigned int GetSymbol(const string &, xqMLType type, SymbolMap *);
    string typeName(xqMLType);
  }; 

//! Class to hold private members for Writer
  class WriterFramework 
    {
    private:
      Context context; //!< variable to keep track of the structure of current document
      Mapping mapping; //!< mapping object to provide lookup service
      ostream & stream; //!< stream to which the generated xqML should be written
      unsigned short int elements_to_close; //!< tracks request for closing elements

//! Returns the name of the current document context
      string contextName();

//! Closes as many elements as there were previous consecutive calls to Writer::endElement
      void closePendingElements();

//! Constructor, creates a xqMLWriter::Mapping object
      WriterFramework(istream &, ostream &);

//! Destructor calls closePendingElements() before exiting.
      ~WriterFramework();
      friend class xqML::Writer;
  };
  
} //Closed Namespace xqMLWriter

// End of Include Guard
#endif 
