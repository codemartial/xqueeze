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

/** \file Parser/mapping.C
 * \brief Implementation of xqMLParser::Mapping.
 */
#include <xqml-build/exceptions.h>
#include "parser_impl.h"
namespace xqMLParser
{
  using std::cout;
  using std::string;
  using std::ostream;
  using xqML::xqMLSymbol;

/** This constructor enables translation of symbol values to their
 *  names according to the xqA spec given as argument. Calls
 *  buildMap() to construct a lookup table and initializes resolveFunc
 *  to getNameString.
 */
  Mapping::Mapping(istream & source_file)
  {
    maxEL = maxEE = maxAT = maxAP = maxVA = maxEN = 0;
    resolveFunc = & xqMLParser::Mapping::getNameString;
    buildMap(source_file);
  }

/** This constructor disables translation of symbol values to their
 *  names. Initializes resolveFunc to getDecimalValue
 */
  Mapping::Mapping()
  {
    resolveFunc = & xqMLParser::Mapping::getDecimalValue;
  }

/**  Fills up the map structures of the associated mapping object with
 *  specifications read from the istream argument */
  void Mapping::buildMap(istream & source) 
  {
    while(source.get() != '\x1E'); //Hunt for preamble.
    string name;
    unsigned int * typeMax = NULL;
    xqMLSymbol s;
    while(source>>s) 
      switch (s.getCode()){
      case 0x02:
	while(source.get() != '\x1E');
	break;
      case 0x3C:
	return;
      case 0x30:
	typeMax = &maxEL;
	source.get();
	break;
      case 0x32:
	typeMax = &maxEE;
	source.get();
	break;
      case 0x34:
	typeMax = &maxAT;
	source.get();
	break;
      case 0x36:
	typeMax = &maxAP;
	source.get();
	break;
      case 0x38:
	typeMax = &maxVA;
	source.get();
	break;
      case 0x3A:
	typeMax = &maxEN;
	source.get();
	break;
      case 0x22: //FIXME: I don't wanna be bothered with comments here - fix the spec!
	while(source.get() != '\x1E'); //Read till the end of comment
	while(source.get() != '\x1E'); //Read till the next markup flag
	break;
      case 0x20: //Possible Prolog
	getline(source, name, '\x1E'); // PITarget
	if(name=="xqa") xqAStart = true;
	while(source.get() != '\x1E'); // PIData
	while(source.get() != '\x1E'); //Read till the next markup flag
	break;
      default:
	if(!xqAStart) 
	  throw xqML::IllegalIdentifier(s.getCode());
	getline(source, name, '\x1E');
	map[s.getCode()] = name;
	*typeMax = *typeMax > s.getCode() ? *typeMax : s.getCode();
	//	std::cerr<<"Added "<<name<<" ("<<s.getCode()<<")\n";
      }
//     std::cerr<<"maxEL = "<<maxEL<<'\n';
//     std::cerr<<"maxEE = "<<maxEE<<'\n';
//     std::cerr<<"maxAT = "<<maxAT<<'\n';
//     std::cerr<<"maxAP = "<<maxAP<<'\n';
//     std::cerr<<"maxVA = "<<maxVA<<'\n';
//     std::cerr<<"maxEN = "<<maxEN<<'\n';
  }

  string Mapping::getNameString(unsigned int code)
  {
    if(map.find(code) == map.end()) throw xqML::IllegalIdentifier(code);
    return map[code];
  }
  
  string Mapping::getDecimalValue(unsigned int code)
  {
    std::ostringstream codevalue;
    codevalue<<code;
    return codevalue.str();
  }
  
} // Closing NameSpace xqMLParser
