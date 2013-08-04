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

/** \file element.h
 *  \brief Interface specification for Element Unit
 */

#ifndef XQML_ELEMENT_H
#define XQML_ELEMENT_H

#include <common/unit.h>
#include <common/stag.h>
#include <common/attlist.h>
#include <common/content.h>

namespace xqMLImpl
{
  //! The Element syntactic Unit
  class Element: public xqMLImpl::Unit
  {
  private:
    xqMLImpl::STag stag_;
    xqMLImpl::Content content_;
  };
}
#endif
