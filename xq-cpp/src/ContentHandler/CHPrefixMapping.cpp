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

/** \file CHPrefixMapping.cpp
 * \brief Implementation of CHPrefixMapping
 */

#include <xqml/exceptions.hpp>
#include "States.hpp"

namespace xqML
{
  void CHPrefixMapping::startPrefixMapping(const string & prefix, const string & uri)
  {
    putReservedSymbol(ch_context.out, 0x1E);
    putReservedSymbol(ch_context.out, 0x1C)<<prefix;
    putReservedSymbol(ch_context.out, 0x1E)<<uri;
    
    XqA * p_xqa = ch_context.xqa_handler.getXqA(uri);
    if (p_xqa) ch_context.uri_map[uri] = p_xqa;
    else {
      if(!ch_context.do_dynamic_maps) throw XqaNotAvailable(uri);
      ch_context.dyn_maps[uri];
    }
    last_code = xqML::xqMLSymbol::nextSymbol(last_code);
    ch_context.prefix_codes[uri] = last_code;
    ch_context.prefix_uri_map[prefix] = uri;
  }
  
  void CHPrefixMapping::startElement(const string & uri, const string & name, const string & q_name, const Attributes & atts)
  {
    ch_context.ch_state = ch_context.state_box.getState(StartElement);
    ch_context.ch_state->startElement(uri, name, q_name, atts);
  }
  
}
