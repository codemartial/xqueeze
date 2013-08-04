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

/** \file exceptions.h
 * \brief Interface specifications for xqML exceptions
 */
#ifndef XQML_EXCEPTIONS_H
#define XQML_EXCEPTIONS_H
#include <string>
#include <ios>
#include <xqml/xqmlsymbol.h>

namespace xqML
{
  using std::string;
  using xqML::xqMLSymbol;
  using std::streampos;

  //! Exception thrown when lookup for an identifier fails.
  struct IdentifierNotFound
  {
  public:
    string identifier;    //!< indentifier that could not be found
    string type;          //!< type name of the identifier
    streampos fail_point; //!< position in the stream where failure happened

    //! Gives name and type of identifier for which a matching symbol
    //! was not found. Reports the position at which the exception was caused.
    IdentifierNotFound(const string & id, const string & id_type, const streampos position)
      : identifier(id), type(id_type), fail_point(position) {}
  };
  
  //! Exception thrown when lookup for a symbol fails
  struct SymbolNotFound
  {
    xqMLSymbol s;      //!< value of the offending symbol
    streampos fail_point; //!< position of the stream where failure happened

    //! Gives the value of the xqML Symbol that could not be
    //! translated. Reports the position at which the exception was
    //! caused.
    SymbolNotFound(const xqMLSymbol value, streampos position)
      : s(value), fail_point(position) {};
  };

  //! Exception thrown due to bad document structure.
  struct IllegalContext
  {
  public:
    string context;       //!< name of the current context of the document
    string identifier;    //!< name of the offending identifier
    string type;          //!< type name of the identifier
    streampos fail_point; //!< position in the stream where failure happened

    //! Gives context, name of identifier and type of identifier where
    //! bad structure was found. Reports the position at which the exception was caused.
    IllegalContext(const string & ill_context, const string & id, const string & id_type, const streampos position)
      : context(ill_context), identifier(id), type(id_type), fail_point(position) {}
  };

  //! Exception thrown due to occurence of unknown/unexpected %xqML
  //! Symbols
  struct IllegalSymbol
  {
    string context;       //!< name of the current context of the document
    xqMLSymbol s;         //!< value of the offending symbol
    streampos fail_point; //!< postition of the stream where failure happened
    
    //! Gives context and value of the unknown/unexpected %xqML Symbol
    //! found. Reports the position at which the exception was caused.
    IllegalSymbol(const string & ill_context, const xqMLSymbol value, const streampos position)
      : context(ill_context), s(value), fail_point(position) {};
  };
}
#endif
