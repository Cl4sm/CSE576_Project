Pixmap
MakeHalfBrightPixmap(Pixmap original, GC gc)
{
    XImage *orig_image, *new_image;
    unsigned int width, height;
    unsigned int dummy, depth;
    int    dummy_x, dummy_y;
    int    bitmap_pad;
    Window win;
    Display *disp = XtDisplay(toplevel);
    int     scno = XDefaultScreen(disp);
    int     x, y;
    Pixmap  new_pixmap;

    XGetGeometry(disp, original, &win,
                 &dummy_x, &dummy_y, &width, &height, &dummy, &depth);
    orig_image = XGetImage(disp, original,
                           0, 0, width, height, AllPlanes, ZPixmap);
    bitmap_pad = (depth>16)?32:((depth>8)?16:8);
    new_image = XCreateImage(disp, XDefaultVisual(disp, scno), depth, ZPixmap,
                             0, NULL, width, height, bitmap_pad, 0);
    new_image->data = (char *)malloc(new_image->bytes_per_line * height);
    for(y=0; y<height; y++) {
        for(x=0; x<width; x++) {
            if ((x+y)%2 == 0)
                XPutPixel(new_image, x, y, BlackPixel(disp, scno));
            else
                XPutPixel(new_image, x, y, XGetPixel(orig_image, x, y));
        }
    }
    new_pixmap = XCreatePixmap(disp, win, width, height, depth);
    XPutImage(disp, new_pixmap, gc, new_image, 0, 0, 0, 0, width, height);
    XDestroyImage(new_image);
    return new_pixmap;
}
