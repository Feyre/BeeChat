/* shapetest.cpp
 * ENB241 Portfolio 2
 *
 * Shape inheritance demonstration
 *
 */

#include "GUI.h"
#include "User.h"
#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;




int main(int argc, char *argv[])
{
    User user("Feyre", SADMIN);
    GUI Gooey(user);
    Gooey.initScreen();
    Gooey.initWindows();
    Gooey.Routine();



    while(1);
    // wgetch(mainWindow.info_scr);
    endwin();
    return 0;
}
