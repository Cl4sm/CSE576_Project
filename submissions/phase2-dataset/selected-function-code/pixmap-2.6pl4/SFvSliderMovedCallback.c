void
SFvSliderMovedCallback(w, n, new)
	Widget	w;
	int	n;
	int	new;
{
	int		old;
	register Window	win;
	SFDir		*dir;

	dir = &(SFdirs[SFdirPtr + n]);

	old = dir->vOrigin;
	dir->vOrigin = new;

	if (old == new) {
		return;
	}

	win = XtWindow(selFileLists[n]);

	if (ABS(new - old) < SFlistSize) {
		if (new > old) {
			XCopyArea(
				SFdisplay,
				win,
				win,
				SFscrollGC,
				SFlineToTextH,
				SFlowerY + (new - old) * SFentryHeight,
				SFentryWidth + SFlineToTextH,
				(SFlistSize - (new - old)) * SFentryHeight,
				SFlineToTextH,
				SFlowerY
			);
			XClearArea(
				SFdisplay,
				win,
				SFlineToTextH,
				SFlowerY + (SFlistSize - (new - old)) *
					SFentryHeight,
				SFentryWidth + SFlineToTextH,
				(new - old) * SFentryHeight,
				False
			);
			SFdrawStrings(win, dir, SFlistSize - (new - old),
				SFlistSize - 1);
		} else {
			XCopyArea(
				SFdisplay,
				win,
				win,
				SFscrollGC,
				SFlineToTextH,
				SFlowerY,
				SFentryWidth + SFlineToTextH,
				(SFlistSize - (old - new)) * SFentryHeight,
				SFlineToTextH,
				SFlowerY + (old - new) * SFentryHeight
			);
			XClearArea(
				SFdisplay,
				win,
				SFlineToTextH,
				SFlowerY,
				SFentryWidth + SFlineToTextH,
				(old - new) * SFentryHeight,
				False
			);
			SFdrawStrings(win, dir, 0, old - new);
		}
	} else {
		XClearArea(
			SFdisplay,
			win,
			SFlineToTextH,
			SFlowerY,
			SFentryWidth + SFlineToTextH,
			SFlistSize * SFentryHeight,
			False
		);
		SFdrawStrings(win, dir, 0, SFlistSize - 1);
	}
}
