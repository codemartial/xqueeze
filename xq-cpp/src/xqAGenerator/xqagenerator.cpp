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

/** \file xqagenerator.C
 *  \brief This file implements the xqAGenerator and xqAGeneratorFramework classes
 */

#include "xqagenerator_impl.hpp"
#include <xqml/xqagenerator.hpp>
#include <xqml/xqmlsymbol.hpp>
#include <sstream>

namespace xqML {
  using std::string;
  using std::istream;
  using std::ostream;
  /** \ingroup utils @{ */
  
  /** \class xqAGenerator xqagenerator.h xqml/xqagenerator.h
   *  \brief This class is used for creating an xqA out of a DTD
   */

  /** Constructor takes an std::istream reference for the DTD and an
   *  std::ostream reference for xqA specification output. The
   *  constructor simply allocates a new xqAGeneratorFramework object
   *  and passes all it's arguments to the xqAGeneratorFramework
   *  constructor.
   */
  xqAGenerator::xqAGenerator(istream & dtd, ostream & xqAspecs)
    : xqagenerator_(new xqAGeneratorFramework(dtd, xqAspecs)) {}

/** Calls xqMLxqAGen::xqAGeneratorFramework::generator() and passes it's
 *  argument to it. The parameter ``preprocess'' should be set to true
 *  if the DTD has Parameter Entities (not yet supported). It is false
 *  by default.
 */
  void xqAGenerator::generate(bool preprocess)
  {
    xqagenerator_->generate(preprocess);
  }
  
  xqAGenerator::~xqAGenerator()
  {
    delete xqagenerator_;
  }
  /** @} */

  using xqML::xqMLSymbol;
  /** \ingroup impl @{ */
  /** \class xqAGeneratorFramework xqagenerator_impl.h xqAGenerator/xqagenerator_impl.h
   *  \brief Class to hold private members for xqAGenerator */

  /** This function generates the xqA specification and prints it to the
   *  std::ostream whose reference was supplied as an argument to the
   *  constructor of this class. If ``preprocess'' is true, it calls the
   *  preprocessDTD method, that expands references to all parameter
   *  entities in the given DTD (not implemented yet). 
   */
  void xqAGeneratorFramework::generate(bool preprocess)
  {
    if(preprocess) preprocessDTD();
    sync(); 
    while (dtd>>buffer) { //Read the first word of the entry
      if(buffer == "ELEMENT") readElement();
      else if(buffer == "ATTLIST") readAttlist();
      else if(buffer == "ENTITY") readEntity();
      else if(buffer == "--") ignoreComment();
      else if(buffer.size() == 0) continue;
      else sync();
    }
    assign_and_print();
  }
  
  /** This method locates a new DTD entry (i.e. content starting with <!)
   *  and reads till the `!'
   */
  void xqAGeneratorFramework::sync()
  {
    do 
      if (!(getline(dtd, buffer, '<'))) break;
    while(dtd.get() != '!');
  }
  
  /** This method reads an Element or Empty Element and places it in the
   *  appropriate list of identifiers
   */
  void xqAGeneratorFramework::readElement()
  {
    string elementname;
    dtd>>elementname;
    dtd>>buffer;
    ids.insert(elementname);
    std::cerr<<elementname<<" read\n";
    sync();
  }
  
  /** This method reads Attributes, including those with enumerated
   *  values, and places them in the appropriate list of identifiers.
   */
  void xqAGeneratorFramework::readAttlist()
  {
    string attributename;
    dtd>>buffer; //Waste the element name
    getline(dtd, buffer, '>'); //Read the whole of this ATTLIST entry
    std::istringstream attlist(buffer);
    while (attlist>>attributename) {
      if (!attributename.size()) continue; //That was an empty line
      getline(attlist, buffer); //Read the rest of the line
      ids.insert(attributename);
      std::cerr<<attributename<<" read\n";
      if (buffer.find('(') < buffer.size()) { //We've got an attribute with enumerated values
	buffer = buffer.substr(buffer.find('('), buffer.size() - buffer.find('(') -1);
	readAttvalues(attlist);
      }
    }
    sync();
  }
  
  /** This method reads an Entity Reference and places the reference
   *  (*not* the replacement text) in the appropriate list of
   *  identifiers.
   */
  void xqAGeneratorFramework::readEntity()
  {
    //    std::cerr<<"Reading entity... ";
    dtd>>buffer; //Read in the entity name
    if(buffer.find('%') >= buffer.size())
      ids.insert(buffer);
    else 
      ; //Parameter entities not supported yet
    std::cerr<<buffer<<" read\n";
    sync();
  }
  
  /** Does what it says - ignore comments!
   */
  void xqAGeneratorFramework::ignoreComment()
  {
    do
      getline(dtd, buffer, '>');
    while (buffer.rfind("--") != buffer.size() -2);
    sync();
  }
  
  /** This method reads Enumerated Attributes Values and places them in
   *  the appropriate list of identifiers.
   */
  void xqAGeneratorFramework::readAttvalues(istream & attlist_source)
  {
    string temp;
    if (buffer.find(')') >= buffer.size()) {
      getline(attlist_source, temp, ')'); //Read the whole enum spec
      buffer = buffer.substr(1, buffer.size() -1) + temp;
    }
    else{
      buffer = buffer.substr(1, buffer.find(')') -1); //Ignore from ) to next whitespace
    }
    size_t pipeloc;
    while ((pipeloc = buffer.find('|')) < buffer.size())
      buffer.replace(pipeloc, 1, " ");
    std::istringstream va_instring(buffer);
    while(va_instring>>buffer) {
      ids.insert(buffer);
      std::cerr<<"Read attvalue: "<<buffer<<'\n';
    }
  }
  
  /** This method reads all the extracted identifiers from the various
   *  lists and assigns them xqMLSymbol values starting from 256
   *  decimal, in ascending alphabetical order. It also prints them in
   *  the std::ostream in correct xqA format.
   */
  void xqAGeneratorFramework::assign_and_print()
  {
    unsigned int code = 0x0100;
    xqa<<xqMLSymbol(0x1E)<<xqMLSymbol(0x20)
       <<"xqa"<<xqMLSymbol(0x1E)<<xqMLSymbol(0x1E); //Prolog
    std::cerr<<"Prolog\n";
    
    for (Identifiers::const_iterator i = ids.begin(); i != ids.end(); ++i, code = nextSymbol(code)) {
      putReservedSymbol(xqa, 0x1E)<<xqMLSymbol(code)<<*i;
      std::cerr<<*i<<" ("<<code<<")\n";
    }

    putReservedSymbol(xqa, 0x1E);
    putReservedSymbol (xqa, 0x40);
    std::cerr<<"End\n";
  }
  
  /** This method returns the next valid xqMLSymbol value that would
   *  come after the valid xqMLSymbol value supplied as argument. It
   *  does not check whether the argument is valid and the return value
   *  may not be valid in this case.
   */
  unsigned int xqAGeneratorFramework::nextSymbol(unsigned int code)
  {
    return xqMLSymbol::nextSymbol(code);
  }
  
  void xqAGeneratorFramework::preprocessDTD()
  {
  }
  /** @} */
}
