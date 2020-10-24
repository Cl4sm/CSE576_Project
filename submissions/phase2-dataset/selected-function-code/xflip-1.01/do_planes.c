do_planes()
{
int	finished = 0;
int	width, xloc, yloc, dist, size, i;
short	**heights;

	heights = (short **) malloc(DisplayPlanes(dpy, screen));
	for (i = 0; i < DisplayPlanes(dpy, screen); i++)
		heights[i] = (short *) calloc(sizeof(short),
			DisplayWidth(dpy, screen));
	while (1)	{
		depth = rnd(DisplayPlanes(dpy, screen));
		width = rnd(MIN_WIDTH) + WIDTH_ADD;

		xloc = calc_xloc(width);

		yloc = DisplayHeight(dpy, screen);
		for (i = xloc; i < (xloc + width); i++)	{
			yloc = min(yloc, heights[depth][i]);
		}
		if (yloc == DisplayHeight(dpy, screen))
			continue;

		dist = rnd(yloc/10 + MIN_DIST);
		size = rnd(max(yloc + MIN_SIZE, MAX_SIZE));

		XSetPlaneMask(dpy, copygc, 1 << depth);
		XSetPlaneMask(dpy, fillgc, 1 << depth);
		XCopyArea(dpy, win, win, copygc, 
			xloc, yloc, 
			width, size,
			xloc, yloc + dist);
		XFillRectangle(dpy, win, fillgc, 
			xloc, yloc, 
			width, dist);
		yloc += dist;
		for (i = xloc; i < (xloc + width); i++)	{
		    if ((heights[depth][i] < (DisplayHeight(dpy, screen) - MIN_SIZE))
		        && (yloc >= (DisplayHeight(dpy, screen) - MIN_SIZE)))
				finished++;

		    heights[depth][i] = max(heights[depth][i], yloc);
		}
		if (finished >= DisplayPlanes(dpy, screen) * (DisplayWidth(dpy, screen) - FINISHED))
			finish();
	}
}