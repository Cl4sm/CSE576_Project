static Pixmap
create_bg_pixmap(void)
{
    Pixmap bg;

    bg = XCreatePixmap(display, icon_window, WINDOWED_SIZE_W, WINDOWED_SIZE_H,
		       depth);
    XSetForeground(display, gc, dockapp_getcolor("rgb:ae/aa/ae"));
    XFillRectangle(display, bg, gc, 0, 0, WINDOWED_SIZE_W, WINDOWED_SIZE_H);
    XSetForeground(display, gc, dockapp_getcolor("rgb:ff/ff/ff"));
    XDrawLine(display, bg, gc, 0, 0, 0, 63);
    XDrawLine(display, bg, gc, 1, 0, 1, 62);
    XDrawLine(display, bg, gc, 2, 0, 63, 0);
    XDrawLine(display, bg, gc, 2, 1, 62, 1);
    XSetForeground(display, gc, dockapp_getcolor("rgb:52/55/52"));
    XDrawLine(display, bg, gc, 1, 63, 63, 63);
    XDrawLine(display, bg, gc, 2, 62, 63, 62);
    XDrawLine(display, bg, gc, 63, 1, 63, 61);
    XDrawLine(display, bg, gc, 62, 2, 62, 61);

    return bg;
}