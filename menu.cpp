#include <sstream>
#include "menu.h"
#include "parseXML.h"

Menu::~Menu() {
  SDL_FreeSurface(surfaceOff);
  SDL_FreeSurface(surfaceOn);
  delete background;
  SDL_FreeSurface(backSurface);
}

Menu::Menu() : 
  io( IOManager::getInstance() ),
  words(), 
  position(),
  space(),
  parser( "xmlSpec/menu.xml" ),
  surfaceOff(io.loadAndSet( parser.getXmlStr("clickoffFile"), true)),
  surfaceOn(io.loadAndSet( parser.getXmlStr("clickonFile"), true)),
  backSurface(io.loadAndSet( parser.getXmlStr("backgroundFile"), true)),
  background(new Frame(backSurface,
                parser.getXmlInt("backgroundWidth"), 
                parser.getXmlInt("backgroundHeight"), 
                parser.getXmlInt("backgroundSrcX"), 
                parser.getXmlInt("backgroundSrcY"))),
  viewport( Viewport::getInstance() ),
  clicks(),
  nextIcon(0),
  click(0) 
{ 
  unsigned int n = parser.getXmlInt("wordsNumber");
  position[0] = parser.getXmlInt("wordsX");
  position[1] = parser.getXmlInt("wordsY");
  std::stringstream strm;
  for ( unsigned i = 0; i < n; ++i ) {
    strm << "words" << i;
    words.push_back( parser.getXmlStr(strm.str()) );
    strm.clear(); // clear error flags
    strm.str(std::string()); // clear contents
  }

  space[0] = parser.getXmlInt("wordsSpaceX");
  space[1] = parser.getXmlInt("wordsSpaceY");

  Frame clickoff(surfaceOff, 
              parser.getXmlInt("clickoffWidth"), 
              parser.getXmlInt("clickoffHeight"), 
              parser.getXmlInt("clickoffSrcX"), 
              parser.getXmlInt("clickoffSrcX")); 
  clicks.push_back( clickoff );

  Frame clickon(surfaceOn, 
              parser.getXmlInt("clickonWidth"), 
              parser.getXmlInt("clickonHeight"), 
              parser.getXmlInt("clickonSrcX"), 
              parser.getXmlInt("clickonSrcX")); 
  clicks.push_back( clickon );

}

void Menu::draw() const {
  int x = position[0];
  int y = position[1];
  background->draw(viewport.X(), viewport.Y(), 0, 0); 
  for (unsigned i = 0; i < words.size(); ++i) {
    io.printMessageAt(words[i], x, y);
    y += space[1];
  }
  y = position[1] + nextIcon*space[1];
  clicks[click].draw(viewport.X()+position[0]-space[0]+10, y-13);
}

const string& Menu::getIconClicked() const { 
  return words[nextIcon]; 
} 

