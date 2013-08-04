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

/** \file xqagenerator_impl.h
 *  \brief This file defines the xqAGenerator class
 */

#ifndef XQML_XQAGENERATOR_IMPL_HPP
#define XQML_XQAGENERATOR_IMPL_HPP
#include <iostream>
#include <string>
#include <set>

namespace xqML {
  class xqAGenerator;

  using std::string;
  using std::istream;
  using std::ostream;

  class xqAGeneratorFramework
  {
  private:
    typedef std::set<string> Identifiers;
    Identifiers ids;
    istream & dtd;
    ostream & xqa;
    string buffer;
    //! Generates the xqA specification out of the given DTD
    void generate(bool);
    //! Locates the next DTD entry
    void sync();
    //! Reads an Element/Empty Element
    void readElement();
    //! Reads an ATTLIST entry
    void readAttlist();
    //! Reads an Entity Reference
    void readEntity();
    //! Skips comment
    void ignoreComment();
    //! Reads a list of enumerated attribute values
    void readAttvalues(istream &);
    //! Assigns xqML Symbols to extracted identifiers and prints the xqA spec
    void assign_and_print();
    //! Expands parameter entities (unimplemented)
    void preprocessDTD();
    //! Returns the value of xqML Symbol that comes after it's argument
    unsigned int nextSymbol(unsigned int code);
    //! Constructor
    xqAGeneratorFramework(istream & dtd_input, ostream & xqAspecs_output)
      : dtd (dtd_input), xqa (xqAspecs_output) {}
    friend class xqML::xqAGenerator;
  };
}
#endif
