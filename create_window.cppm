module;
#include <X11/Xlib.h>

export module create_window;
import std;


export class YellowWindow
{
public:
    YellowWindow() : display(XOpenDisplay(nullptr))
    {
        Window root = DefaultRootWindow(display);

        window = XCreateSimpleWindow(display, root, 0, 0, 800, 600, 0, 0, 0xffffffff);

        XMapWindow(display, window);
    };

    ~YellowWindow()
    {
        XCloseDisplay(display);
    }

    void Run()
    {
        Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(display, window, &wm_delete_window, 1);
        XEvent event;
        while (!quited)
        {
            XNextEvent(display, &event);

            switch (event.type)
            {
            case ClientMessage:
                if (event.xclient.data.l[0] == wm_delete_window)
                {
                    XDestroyWindow(display, window);
                    quited = true;
                }
                break;
            default:
                break;
            }
        }
    }


    bool quited = false;
    Display* display;
    Window window;
};
