#include <iostream>
#include <fstream>
#include <xqml/TranslatingContentHandler.hpp>
#include <xqml/XqaHandler.hpp>
#include <xqml/exceptions.hpp>

using namespace std;
using namespace xqML;

int main()
{
  try {
  XqSpecFinder finder;
  XqaHandler myhandler(finder);
  Attributes atts;
  TranslatingContentHandler tch(myhandler, cout, true) ;
  tch.startDocument();
  tch.startPrefixMapping("tst", "test");
  tch.startElement("test", "ARTICLE", "", atts);
  Attribute att;
  att.uri = "test";
  att.name = "BOdy";
  att.value = "BODY";
  atts.push_back(att);

  tch.startElement("test", "BODY", "", atts);
  tch.characters("blah");
  tch.endElement("test", "BODY", "");
  tch.endElement("test", "ARTICLE", "");

  tch.endPrefixMapping("tst");
  tch.endDocument();
  }
  catch (SAXException & e) {
    std::cerr<<e.what()<<'\n';
  }
}
