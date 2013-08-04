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

/** \file CHEndElement.cpp
 * \brief Implementation of CHEndElement
 */

#include "States.hpp"
#include <cassert>

namespace xqML
{
  void CHEndElement::startElement(const string & uri, const string & name, const string & q_name, const Attributes & atts)
  {
    ch_context.ch_state = ch_context.state_box.getState(StartElement);
    if(elements_to_close == 1) {
      dynamic_cast<CHStartElement *>(ch_context.ch_state)->setCPFlag();
      elements_to_close = 0;
    }
    else closePendingElements();
    ch_context.ch_state->startElement(uri, name, q_name, atts);
  }

  void CHEndElement::endElement(const string & uri, const string & name, const string & q_name, bool handle_ee)
  {
    if (elements_to_close == 255) closePendingElements();
    ++elements_to_close;
    if (handle_ee) {
      pop_elements();
      ch_context.ch_state = ch_context.state_box.getState(Content);
    }
  }

  void CHEndElement::endElement(const string & uri, const string & name, const string & q_name)
  {
    this->endElement(uri, name, q_name, false);
  }
  
  void CHEndElement::startPrefixMapping(const string & prefix, const string & uri)
  {
    closePendingElements();
    ch_context.ch_state = ch_context.state_box.getState(PrefixMapping);
    ch_context.ch_state->startPrefixMapping(prefix, uri);
  }
  
  void CHEndElement::endPrefixMapping(const string & prefix)
  {
    if (ch_context.prefix_uri_map.find(prefix) != ch_context.prefix_uri_map.end()) {
      ch_context.uri_map.erase(ch_context.prefix_uri_map[prefix]);
      ch_context.prefix_uri_map.erase(prefix);
    }
  }
  
  void CHEndElement::closePendingElements()
  {
    write_etag();
    pop_elements();
  }
  
  void CHEndElement::write_etag()
  {
    putReservedSymbol(ch_context.out, 0x1E);
    putReservedSymbol(ch_context.out, 0x30);
    putReservedSymbol(ch_context.out, elements_to_close);
  }

  void CHEndElement::pop_elements()
  {
    // elements_to_close > 0 is guaranteed
    if(elements_to_close > ch_context.open_el_stack.size())
      throw IllegalContext("endElement", ch_context.current_uri, "requested too many elements to close");
    
    do
      ch_context.open_el_stack.pop();
    while (--elements_to_close);
      
    if(!ch_context.open_el_stack.empty())
      ch_context.current_uri = ch_context.open_el_stack.top().first;
    else
      ch_context.ch_state = ch_context.state_box.getState(Epilog);
  }
  
  void CHEndElement::processingInstruction(const string & target, const string & data)
  {
    closePendingElements();
    ch_context.ch_state = ch_context.state_box.getState(Content);
    ch_context.ch_state->processingInstruction(target, data);
  }
  
  void CHEndElement::skippedEntity(const string & name)
  {
    closePendingElements();
    ch_context.ch_state = ch_context.state_box.getState(Content);
    ch_context.ch_state->skippedEntity(name);
  }
  
  void CHEndElement::characters(const string & characters)
  {
    closePendingElements();
    ch_context.ch_state = ch_context.state_box.getState(Content);
    ch_context.ch_state->characters(characters);
  }
  
  void CHEndElement::endDocument()
  {
    closePendingElements();
    ch_context.ch_state = ch_context.state_box.getState(Epilog);
  }
  
}
