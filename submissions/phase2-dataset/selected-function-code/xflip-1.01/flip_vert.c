flip_vert(d, x, y, w, h)
Drawable	d;
int	x, y, w, h;
{
Pixmap	temp;
int	i;
int	height = 1;

	assert(w > 0);
	temp = XCreatePixmap(dpy, RootWindow(dpy, screen), 
		w, height, DisplayPlanes(dpy, screen));

	for (i = y; i <= (y + h)/do_half; i+= height)	{
		XCopyArea(dpy, d, temp, copygc, 
			x, i,
			w, height,
			0, 0);
		XCopyArea(dpy, d, d, copygc, 
			x, (y + h) - (i - y),
			w, height,
			x, i);
		XCopyArea(dpy, temp, d, copygc, 
			0, 0, 
			w, height,
			x, (y + h) - (i - y));
	}
	XFreePixmap(dpy, temp);
}