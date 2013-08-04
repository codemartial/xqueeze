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

/** \file Writer/mapping.C
 * \brief Implementation of xqMLWriter::Mapping.
 */
#include "writer_impl.h"
#include <xqml-build/exceptions.h>
#include <xqml-build/xqmlsymbol.h>

namespace xqMLWriter
{
  using std::cout;
  using std::string;
  using std::ostream;
  using xqML::xqMLSymbol;

/**  Returns the name corresponding to the argument */
  string Mapping::typeName(xqMLType type)
  {
    switch (type){
    case EL:
      return "Element";
      break;
    case EE:
      return "Empty Element";
      break;
    case AT:
      return "Attribute";
      break;
    case AP:
      return "Attribute with Predefined Value";
      break;
    case VA:
      return "Attribute Value";
      break;
    case EN:
      return "Entity Reference";
      break;
    default:
      return "unknown type";
    }
  }
 
/** Constructor calls buildMap() */
  Mapping::Mapping(istream & source_file)
  {
    buildMap(source_file);
  }

/**  Fills up the map structures of the associated mapping object with
 *  specifications read from the istream argument */
  void Mapping::buildMap(istream & source) 
  {
    while(source.get() != '\x1E'); //Hunt for prolog
    SymbolMap * map_ = NULL; 
    string name;
    xqMLSymbol s;
    while(source>>s) 
      switch (s.getCode()){
      case 0x02:
	while(source.get() != '\x1E');
	break;
      case 0x3C:
	std::cerr<<"End of map-building\n";
	return;
      case 0x30:
	map_ = &EL_map;
	source.get();
	std::cerr<<"Elements\n";
	break;
      case 0x32:
	map_ = &EE_map;
	source.get();
	std::cerr<<"Empty Elements\n";
	break;
      case 0x34:
	map_ = &AT_map;
	source.get();
	std::cerr<<"Attributes\n";
	break;
      case 0x36:
	map_ = &AP_map;
	source.get();
	std::cerr<<"Attributes WPV\n";
	break;
      case 0x38:
	map_ = &VA_map;
	source.get();
	std::cerr<<"Attribute Values\n";
	break;
      case 0x3A:
	map_ = &EN_map;
	source.get();
	std::cerr<<"Entity References\n";
	break;
      case 0x22: //FIXME: I don't wanna be bothered with comments here - fix the spec!
	std::cerr<<"Comment\n";
	while(source.get() != '\x1E'); //Read till the end of comment
	while(source.get() != '\x1E'); //Read till the next markup flag
	break;
      case 0x20: //Possible Prolog
	std::cerr<<"Possible prolog... ";
	getline(source, name, '\x1E'); // PITarget
	if(name=="xqa") { 
	  xqAStart = true;
	  std::cerr<<"yes\n";
	}
	while(source.get() != '\x1E'); // PIData
	while(source.get() != '\x1E'); //Read till the next markup flag
	break;
      default:
	if(!xqAStart)
	  throw xqML::IllegalIdentifier(s.getCode());
	getline(source, name, '\x1E');
	(*map_)[name] = s.getCode();
	std::cerr<<"\t\t"<<name<<" ("<<(*map_)[name]<<"/"<<s.getCode()<<")\n";
      }
  }

/** Lookup a symbol matching the given name and type
 *  
 * Returns the symbol for the first argument from the map structure
 * pointed to by the third argument. The second argument is for
 * determining the type for throwing the exception.
 *
 * Throws exception IllegalIdentifier if lookup fails */
  inline unsigned int Mapping::GetSymbol(const string & name, xqMLType type, SymbolMap * map_)
  {
    if((map_->find(name)) != map_->end())
      return (*map_)[name];
    throw xqML::IllegalIdentifier(name, typeName(type)); 
  }
  
  xqMLSymbol Mapping::GetEmptyElement(const string & name)
  {
    return xqMLSymbol(GetSymbol(name, EE, &EE_map));
  }
  
  xqMLSymbol Mapping::GetElement(const string & name)
  {
    return xqMLSymbol(GetSymbol(name, EL, &EL_map));
  }

  xqMLSymbol Mapping::GetAttribute(const string & name)
  {
    return xqMLSymbol(GetSymbol(name, AT, &AT_map));
  }

  xqMLSymbol Mapping::GetAttributeWPV(const string & name)
  {
    return xqMLSymbol(GetSymbol(name, AP, &AP_map));
  }

  xqMLSymbol Mapping::GetAttributeValue(const string & name)
  {
    return xqMLSymbol(GetSymbol(name, VA, &VA_map));
  }

  xqMLSymbol Mapping::GetEntityReference(const string & name)
  {
    return xqMLSymbol(GetSymbol(name, EN, &EN_map));
  }
  
} // Closing NameSpace xqMLWriter
