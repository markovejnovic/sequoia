// Copyright Marko Vejnovic <contact@markovejnovic.com> 2021

#ifndef _SEQUOIA_MAIN_UI_HPP_
#define _SEQUOIA_MAIN_UI_HPP_

#include <string>
#include "uicontext.hpp"
#include "event.hpp"

#include "raylib.h"
#include "raygui.hpp"

#define DEF_SCREEN_WIDTH 800
#define DEF_SCREEN_HEIGHT 600
#define DEF_STATUSBAR_HEIGHT 24
#define DEF_MARGIN 16

namespace UI {

class MainUI {
 public:
    MainUI() {}
    explicit MainUI(std::string windowTitle);

    void main(UIContext*);


 protected:
    void loop(UIContext*);
    void init(UIContext*);

    /**
     * Function called when events are sent to this thread.
     */
    void handleEvents(UIContext*);

    /**
     * Function called to load the data. This function may or may not actually
     * make changes, signified by 0 for success, 1 for error.
     */
    int loadData(UIContext*);

    /**
     * Holds the events to be processed.
     */
    EventBuffer eventBuffer;

    std::string getStatusbarText();

 private:
    int screenWidth = DEF_SCREEN_WIDTH;
    int screenHeight = DEF_SCREEN_HEIGHT;
    std::string windowTitle = "MainUI";

    void handleInternalEvents(UIContext*);
    void drawDecisionButtons(UIContext*);
    void drawImage(UIContext*);
    void drawMetrics(UIContext*);

    // Model data.
    float fps = 0;
    Texture image;
};

} // namespace UI

#endif  // _SEQUOIA_MAIN_UI_HPP_
