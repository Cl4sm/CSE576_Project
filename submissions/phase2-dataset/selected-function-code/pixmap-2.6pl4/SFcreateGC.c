SFcreateGC()
{
	XGCValues	gcValues;
	XRectangle	rectangles[1];

	gcValues.foreground = SFfore;

	SFlineGC = XtGetGC(
		selFileLists[0],
		(XtGCMask)
			GCForeground		|
			0,
		&gcValues
	);

	SFscrollGC = XtGetGC(
		selFileLists[0],
		(XtGCMask)
			0,
		&gcValues
	);

	gcValues.function = GXinvert;
	gcValues.plane_mask = (SFfore ^ SFback);

	SFinvertGC = XtGetGC(
		selFileLists[0],
		(XtGCMask)
			GCFunction		|
			GCPlaneMask		|
			0,
		&gcValues
	);

	gcValues.foreground = SFfore;
	gcValues.background = SFback;
	gcValues.font = SFfont->fid;

	SFtextGC = XCreateGC(
		SFdisplay,
		XtWindow(selFileLists[0]),
		(unsigned long)
			GCForeground		|
			GCBackground		|
			GCFont			|
			0,
		&gcValues
	);

	rectangles[0].x = SFlineToTextH + SFbesideText;
	rectangles[0].y = 0;
	rectangles[0].width = SFcharsPerEntry * SFcharWidth;
	rectangles[0].height = SFupperY + 1;

	XSetClipRectangles(
		SFdisplay,
		SFtextGC,
		0,
		0,
		rectangles,
		1,
		Unsorted
	);
}
