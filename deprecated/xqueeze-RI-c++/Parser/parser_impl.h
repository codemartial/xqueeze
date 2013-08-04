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

/** \file parser_impl.h
 * \brief Interface specifications for xqMLParser classes.
 */
#ifndef XQML_PARSER_IMPL_H
#define XQML_PARSER_IMPL_H
#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <xqml-build/xqmlsymbol.h>
namespace xqML
{
  class Parser; //Forward declaration for WriterFramework to befriend Writer
}

namespace xqMLParser
{
  using std::string;
  using std::ostream;
  using std::istream;
  using xqML::xqMLSymbol;
  using xqML::Parser;

  //! Document contexts for tracking the structure of the document.
  /** This is used by WriterFramework to keep track of the structure
   * of the document being produced.
   */
  enum Context {prolog, //!< Document prolog
		start_tag, //!< (element) start tag
		ee_start_tag, //!< empty element start tag
		element, //!< element
		open_attribute, //!< open attribute
		attribute_w_predef_value //!< open attribute with predefined value
  };
  string contextName(Context);

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
  //! provides symbol-to-name/type lookup service for that specification.
  class Mapping 
  {
  public:
    //! Constructor (translation enabled)
    explicit Mapping(istream &);

    //! Constructor (translation disabled)
    Mapping();

    //! Returns name or decimal representation of the value of %xqML Symbol passed as argument
    inline string getName(unsigned int code)
    {
      return (this->*resolveFunc)(code);
    }

  private:
    typedef std::map<unsigned int, string> SymbolMap;
    SymbolMap map; //!< symbol vs. name mapping
    bool xqAStart; //!< \c true when xqA section starts
    string (xqMLParser::Mapping::* resolveFunc)(unsigned int); //!< Pointer to function used for retrieving symbol name
    unsigned int maxEL; //!< Max value of an Element in the given xqA spec
    unsigned int maxEE; //!< Max value of an Empty Element
    unsigned int maxAT; //!< Max value of an Attribute
    unsigned int maxAP; //!< Max value of an Attribute with predefined values
    unsigned int maxVA; //!< Max value of a Predefined attribute value
    unsigned int maxEN; //!< Max value of an Entity Reference

    //! Returns the name corresponding to the symbol whose value is passed as argument
    string getNameString(unsigned int);

    //! Returns the decimal value of the symbol whose value is passed as argument
    string getDecimalValue(unsigned int);

    //! Reads xqA specs from the given input stream and constructs a map
    void buildMap(istream &);
  }; 

  //! Class to hold private members for Writer
  class ParserFramework 
    {
    private:
      Context context; //!< variable to keep track of the structure of current document
      bool isEmpty; //!< set when empty element start tag is found
      Mapping mapping; //!< mapping object to provide lookup service
      istream & document; //!< stream from which xqML input to be parsed is received
      ostream & outstream; //!< stream to which the parser output should be sent
      unsigned short int elements_to_close; //!< tracks request for closing elements
      xqMLSymbol s; //!< holds the value of the current symbol
      string buffer; //!< char buffer to hold temporary data
      std::stack<string> openELstack; //!< stack to hold open elements

      //! Closes as many elements as indicated by the 3rd byte of a closing tag
      void closePendingElements();

      //! The top-most function for parsing an %xqML document
      void parseDocument();

      //! Parses the Prolog of an %xqML document
      void parseProlog();

      //! Parses an %xqML Declaration
      void parsexqMLDecl();

      //! Parses a Processing Instruction
      void parsePI();
      
      //! Parses the main content of an %xqML document
      void parseElement();

      //! Parses Attributes with arbitrary values
      void parseATAttribute();
      
      //! Parses Attributes with predefined values
      void parseAPAttribute();
      
      //! Looks for continuation of a start tag
      void parseStartTag();
      
      //! Handles a namespace prefix
      void processNSPrefix();
      
      //! Parses a namespace declaration
      void parseNSDecl();
      
      //! Parses comments
      void parseComment();

      //! Parses blocks of plain CDATA
      void parseChars();

      ParserFramework(istream & association, istream & document, ostream & outstream);
      ParserFramework(istream & document, ostream & outstream);

      ~ParserFramework() {outstream<<std::endl;}
      friend class xqML::Parser;
  };
  
} //Closed Namespace xqMLParser

// End of Include Guard
#endif 
