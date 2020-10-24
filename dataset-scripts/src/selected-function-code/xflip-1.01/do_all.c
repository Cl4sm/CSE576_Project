do_all()
{
int	finished = 0;
int	width, xloc, yloc, dist, size, i, cnt=0;
short	*heights;

	heights = (short *) calloc(sizeof(short), DisplayWidth(dpy, screen));

	while (1)	{
		depth = rnd(DisplayPlanes(dpy, screen));
		width = rnd(MIN_WIDTH) + WIDTH_ADD;

		xloc = calc_xloc(width);

		yloc = DisplayHeight(dpy, screen);
		for (i = xloc; i < (xloc + width); i++)	{
			yloc = min(yloc, heights[i]);
		}
		if (yloc == DisplayHeight(dpy, screen))
			continue;

		dist = rnd(yloc/10 + MIN_DIST);
		size = rnd(max(yloc + MIN_SIZE, MAX_SIZE));

		XCopyArea(dpy, win, win, copygc, 
			xloc, yloc, 
			width, size,
			xloc, yloc + dist);
		XFillRectangle(dpy, win, fillgc, 
			xloc, yloc, 
			width, dist);
		yloc += dist;
		for (i = xloc; i < (xloc + width); i++)	{
		    if ((heights[i] < (DisplayHeight(dpy, screen) - MIN_SIZE))
		        && (yloc >= (DisplayHeight(dpy, screen) - MIN_SIZE)))
				finished++;

		    heights[i] = max(heights[i], yloc);
		}
		if (finished >= (DisplayWidth(dpy, screen) - FINISHED))	
			finish();

	  	if (delay > 0 && cnt++ == delay) {
		  	usleep(1);
		  	cnt=0;
		}
	}
}