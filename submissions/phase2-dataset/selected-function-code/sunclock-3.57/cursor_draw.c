void cursor_draw(unsigned long color)
{
	XSetForeground(dpy, gc, color);
#ifdef VERTCURS
	XDrawRectangle(dpy, win, gc,
		(outxy.X * fwidth) + 2, (outxy.Y * fheight) + 2,
		0, fheight - 0);
#else
#ifdef BOXCURS
	XDrawRectangle(dpy, win, gc,
		(outxy.X * fwidth) + 1, (outxy.Y * fheight) + 2,
		fwidth, fheight);
#else
	XFillRectangle(dpy, win, gc,
		(outxy.X * fwidth) + 2, ((outxy.Y+1)* fheight) + 2,
		fwidth-1, 2);
#endif
#endif
	XSetForeground(dpy, gc, HiLo ? HiFgXColor.pixel : FgXColor.pixel);
}
