void show_vbar()
{
	int j;

	/* display buffer screen height in pixels */
	vtot = (screen_height-2)*fheight - font->descent;

	/* font true height as a constant */
	ftheight = fheight+font->descent+1;

	/* thumb height is display buffer percentage of total lines */
	vcur = (vtot*(screen_height-1))/((ytot)?ytot:1);

	/* min thumb size is 1 char */
	if (vcur<fheight) vcur = fheight;

	/* max thumb size is 2/3 vtot */
	j = 2*vtot/3;
	if (vcur>j) vcur = j;

	/* current line number to vertical thumb pos */
	pos = (ytru * (vtot-(vcur-fheight)))/((ytot)? ytot:1);

	/* draw scrollbar through */
	XSetForeground(dpy,gc,HiBgXColor.pixel);
#ifdef THREED	// provide background for line scroll triangles
	XFillRectangle(dpy, win, gc, Width-10, 0, 10, Height);
#else	/* indicate line scroll rectangles */
	XFillRectangle(dpy, win, gc, Width-10, ftheight, 10, Height+2-2*(ftheight));
#endif /* THREED */

	/* draw thumb */
	XSetForeground(dpy,gc,~HiBgXColor.pixel);
	XDrawRectangle(dpy, win, gc, Width-9, ftheight+pos, 8, vcur);

#ifdef THREED
	{
		XPoint opoints[] ={{Width-10,ftheight-1},{10,0},
						   {-5,-9},{-5,9},
						   {0,Height+2-(2*(ftheight))},{5,9},
						   {5,-9},{-10,0}
						  };
		XSegment lpoints[] ={{Width-1, ftheight+pos, Width-9, ftheight+pos},
							 {Width-9, ftheight+pos, Width-9, ftheight+pos+vcur},
							 {Width-9, ftheight-2, Width-6, ftheight-9},
							 {Width-9, Height+2-ftheight, Width-6, Height+3-ftheight+6}
							};

		/* outline trough and end triangles */
		XDrawLines(dpy, win, gc, opoints, 8, CoordModePrevious);

		/* draw in light color for 3D shading */
		XSetForeground(dpy, gc, -1);
		XDrawSegments(dpy, win, gc, lpoints, 4);
	}
#endif /* THREED */
	lowvideo();
}
