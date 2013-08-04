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

/** \file parserframework.h
 *  \brief Definition of xqMLImpl::ParserFramework
 */

#ifndef XQML_PARSERFRAMEWORK_H
#define XQML_PARSERFRAMEWORK_H

#include <xqml/parser.h>
#include <xqml/xqmlsymbol.h>
#include <common/contexts.h>
#include <string>
#include <stack>

namespace xqML
{
  class Parser;
}

namespace xqMLImpl
{
  using namespace xqMLImpl;

  /** \class ParserFramework parserframework.h Parser/parserframework.h
      \brief Implementation class for xqML::Parser
      \ingroup impl @{
  */
  class ParserFramework
  {
    friend class xqML::Parser; //!< Needed for Parser methods to access this class

    Context context;                     //!< Keeps track of the document structure
    std::istream & document;             //!< The document to parse
    std::ostream & outstream;
    xqML::xqMLSymbol s;                  //!< Temporary store for symbols being processed
    std::string buffer;                  //!< Temporary store for data being processed
    std::stack<std::string> openELstack; //!< Stack to hold open elements
    bool isEmpty;                        //!< Tracks current empty element

    //!Constructor
    ParserFramework()
      : context(ready), document(std::cin), outstream(std::cout), s(0)
    {}
    
    //! Set the input source to be parsed
    void setSource(std::istream &);
    
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
  };
  /** @} */
}
#endif
