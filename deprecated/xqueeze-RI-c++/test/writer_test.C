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
#include <string>
#include <fstream>
#include <xqml/writer.h>
#include <xqml/exceptions.h>

int main(void)
{
  using std::string;
  try {
    std::ifstream map_input("test.xqa", std::ios_base::binary);
    std::ofstream xqml_output("test.xqm", std::ios_base::binary);
    xqML::Writer mywriter(map_input, xqml_output);
    mywriter.prolog("test.xqa");
    mywriter.startElement("ARTICLE");
    mywriter.attribute("AUTHOR", "Tahir Hashmi");
    mywriter.startAttribute("EDITOR");
    xqml_output<<"Tahir Hashmi";
    mywriter.endAttribute();
    mywriter.attribute("EDITION", "2nd", true);

    mywriter.startElement("HEADLINE");
    xqml_output<<"Xqueeze RI C++ v 0.2 Released";
    mywriter.endElement();

    mywriter.startElement("BYLINE");
    xqml_output<<"New version supports parsing without knowledge of schema";
    mywriter.endElement();

    mywriter.startElement("LEAD");
    xqml_output<<"A new version of Xqueeze Reference Implementation in C++ (Xqueeze-RI-c++) was released here today. "
	       <<"This version implements the latest version 0.3 of xqML encoding format.";
    mywriter.endElement();

    mywriter.startElement("BODY");
    xqml_output<<"The latest version of xqML contains inline document structure information,\
 thereby enabling parsers to parse arbitrary xqML documents without the knowledge of their schema."
	       <<" Other improvements include support for namespace declarations and prefixes.\
 Though there is no API for using them as yet, the parser is capable of reporting them."
	       <<"This version also supports writing and parsing of Processing Instructions.";
    mywriter.endElement();

    mywriter.startElement("NOTES");
    mywriter.characterReference(65);
    xqml_output<<" Random note. ";
    mywriter.comment("This is a comment");
    mywriter.startElement("XYZ", true);
    mywriter.endElement("NOTES"); //The argument is redundant
    
    mywriter.endElement("ARTICLE"); //The argument is redundant
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
