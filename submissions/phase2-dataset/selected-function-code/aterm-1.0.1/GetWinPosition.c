GetWinPosition (Window w, int *x, int *y)
{
#ifndef X_DISPLAY_MISSING
	int bRes = 1;
	static int rootWidth = 0, rootHeight = 0;
	int my_x, my_y;
	Window wdumm;

	if (!x)
  		x = &my_x;
	if (!y)
  		y = &my_y;

	*x = 0;
	*y = 0;

	if (!rootWidth || !rootHeight)
  		if (!GetRootDimensions (&rootWidth, &rootHeight))
    		return 0;

	XTranslateCoordinates (dpy, w, RootWindow(dpy,DefaultScreen(dpy)), 0, 0, x, y, &wdumm);
	/* taking in to consideration virtual desktopping */
	if (*x < 0 || *x >= rootWidth || *y < 0 || *y >= rootHeight)
		bRes = 0;
	/* don't want to return position outside the screen even if we fail */
	while(*x < 0)
		*x += rootWidth;
	while (*y < 0)
		*y += rootHeight;
	if (*x > rootWidth)
		*x %= rootWidth;
	if (*y > rootHeight)
		*y %= rootHeight;
	return bRes;
#endif
	*x = 0;
	*y = 0;
	return 0;
}
