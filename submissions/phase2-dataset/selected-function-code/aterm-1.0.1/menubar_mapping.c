int
menubar_mapping(int map)
{
    int             change = 0;
#ifdef MENUBAR

    if (map && !menubar_visible()) {
	menuBar.state = 1;
	XMapWindow(Xdisplay, menuBar.win);
	change = 1;
    } else if (!map && menubar_visible()) {
	menubar_expose();
	menuBar.state = 0;
	XUnmapWindow(Xdisplay, menuBar.win);
	change = 1;
    } else
	menubar_expose();
#endif

    return change;
}
