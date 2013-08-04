/*
 * Author: Tahir Hashmi
 * 
 * Copyright (C) 2002 2003, Xqueeze Developers
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

/** \file xqagenerator.h
 *  \brief This file specifies the interface for xqML::xqAGenerator class
 */

#ifndef XQML_XQAGENERATOR_H
#define XQML_XQAGENERATOR_H
#include <iostream>
#include <string>

//! Namespace for implementation specific interfaces for xqAGenerator
namespace xqML {
  class xqAGeneratorFramework;

  using std::string;
  using std::istream;
  using std::ostream;
  /** \defgroup xqagen Xqueeze Association Generator
   *  \brief API for generating xqA specifications
   *
   *  This is used to generate xqA specification against a DTD for
   *  generation of xqML documents against it.
   *  @{
   */
  //! This class is used for creating an xqA out of a DTD
  class xqAGenerator
  {
  public:	
    //! Constructor
    xqAGenerator(istream & dtd, ostream & xqAspecs);
    //! Generates the xqA specs for the given DTD
    void generate(bool preprocess = false) ;
    ~xqAGenerator();
  private:
    xqAGeneratorFramework * xqagenerator_;
  };
  /** @} */
}
#endif
