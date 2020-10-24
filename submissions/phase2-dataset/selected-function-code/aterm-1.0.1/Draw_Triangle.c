Draw_Triangle(Window win, GC topShadow, GC botShadow, int x, int y, int w, int type)
{
    switch (type) {
    case 'r':			/* right triangle */
	XDrawLine(Xdisplay, win, topShadow, x, y, x, y + w);
	XDrawLine(Xdisplay, win, topShadow, x, y, x + w, y + w / 2);
	XDrawLine(Xdisplay, win, botShadow, x, y + w, x + w, y + w / 2);
	break;

    case 'l':			/* right triangle */
	XDrawLine(Xdisplay, win, botShadow, x + w, y + w, x + w, y);
	XDrawLine(Xdisplay, win, botShadow, x + w, y + w, x, y + w / 2);
	XDrawLine(Xdisplay, win, topShadow, x, y + w / 2, x + w, y);
	break;

    case 'd':			/* down triangle */
	XDrawLine(Xdisplay, win, topShadow, x, y, x + w / 2, y + w);
	XDrawLine(Xdisplay, win, topShadow, x, y, x + w, y);
	XDrawLine(Xdisplay, win, botShadow, x + w, y, x + w / 2, y + w);
	break;

    case 'u':			/* up triangle */
	XDrawLine(Xdisplay, win, botShadow, x + w, y + w, x + w / 2, y);
	XDrawLine(Xdisplay, win, botShadow, x + w, y + w, x, y + w);
	XDrawLine(Xdisplay, win, topShadow, x, y + w, x + w / 2, y);
	break;
#if 0
    case 's':			/* square */
	XDrawLine(Xdisplay, win, topShadow, x + w, y, x, y);
	XDrawLine(Xdisplay, win, topShadow, x, y, x, y + w);
	XDrawLine(Xdisplay, win, botShadow, x, y + w, x + w, y + w);
	XDrawLine(Xdisplay, win, botShadow, x + w, y + w, x + w, y);
	break;
#endif
    }
}
