/*
 * Author: Tahir Hashmi
 * 
 * Copyright (C) 2003 Xqueeze Developers
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

/** \file unit.h
 * \brief Defines a syntactic unit of xqML encoding
 */

#ifndef XQML_UNIT_H
#define XQML_UNIT_H

#include <iostream>
#include <string>

namespace xqMLImpl 
{
  using std::string;
  using std::istream;
  using std::ostream;
  
  //! The interface of a syntactic unit of xqML encoding
  class Unit 
  {
  public:
    //! Encode \c this unit into a string.
    virtual ostream & encode(ostream &) const = 0;
    
    //! Decode the string into \c this unit
    virtual istream & decode(istream &) = 0;

    //! Encode and write out to the given ostream
    friend ostream & operator<<(ostream & o, const Unit & u);

    //! Read from the given istream and decode
    friend istream & operator>>(istream & i, Unit & u);

    Unit();
    
    virtual ~Unit();
  };
  
  ostream & operator<<(ostream & o, const Unit & u)
  {
    u.encode(o);
    return o;
  }
  
  istream & operator>>(istream & i, Unit & u)
  {
    u.decode(i);
    return i;
  }
  
}
#endif
