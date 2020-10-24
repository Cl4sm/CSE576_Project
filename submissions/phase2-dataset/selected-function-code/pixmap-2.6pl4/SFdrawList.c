SFdrawList(n, doScroll)
	int	n;
	int	doScroll;
{
	SFDir	*dir;
	Window	w;

	SFclearList(n, doScroll);

	if (SFdirPtr + n < SFdirEnd) {
		dir = &(SFdirs[SFdirPtr + n]);
		w = XtWindow(selFileLists[n]);
		XDrawImageString(
			SFdisplay,
			w,
			SFtextGC,
			SFtextX - dir->hOrigin * SFcharWidth,
			SFlineToTextV + SFaboveAndBelowText + SFcharAscent,
			dir->dir,
			strlen(dir->dir)
		);
		SFdrawStrings(w, dir, 0, SFlistSize - 1);
	}
}
