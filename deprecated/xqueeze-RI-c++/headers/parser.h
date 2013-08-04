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

/** \file parser.h
 * \brief Definition of xqML::Parser - a generic parser.
 */
#ifndef XQML_PARSER_H
#define XQML_PARSER_H
#include <iostream>
#include <string>

namespace xqMLImpl {
  class ParserFramework;
}

namespace xqML 
{
  /** \ingroup parser @{ */

  /** \class Parser parser.h xqml/parser.h
      \brief Provides API to the client for parsing xqML Documents.
  */
  class Parser 
  {
  public:
    Parser();
    
    //! Parse an input stream
    virtual void parse(std::istream &);
      
    //! Parse a document from a System Identifier (URI)
    virtual void parse(const std::string &);
    
    virtual ~Parser();
  private:
    xqMLImpl::ParserFramework * parser_;
  };

  /** @} */
}
#endif
