//
// Let's try the more straightforward X11 API to click the mouse
// This example code was harvested from the internet
//
// https://www.linuxquestions.org/questions/programming-9/simulating-a-mouse-click-594576/
//
// Another link is here
// https://stackoverflow.com/questions/27984220/x11-sending-a-mouse-click-without-moving-a-pointer

#include <QMainWindow>
#include <QDebug>
#include <QString>

#include <mouseclick.h>

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>


// open and close do nothing.
// we are going to open the display each time we click.

bool mouseopen()
{
    return true;
}

bool mouseclose()
{
    return true;
}


bool mouseclick()
{
    int button = Button1; // just click button one. I'm sure it will be fine.
    Display *display = XOpenDisplay(NULL);

    XEvent event;

    if(display == NULL)
    {
        qDebug() << QString("XOpenDisplay returned NULL");
        return false;
        //fprintf(stderr, "Errore nell'apertura del Display !!!\n");
        //exit(EXIT_FAILURE);
    }

    memset(&event, 0x00, sizeof(event));

    event.type = ButtonPress;
    event.xbutton.button = button;
    event.xbutton.same_screen = True;

    XQueryPointer(display, RootWindow(display, DefaultScreen(display)),
                  &event.xbutton.root, &event.xbutton.window,
                  &event.xbutton.x_root, &event.xbutton.y_root,
                  &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

    event.xbutton.subwindow = event.xbutton.window;
    qDebug() << "Mouse Window " << event.xbutton.window;

#if 1 // don't look for a subwindow. Just use display. Is this Root?
    while(event.xbutton.subwindow)
    {
        event.xbutton.window = event.xbutton.subwindow;

        XQueryPointer(display, event.xbutton.window, &event.xbutton.root,
                      &event.xbutton.subwindow, &event.xbutton.x_root,
                      &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    }
#endif

    qDebug() << "Mouse Subwindow" << event.xbutton.window;

    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0)
    {
        qDebug() << QString("XSendEvent error XS1");
        //fprintf(stderr, "Errore nell'invio dell'evento !!!\n");
    }
    else
    {
        qDebug() << "Button Press";
    }

    XFlush(display);

    usleep(100000);

    event.type = ButtonRelease;
    event.xbutton.state = 0x100;

    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0)
    {
        qDebug() << QString("XSendEvent error XS2");
        //fprintf(stderr, "Errore nell'invio dell'evento !!!\n");
    }
    else
    {
        qDebug() << "Button Release";
    }

    XFlush(display);

    XCloseDisplay(display);
    return true;
}
