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

/** \file etag.h
 *  \brief Interface for ETag Unit
 */

#ifndef XQML_ETAG_H
#define XQML_ETAG_H

#include <common/codepoint.h>

namespace xqMLImpl
{
  //! The End Tag syntactic Unit
  class Etag: public xqMLImpl::Unit
  {
  public:
    unsigned short getElementsToClose();
    void setElementsToClose(unsigned int);
  private:
    unsigned short elements_to_close;
  };
}
#endif