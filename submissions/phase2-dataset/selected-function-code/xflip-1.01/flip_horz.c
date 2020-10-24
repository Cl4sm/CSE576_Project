flip_horz(d, x, y, w, h)
Drawable	d;
int	x, y, w, h;
{
Pixmap	temp;
int	i;
int	width = 1;

	assert(h > 0);
	temp = XCreatePixmap(dpy, RootWindow(dpy, screen), 
		width, h, DisplayPlanes(dpy, screen));

	for (i = x; i <= (x + w)/do_half; i+= width)	{
		XCopyArea(dpy, d, temp, copygc, 
			i, y, 
			width, h,
			0, 0);
		XCopyArea(dpy, d, d, copygc, 
			(w + x) - (i - x), y, 
			width, h,
			i, y);
		XCopyArea(dpy, temp, d, copygc, 
			0, 0, 
			width, h,
			(w + x) - (i - x), y);
	}
	XFreePixmap(dpy, temp);
}