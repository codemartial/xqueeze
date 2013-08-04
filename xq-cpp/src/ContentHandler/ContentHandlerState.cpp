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

/** \file ContentHandlerState.cpp
 * \brief Implementation of TranslatingContentHandler state m/c
 */

#include <algorithm>
#include <utility>
#include <xqml/TranslatingContentHandler.hpp>
#include <xqml/xqmlsymbol.hpp>
#include <xqml/exceptions.hpp>
#include "ContentHandlerState.hpp"
#include "CHContext.hpp"
#include "States.hpp"

namespace 
{
  struct LargestSecond
  {
    bool operator()(std::pair<const string, uint32_t> & a, std::pair<const string, uint32_t> & b)
    {
      return a.second < b.second;
    }
  };
}

namespace xqML
{
  CHContext::CHContext(XqaHandler & handler, ostream & document, bool dynamic)
    : state_box(CHStateBox(*this)),
      ch_state(state_box.getState(Default)),
      xqa_handler(handler),
      out(document),
      do_dynamic_maps(dynamic),
      current_prefix(0)
  {}
    
  uint32_t CHContext::lookup(const string & uri, const string & name)
  {
    URIMap::iterator i = uri_map.find(uri);
    if(i != uri_map.end() && i->second->find(name) != i->second->end())
	return (*i->second)[name];
    return 0;
  }
  
  uint32_t CHContext::dynamicLookup(const string & uri, const string & name)
  {
    if (!dyn_maps[uri].size()) {
      uint32_t last_code = uri_map.find(uri) != uri_map.end() ?
	last_code = (--(uri_map[uri]->end()))->first : 256;
      dyn_maps[uri][name] = xqMLSymbol::nextSymbol(last_code);
    } // We just added the first name to the corresponding map
    
    if (dyn_maps[uri].find(name) == dyn_maps[uri].end())
      dyn_maps[uri][name] = xqMLSymbol::nextSymbol(std::max_element(dyn_maps[uri].begin(), dyn_maps[uri].end(), LargestSecond())->second);
    return dyn_maps[uri][name];
  }
  
  void ContentHandlerState::throwError()
  {
    throw xqML::IllegalContext("", "", ""); // Later.
  }
  
  CHStateBox::CHStateBox(CHContext & ctx)
  {
    // Number of states can be found by Default
    // because it is enumerated last of all.
    statebox.resize(Default);
    
    statebox[Decl] = new CHDecl(ctx);
    statebox[Prolog] = new CHProlog(ctx);
    statebox[PrefixMapping] = new CHPrefixMapping(ctx);
    statebox[StartElement] = new CHStartElement(ctx);
    statebox[EndElement] = new CHEndElement(ctx);
    statebox[Content] = new CHContent(ctx);
    statebox[Epilog] = new CHEpilog(ctx);
    statebox[Default] = statebox[Decl];
  }

  CHStateBox::~CHStateBox()
  {
    for (state_map::iterator i = statebox.begin(); i != statebox.end(); ++i)
      delete *i;
  }
  
}

// End of Include Guard
