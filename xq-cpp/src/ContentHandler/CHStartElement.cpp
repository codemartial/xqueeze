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

/** \file CHStartElement.cpp
 * \brief Implementation of CHStartElement
 */

#include <sstream>
#include "States.hpp"

using std::ostringstream;

namespace xqML
{
  
  void CHStartElement::unqueueTag()
  {
    putReservedSymbol(ch_context.out, 0x1E);
    if (flags & 0x0E) putReservedSymbol(ch_context.out, flags);
    ch_context.out<<queued_tag;
    flags = 0x30;
    queued_tag = "";
  }
  
  void CHStartElement::startElement(const string & uri, const string & name, const string & q_name, const Attributes & atts)
  {
    if (queued_tag.size()) unqueueTag();
    (this->*real_se_call)(uri, name, q_name, atts);    
  }
  
  void CHStartElement::seInit(const string & uri, const string & name, const string & q_name, const Attributes & atts)
  {
    if(name != "") real_se_call = & xqML::CHStartElement::seURI;
    else real_se_call = & xqML::CHStartElement::sePrefix;
    (this->*real_se_call)(uri, name, q_name, atts);
  }
  
  void CHStartElement::seURI(const string & uri, const string & name, const string & q_name, const Attributes & atts)
  {
    ostringstream dyn_reg;
    ostringstream element;
    ostringstream attributes;

    if (uri != ch_context.current_uri) {
      ch_context.current_uri = uri;
      flags |= 0x04;
      element<<xqMLSymbol(ch_context.prefix_codes[uri]);
    }

    xqMLSymbol el_symbol = ch_context.lookup(uri, name);
    if (!el_symbol) {
      if (ch_context.do_dynamic_maps) {
	el_symbol = ch_context.dynamicLookup(uri, name);
	putReservedSymbol(dyn_reg, 0x1E);
	putReservedSymbol(dyn_reg, 0x2A)<<el_symbol<<name;
      }
      else throw IllegalIdentifier(uri, name);
    }
    element<<el_symbol;
    
    size_t i = atts.getLength();
    if (i)
      if (atts[0].name != "")
	while(i--)
	  writeAttributeURI(atts[i], dyn_reg, attributes);
      else
	while(i--)
	  writeAttributePfx(atts[i], dyn_reg, attributes);
    
    ch_context.out<<dyn_reg.str();
    queued_tag =  element.str() + attributes.str();

    ch_context.open_el_stack.push(pair<string, string>(uri, name));
  }
  
  void CHStartElement::sePrefix(const string & uri, const string & name, const string & q_name, const Attributes & atts)
  {
    size_t colon_loc = q_name.find(':');
    if(colon_loc < q_name.size()) // Prefix exists
      seURI(ch_context.prefix_uri_map[q_name.substr(0, colon_loc)],
	    q_name.substr(colon_loc + 1, q_name.size() - colon_loc -1),
	    q_name,
	    atts);
    else
      seURI(uri, q_name, q_name, atts);
  }
  
  void CHStartElement::writeAttributeURI(const Attribute & att, ostream & dyn_reg, ostream & attributes)
  {
    bool write_nss = ch_context.current_uri != att.uri;
    
    xqMLSymbol at_symbol = ch_context.lookup(att.uri, att.name);
    if (!at_symbol) {
      if (ch_context.do_dynamic_maps) {
	at_symbol = ch_context.dynamicLookup(att.uri, att.name);
	putReservedSymbol(dyn_reg, 0x1E);
	if(!write_nss)
	  putReservedSymbol(dyn_reg, 0x2A);
	else
	  putReservedSymbol(dyn_reg, 0x28)<<xqMLSymbol(ch_context.prefix_codes[att.uri]);
	dyn_reg<<at_symbol<<att.name;
      }
      else throw IllegalIdentifier(att.uri, att.name);
    }

    xqMLSymbol at_value = ch_context.lookup(att.uri, att.value);
    if (!at_value) {
      if (!write_nss)
	putReservedSymbol(attributes, 0x16);
      else 
	putReservedSymbol(attributes, 0x14)<<xqMLSymbol(ch_context.prefix_codes[att.uri]);
      attributes<<at_symbol<<att.value;
      putReservedSymbol(attributes, 0x16);
    }
    else {
      if (!write_nss)
	putReservedSymbol(attributes, 0x1A);
      else 
	putReservedSymbol(attributes, 0x18)<<xqMLSymbol(ch_context.prefix_codes[att.uri]);
      attributes<<at_symbol<<at_value;
    }
  }
  
  void CHStartElement::writeAttributePfx(const Attribute & att, ostream & dyn_reg, ostream & attributes)
  {
    Attribute new_att = att;
    size_t colon_loc = att.q_name.find(':');
    
    if(colon_loc < att.q_name.size()) { // Prefix exists
      new_att.uri = ch_context.prefix_uri_map[att.q_name.substr(0, colon_loc)];
      new_att.name = att.q_name.substr(colon_loc + 1, att.q_name.size() - colon_loc -1);
    }
    else
      new_att.name = att.q_name;
    writeAttributeURI(new_att, dyn_reg, attributes);
  }
    
  void CHStartElement::endElement(const string & uri, const string & name, const string & q_name)
  {
    flags |= 0x02;
    unqueueTag();
    ch_context.ch_state = ch_context.state_box.getState(EndElement);
    dynamic_cast<CHEndElement *>(ch_context.ch_state)->endElement(uri, name, q_name, true);
  }
  
  void CHStartElement::processingInstruction(const string & target, const string & data)
  {
    unqueueTag();
    ch_context.ch_state = ch_context.state_box.getState(Content);
    ch_context.ch_state->processingInstruction(target, data);
  }
  
  void CHStartElement::startPrefixMapping(const string & prefix, const string & uri)
  {
    unqueueTag();
    ch_context.ch_state = ch_context.state_box.getState(PrefixMapping);
    ch_context.ch_state->startPrefixMapping(prefix, uri);
  }
  
  void CHStartElement::skippedEntity(const string & name)
  {
    unqueueTag();
    ch_context.ch_state = ch_context.state_box.getState(Content);
    ch_context.ch_state->skippedEntity(name);
  }
  
  void CHStartElement::characters(const string & characters)
  {
    unqueueTag();
    ch_context.ch_state = ch_context.state_box.getState(Content);
    ch_context.ch_state->characters(characters);
  }

}
