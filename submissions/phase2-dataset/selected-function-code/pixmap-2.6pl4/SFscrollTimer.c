static void
SFscrollTimer(n)
	int	n;
{
	SFDir	*dir;
	int	save;

	dir = &(SFdirs[SFdirPtr + n]);
	save = dir->vOrigin;

	if (SFcurrentListY < SFlowerY) {
		if (dir->vOrigin > 0) {
			SFvSliderMovedCallback(selFileVScrolls[n], n,
				dir->vOrigin - 1);
		}
	} else if (SFcurrentListY > SFupperY) {
		if (dir->vOrigin < dir->nEntries - SFlistSize) {
			SFvSliderMovedCallback(selFileVScrolls[n], n,
				dir->vOrigin + 1);
		}
	}

	if (dir->vOrigin != save) {
		if (dir->nEntries) {
		    XawScrollbarSetThumb(
			selFileVScrolls[n],
			(float) (((double) dir->vOrigin) / dir->nEntries),
			(float) (((double) ((dir->nEntries < SFlistSize) ?
				dir->nEntries : SFlistSize)) / dir->nEntries)
		    );
		}
	}

	if (SFbuttonPressed) {
		SFscrollTimerId = XtAppAddTimeOut(SFapp,
			SFscrollTimerInterval(), SFscrollTimer, (XtPointer)(long)n);
	}
}
