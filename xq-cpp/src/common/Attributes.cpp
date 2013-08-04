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

/** \file Attribtues.cpp
 * \brief Implementation of xqML::Attributes
 */

#include <algorithm>
#include <xqml/Attributes.hpp>

namespace xqML
{

  namespace 
  {
    struct EqQName
    {
      EqQName(const string & q_name) : q_name_(q_name) 
      {}
      bool operator()(const Attribute & att)
      {
	return att.q_name == q_name_;
      }
      string q_name_;
    };
    
    struct EqURI
    {
      EqURI(const string & uri, const string & name)
	: uri_(uri), name_(name)
      {}
      bool operator()(const Attribute & att)
      {
	return att.uri == uri_ && att.name == name_;
      }
      string uri_, name_;
    };
    
  }
    
  int Attributes::getIndex(const string & q_name)
  {
    iterator i = std::find_if(this->begin(), this->end(), EqQName(q_name));
    return i != this->end() ? i - this->begin() : -1;
  }
    
  int Attributes::getIndex(const string & uri, const string & name)
  {
    iterator i = std::find_if(this->begin(), this->end(), EqURI(uri, name));
    return i != this->end() ? i - this->begin() : -1;
  }
  
  size_t Attributes::getLength() const
  {
    return this->size();
  }
  
  string Attributes::getLocalName(size_t index)
  {
    return (*this)[index].name;
  }
  
  string Attributes::getQName(size_t index)
  {
    return (*this)[index].q_name;
  }
  
  string Attributes::getType(size_t index)
  {
    return index < this->size() ? "CDATA" : "";
  }
  
  string Attributes::getType(const string & q_name)
  {
    return getType(getIndex(q_name));
  }
  
  string Attributes::getType(const string & uri, const string & name)
  {
    return getType(getIndex(uri, name));
  }
  
  string Attributes::getURI(size_t index)
  {
    return index < this->size() ? (*this)[index].uri : "";
  }
  
  string Attributes::getValue(size_t index)
  {
    return index < this->size() ? (*this)[index].value : "";
  }
  
  string Attributes::getValue(const string & q_name)
  {
    return getValue(getIndex(q_name));
  }
  
  string Attributes::getValue(const string & uri, const string & name)
  {
    return getValue(getIndex(uri, name));
  }
  
  bool Attributes::addAttribute(const Attribute & att)
  {
    if (getIndex(att.uri, att.name) == -1 && getIndex(att.q_name) == -1) {
      this->push_back(att);
      return true;
    }
    return false;
  }
  
}
