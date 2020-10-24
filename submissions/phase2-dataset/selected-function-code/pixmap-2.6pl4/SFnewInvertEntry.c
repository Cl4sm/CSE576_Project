static int
SFnewInvertEntry(n, event)
	register int		n;
	register XMotionEvent	*event;
{
	register int	x, y;
	register int	new;
	static int	SFscrollTimerAdded = 0;

	x = event->x;
	y = event->y;

	if (SFdirPtr + n >= SFdirEnd) {
		return -1;
	} else if (
		(x >= 0)	&& (x <= SFupperX) &&
		(y >= SFlowerY)	&& (y <= SFupperY)
	) {
		register SFDir *dir = &(SFdirs[SFdirPtr + n]);

		if (SFscrollTimerAdded) {
			SFscrollTimerAdded = 0;
			XtRemoveTimeOut(SFscrollTimerId);
		}

		new = (y - SFlowerY) / SFentryHeight;
		if (dir->vOrigin + new >= dir->nEntries) {
			return -1;
		}
		return new;
	} else {
		if (SFbuttonPressed) {
			SFcurrentListY = y;
			if (!SFscrollTimerAdded) {
				SFscrollTimerAdded = 1;
				SFscrollTimerId = XtAppAddTimeOut(SFapp,
					SFscrollTimerInterval(), SFscrollTimer,
					(XtPointer)(long)n);
			}
		}

		return -1;
	}
}
