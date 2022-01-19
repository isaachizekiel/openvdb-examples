/* */

#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <string>

#include <openvdb/openvdb.h>


namespace game_window {

  class GameWindow;

  enum { DEFAULT_WIDTH = 900, DEFAULT_HEIGHT = 800 };


  GameWindow init(const std::string& progName, bool background);

  void exit();


  class GameWindow {

  public:
    void open(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);

    void view(const openvdb::GridCPtrVec&);

    void handleEvents();

    void close();

    void resize(int width, int height);

    std::string getVersionString() const;
    
    
    
  private:
    friend GameWindow init(const std::string&, bool);
    GameWindow();
    
  };
  
} // game_window


#endif // GAME_WINDOW_H
