#include "clipboard.h"

#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include <cstring>
#include <iostream>
#include <string>

void set_clipboard(const std::string &text)
{
	Display *display = XOpenDisplay(nullptr);
	if (!display)
	{
		std::cerr << "[ ERROR ]: Cannot load X11" << std::endl;
		return;
	}

	Atom clipboard = XInternAtom(display, "CLIPBOARD", False);

	XSetSelectionOwner(display, clipboard, XDefaultRootWindow(display), CurrentTime);
	XStoreBytes(display, text.c_str(), text.length());

	XCloseDisplay(display);
}
