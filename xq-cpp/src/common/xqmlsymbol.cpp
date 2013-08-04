/*
 * Author: Tahir Hashmi
 * 
 * Copyright (C) 2002, Xqueeze Developers
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

/** \file xqmlsymbol.cpp
 * \brief Implementation of xqML::xqMLSymbol
 */
#include <iostream>
#include <xqml/xqmlsymbol.hpp>

namespace xqML {
/** \class xqMLSymbol xqmlsymbol.h xqml/xqmlsymbol.h */

/** This operator is capable of writing any unsigned integer in
 * network byte order since it doesn't check whether the xqMLSymbol is
 * indeed valid (i.e. properly uses the lsb as a continuation flag)
 */
  ostream & operator<<(ostream & stream, xqMLSymbol symbol)
  {
    char octets[sizeof(symbol.code)];
    short int i = 0;
    octets[i] = static_cast<unsigned char> (symbol.code & 0xFF);
    symbol.code >>= 8;
    for(i = 1; symbol.code; ++i) {
      octets[i] = static_cast<unsigned char> (symbol.code & 0xFF);
      symbol.code >>= 8;
    }
    for(--i; i >= 0; --i)
      stream.put(octets[i]);
    return stream;
  }
  
/** Can't handle an xqMLSymbol whose value is greater than the
 *  largest value an unsigned integer can hold on the platform used.
 */
  istream & operator>>(istream & stream, xqMLSymbol & symbol)
  {
    symbol.code = 0;
    char octet;
    stream.get(octet); // Atleast one octet is to be uncoditionally read
    symbol.code = static_cast<unsigned char>(octet);
    while (symbol.code & 0x01 && stream.get(octet)) {
      symbol.code <<= 8;
      symbol.code |= static_cast<unsigned char>(octet);
    }
    return stream;
  }
  
  uint32_t xqMLSymbol::nextSymbol(uint32_t code)
  {
    code += 2;
    for (int i=1; code/(1<<i*8); ++i)
      if(!((code/(1<<i*8)) & 0x01))
	code += 1<<i*8;
    return code;
  }
  
}
