void
SFvAreaSelectedCallback(w, n, pnew)
	Widget	w;
	int	n;
	int	pnew;
{
	SFDir	*dir;
	int	new;

	dir = &(SFdirs[SFdirPtr + n]);

	new = dir->vOrigin +
		(((double) pnew) / SFvScrollHeight) * dir->nEntries;

	if (new > dir->nEntries - SFlistSize) {
		new = dir->nEntries - SFlistSize;
	}

	if (new < 0) {
		new = 0;
	}

	if (dir->nEntries) {
		float	f;

		f = ((double) new) / dir->nEntries;

		XawScrollbarSetThumb(
			w,
			f,
			(float) (((double) ((dir->nEntries < SFlistSize) ?
				dir->nEntries : SFlistSize)) / dir->nEntries)
		);
	}

	SFvSliderMovedCallback(w, n, new);
}
