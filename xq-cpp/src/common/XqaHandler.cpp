/*
 * Author: Tahir Hashmi
 * 
 * Copyright (C) 2004, Xqueeze Developers
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

/** \file XqaHandler.cpp
 * \brief Implementation of XqaHandler
 */

#include <xqml/xqmlsymbol.hpp>
#include <xqml/exceptions.hpp>
#include <xqml/XqaHandler.hpp>

namespace xqML
{
  XqA * XqaHandler::getXqA(const string & uri)
  {
    try{
      XqACache::iterator i = known_maps.find(uri);
      if(i != known_maps.end()) return &(i->second);
      known_maps[uri] = buildMap(std::auto_ptr<istream> (xqa_finder(uri)));
      return & known_maps[uri];
    }
    catch (std::ios_base::failure & e) {
      throw xqML::XqaNotAvailable(uri);
    }
  }
  
  bool XqaHandler::registerXqA(const string & uri, std::auto_ptr<istream> spec_stream)
  {
    return registerXqA(uri, spec_stream.get());
  }
  
  bool XqaHandler::registerXqA(const string & uri, istream * const spec_stream)
  {
    if(known_maps.find(uri) != known_maps.end()) return false;
    try{
      known_maps[uri] = buildMap(spec_stream);
      return true;
    }
    catch (std::ios_base::failure & e) {
      throw xqML::XqaNotAvailable(uri);
    }
  }
    
  XqA XqaHandler::buildMap(std::auto_ptr<istream> source)
  {
    return buildMap(source.get());
  }
  
  XqA XqaHandler::buildMap(istream * const source)
  {
    XqA symbol_map;
    bool xqa_start = false;
    while(source->get() != '\x1E'); //Hunt for prolog
    string name;
    xqML::xqMLSymbol s;
    while(*source>>s) 
      switch (static_cast<uint32_t>(s)) {
      case 0x00:
	break;
      case 0x40:
	return symbol_map;
      case 0x20: //Possible Prolog
	getline(*source, name, '\x1E'); // PITarget
	if(name=="xqa")
	  xqa_start = true;
	while(source->get() != '\x1E'); // PIData
	while(source->get() != '\x1E'); //Read till the next markup flag
	break;
      default:
	getline(*source, name, '\x1E');
	if(xqa_start)
	  symbol_map.insert(s, name);
      }
    throw std::ios_base::failure::failure("Unfinished Specs in stream");
  }
}
