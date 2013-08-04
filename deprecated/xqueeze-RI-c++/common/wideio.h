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

/** \file wideio.h
 *  \brief Xqueeze specific overloads of I/O ops for 16-bit streams
 */

#ifndef XQML_WIDEIO_H
#define XQML_WIDEIO_H

#include <iostream>
#include <string>
#include <xqml/xqtypes.h>

namespace xqMLImpl
{
  using std::istream;
  using std::ostream;
  using std::string;
  using xqML::uint16_t;
  
  /** \ingroup impl @{ */
  /** \class WideIO wideio.h common/wideio.h
   *  \brief State information and I/O ops for 16-bit streams
   */
  class WideIO
  {
  public:
    //! Create a WideIO facility and initialize state
    explicit WideIO(bool isBigEndian)
    {
      if(isBigEndian)
	resolve_initPair = & xqMLImpl::WideIO::initPair_BE;
      else
	resolve_initPair = & xqMLImpl::WideIO::initPair_LE;
    }

    //! Returns an octet-pair as an unsigned integer
    uint16_t get(istream & i);

    //! Ignore input till the specified 16-bit delimiter
    istream & ignoreUntil(istream & i, const uint16_t wChar);

    //! Reads into a string upto (but excluding) a 16-bit delimiter
    istream & getline(istream & i, string & buffer, const uint16_t wChar);

  private:
    char pair[2];
    void (xqMLImpl::WideIO::* resolve_initPair)(uint16_t);
    
    inline void initPair(uint16_t wChar)
    {
      (this->*resolve_initPair)(wChar);
    }

    void initPair_LE (uint16_t);
    void initPair_BE (uint16_t);
  };
  /* @} */
}
#endif
