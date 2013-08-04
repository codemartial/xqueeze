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

/** \file wideio.C
 *  \brief Implementation of Xqueeze 16-bit I/O routines
 */

#include <iostream>
#include <string>
#include <xqml/xqtypes.h>
#include <common/wideio.h>

namespace xqMLImpl
{
  using std::string;
  using std::istream;
  using std::ostream;
  using xqML::uint16_t;
  using xqMLImpl::WideIO;

  uint16_t WideIO::get(istream & i)
  {
    uint16_t tmp = i.get();
    return (i.get()<<8) | tmp;
  }

  istream & WideIO::ignoreUntil(istream & i, const uint16_t wChar)
  {
    initPair(wChar);
    while(i && i.get() != pair[0]); // Look for the first octet

    while (i.get() != pair[1])     // Keep trying until second octet matches
      while(i && i.get() != pair[0]); // WEIRD LOGIC ALERT!!!

    return i;
  }

  istream & WideIO::getline(istream & i, string & buffer, const uint16_t wChar)
  {
    initPair(wChar);
    std::getline(i, buffer, pair[0]); // Read till the first octet
    if(!(buffer.size()%2)) {
      if (i.peek() == pair[1]) {
	i.ignore();
	return i;
      }
    }
    string tempbuf;
    while (i && (buffer.size()%2 || i.peek() != pair[1]) ) {
      buffer += pair[0];
      if (buffer.size()%2)
	buffer += i.get();
      std::getline(i, tempbuf, pair[0]);
      buffer += tempbuf;
    }
    //;
    return i;
  }

  void WideIO::initPair_LE(uint16_t wChar)
  {
    pair[0] = wChar & 255;
    pair[1] = wChar >> 8;
  }

  void WideIO::initPair_BE(uint16_t wChar)
  {
    pair[0] = wChar >> 8;
    pair[1] = wChar & 255;
  }

}
