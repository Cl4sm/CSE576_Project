SFclearList(n, doScroll)
	int	n;
	int	doScroll;
{
	SFDir	*dir;

	SFcurrentInvert[n] = -1;

	XClearWindow(SFdisplay, XtWindow(selFileLists[n]));

	XDrawSegments(SFdisplay, XtWindow(selFileLists[n]), SFlineGC, SFsegs,
		2);

	if (doScroll) {
		dir = &(SFdirs[SFdirPtr + n]);

		if ((SFdirPtr + n < SFdirEnd) && dir->nEntries && dir->nChars) {
			XawScrollbarSetThumb(
				selFileVScrolls[n],
				(float) (((double) dir->vOrigin) /
					dir->nEntries),
				(float) (((double) ((dir->nEntries < SFlistSize)
					? dir->nEntries : SFlistSize)) /
					dir->nEntries)
			);

			XawScrollbarSetThumb(
				selFileHScrolls[n],
				(float) (((double) dir->hOrigin) / dir->nChars),
				(float) (((double) ((dir->nChars <
					SFcharsPerEntry) ? dir->nChars :
					SFcharsPerEntry)) / dir->nChars)
			);
		} else {
			XawScrollbarSetThumb(selFileVScrolls[n], (float) 0.0,
				(float) 1.0);
			XawScrollbarSetThumb(selFileHScrolls[n], (float) 0.0,
				(float) 1.0);
		}
	}
}
