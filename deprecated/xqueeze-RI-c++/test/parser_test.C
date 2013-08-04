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

#include <iostream>
#include <fstream>
#include <string>
#include <xqml/parser.h>
#include <xqml/exceptions.h>

int main (int argc, char * argv[])
{
  try {
    std::ifstream doc("test.xqm", std::ios_base::binary);
    if(argc == 2 && std::string(argv[1]) == "-t") {
      std::ifstream mapfile("test.xqa", std::ios_base::binary);
      std::ofstream output("test.xml");
      std::cerr<<"Parsing with translation enabled... ";
      output<<"<?xml version=\"1.0\"?>\n";
      xqML::Parser myparser(mapfile, doc, output);
      myparser.parseDocument();
      std::cerr<<"done\n\tSee file \"test.xml\" for an xml-like output (viewable in Mozilla/IE)\n";
    }
    else if (argc == 1) {
      std::ofstream output("test.dat");
      std::cerr<<"Parsing with translation disabled... ";
      xqML::Parser myparser(doc, output);
      myparser.parseDocument();
      std::cerr<<"done\n\tSee file \"test.dat\" for an xml-like output with numbers instead of identifiers\n";
    }
    else {
      std::cerr<<"Usage: parser_test [-t]\n";
      std::cerr<<"       Options:\n";
      std::cerr<<"       -t\tenable translation\n";
    }
  }
  catch (xqML::IllegalIdentifier exception) {
    std::cerr<<"Illegal Identifier "<<exception.identifier<<" of type "<<exception.type<<"\n";
  }
  catch (xqML::IllegalContext exception) {
    std::cerr<<"Illegal Context \""<<exception.context<<"\" while writing \""<<exception.identifier<<"\" of type "<<exception.type<<'\n';
  }
  catch (...) {
    std::cerr<<"Caught unknown exception, aborting :-(\n";
    std::abort();
  }
  return 0;
}
