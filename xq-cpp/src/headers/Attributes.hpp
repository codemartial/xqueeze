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

/** \file Attributes.hpp
 * \brief Specification of SAX2 Attributes Implementation
 */
#ifndef XQML_ATTRIBUTES_HPP
#define XQML_ATTRIBUTES_HPP

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace xqML
{
  
  struct Attribute
  { string uri, name, q_name, value; };
  
  class Attributes : public vector<Attribute>
  {
  public:
    int getIndex(const string & q_name);
    int getIndex(const string & uri, const string & name);
    size_t getLength() const;
    string getLocalName(size_t index);
    string getQName(size_t index);
    string getType(size_t index);
    string getType(const string & q_name);
    string getType(const string & uri, const string & name);
    string getURI(size_t index);
    string getValue(size_t index);
    string getValue(const string & q_name);
    string getValue(const string & uri, const string & name);
    bool addAttribute(const Attribute & att);
  };
}


// End of Include Guard
#endif
