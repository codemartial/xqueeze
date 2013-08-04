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

/** \file CHContent.cpp
 * \brief Implementation of CHContent
 */

#include "States.hpp"

namespace xqML
{
  
  void CHContent::startElement(const string & uri, const string & name, const string & q_name, const Attributes & atts)
  {
    ch_context.ch_state = ch_context.state_box.getState(StartElement);
    ch_context.ch_state->startElement(uri, name, q_name, atts);
  }
  
  void CHContent::endElement(const string & uri, const string & name, const string & q_name)
  {
    ch_context.ch_state = ch_context.state_box.getState(EndElement);
    ch_context.ch_state->endElement(uri, name, q_name);
  }
    
  void CHContent::startPrefixMapping(const string & prefix, const string & uri)
  {
    ch_context.ch_state = ch_context.state_box.getState(PrefixMapping);
    ch_context.ch_state->startPrefixMapping(prefix, uri);
  }
  
  void CHContent::processingInstruction(const string & target, const string & data)
  {
    putReservedSymbol(ch_context.out, 0x1E);
    putReservedSymbol(ch_context.out, 0x20)<<target;
    putReservedSymbol(ch_context.out, 0x1E)<<data;
    putReservedSymbol(ch_context.out, 0x1E);
  }
  
  void CHContent::skippedEntity(const string & name)
  {
    xqMLSymbol en_symbol = ch_context.lookup(ch_context.current_uri, name);
    if(!en_symbol) throw IllegalIdentifier(ch_context.current_uri, name);
    
    putReservedSymbol(ch_context.out, 0x22);
    ch_context.out<<en_symbol;
  }
  
  void CHContent::characters(const string & characters)
  {
    ch_context.out<<characters;
  }

}
