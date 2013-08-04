/*
 * Author: Tahir Hashmi
 * 
 * Copyright (C) 2002, 2003 Xqueeze Developers
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

/**\file parser.C
 * \brief Implementation of xqML::Parser
 */

#include <xqml/parser.h>
#include <xqml/exceptions.h>
#include <Parser/parserframework.h>
#include <iostream>
#include <string>

namespace xqML 
{
  /** The constructor creates and initializes a Parser object that's
   *  ready to process the standard input stream. (Use parse()
   *  functions to parse other streams).
   */
  Parser::Parser()
  {
    parser_ = new xqMLImpl::ParserFramework();
  }

  /** Parses xqML document(s) in the input stream. For the same 
   *  parser object and same source, this function will parse all
   *  documents if the stream contains more than one document.
   *  \promises
   *     - Will parse one xqML document in the input stream
   *     - Will report syntax errors through exceptions
   *     - Will return only when all documents in the stream
   *     have been successfully parsed
   *     .
   *  \requires
   *     - The caller must not call this function while a parse is
   *     already underway
   *     .
   *  \throws xqML::IllegalContext
   *  \throws xqML::IllegalSymbol
   *  \throws std::exception
   */
  void Parser::parse(std::istream & documentStream) 
  {
    parser_->setSource(documentStream);
    parser_->parseDocument();
  }
  
  /** This allows you to parse a stream derived from the supplied
      system URI. (Yet to be implemented)
      \exception xqML::IllegalContext
   */
  void Parser::parse(const std::string & systemURI)
  {
    ;
  }
  
  Parser::~Parser()
  {
    delete parser_;
  }
}
