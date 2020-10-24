flip_oblic(d, x, y, w, h)
Drawable        d;
int     x, y, w, h;
{
Pixmap  tempv;
Pixmap  temph;
int     i;
int     height = 1;
int     width = 1;

        assert(w > 0);
        assert(h > 0);
        tempv = XCreatePixmap(dpy, RootWindow(dpy, screen),
                w, height, DisplayPlanes(dpy, screen));
        temph = XCreatePixmap(dpy, RootWindow(dpy, screen),
                width, h, DisplayPlanes(dpy, screen));

        for (i = y; i <= (y + h)/do_half; i+= height)   {
                XCopyArea(dpy, d, tempv, copygc,
                        x, i,
                        w, height,
                        0, 0);
                XCopyArea(dpy, d, d, copygc,
                        x, (y + h) - (i - y),
                        w, height,
                        x, i);
                XCopyArea(dpy, tempv, d, copygc,
                        0, 0,
                        w, height,
                        x, (y + h) - (i - y));
                XCopyArea(dpy, d, temph, copygc,
                        i, y,
                        width, h,
                        0, 0);
                XCopyArea(dpy, d, d, copygc,
                        (w + x) - (i - x), y,
                        width, h,
                        i, y);
                XCopyArea(dpy, temph, d, copygc,
                        0, 0,
                        width, h,
                        (w + x) - (i - x), y);
        }
        XFreePixmap(dpy, tempv);
        XFreePixmap(dpy, temph);
}