#include <string>
#include <iostream>
#include <algorithm>
#include <xqml/ContentHandler.hpp>
#include <xqml/XMLReader.hpp>
#include <xqml/XqaHandler.hpp>
#include <xqml/exceptions.hpp>

using std::string;
using std::cerr;
using std::ostream;
using namespace xqML;

namespace 
{
  struct doPrint
  {
    doPrint(ostream & str) : out(str){}
    void operator()(Attribute att) 
    { out<<" {"<<att.uri<<"}"<<att.name<<" "<<att.q_name<<" = \""<<att.value<<"\""; }
    private: ostream & out;
  };
}

ostream & operator <<(ostream & out, const Attributes & atts)
{
  std::for_each(atts.begin(), atts.end(), doPrint(out));
  return out;
}

class TestContentHandler : public ContentHandler
{
  public:
  //! Writes the characters received
  virtual void characters (const string & chars)
  {
    cerr<<"reader: characters ("<<chars<<")\n";
  }
  
  //! Finishes up the document
  virtual void endDocument ()
  {
    cerr<<"reader: endDocument ()\n";
  }
  
  //! Ends an element. The string argument is redundant and defaults to blank.
  virtual void endElement (const string & namespaceURI, const string & localName, const string & qName)
  {
    cerr<<"reader: endElement ("<<namespaceURI<<", "<<localName<<", "<<qName<<") \n";
  }
  
  //! Ends the scope of a prefix-URI mapping
  virtual void endPrefixMapping(const string & prefix)
  {
    cerr<<"reader: endPrefixMapping ("<<prefix<<")\n";
  }
  
  //! Handles ignorable whitespace
  virtual void ignorableWhitespace(const string & spaces)
  {
    cerr<<"reader: ingnorableWhitespace("<<spaces<<")\n";
  }
  
  //! Writes a Processing instruction
  virtual void processingInstruction (const string & target, const string & data)
  {
    cerr<<"reader: processingInstruction ("<<target<<", "<<data<<")\n";
  }
  
  //! Sets the Locator object for location information
  virtual void setDocumentLocator(Locator & locator)
  {
    cerr<<"reader: setDocumentLocator ()\n";
  }
  
  //! Writes an Entity Reference that is passed as the string argument
  virtual void skippedEntity (const string & name)
  {
    cerr<<"reader: skippedEntity ("<<name<<")\n";
  }
  
  //! Writes a prolog
  virtual void startDocument()
  {
    cerr<<"reader: startDocument ()\n";
  }
  
  //! Starts an Element.
  virtual void startElement (const string & uri, const string & localName, const string & qName, const Attributes & attributes)
  {
    cerr<<"reader: startElement ("<<uri<<", "<<localName<<", "<<qName<<", "<<attributes<<" )\n";
  }
  
  //! Begins the scope of a prefix-URI namespace mapping
  virtual void startPrefixMapping(const string & prefix, const string & uri)
  {
    cerr<<"reader: startPrefixMapping ("<<prefix<<", "<<uri<<")\n";
  }
  
};

int main()
{
  try {
    XqSpecFinder finder;
    XqaHandler myhandler(finder);
    XMLReader reader(myhandler);
    
    TestContentHandler tch;
    reader.setContentHandler(tch);
    reader.parse("test.xqm");
  }
  catch (SAXException & e ) {
    cerr<<(e.what())<<'\n';
  }
  catch (std::exception & e){
    cerr<<e.what()<<'\n';
  }
  return 0;
}
