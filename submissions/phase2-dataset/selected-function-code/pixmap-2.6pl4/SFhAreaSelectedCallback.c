void
SFhAreaSelectedCallback(w, n, pnew)
	Widget	w;
	int	n;
	int	pnew;
{
	SFDir	*dir;
	int	new;

	dir = &(SFdirs[SFdirPtr + n]);

	new = dir->hOrigin +
		(((double) pnew) / SFhScrollWidth) * dir->nChars;

	if (new > dir->nChars - SFcharsPerEntry) {
		new = dir->nChars - SFcharsPerEntry;
	}

	if (new < 0) {
		new = 0;
	}

	if (dir->nChars) {
		float	f;

		f = ((double) new) / dir->nChars;

		XawScrollbarSetThumb(
			w,
			f,
			(float) (((double) ((dir->nChars < SFcharsPerEntry) ?
				dir->nChars : SFcharsPerEntry)) / dir->nChars)
		);

		SFhSliderMovedCallback(w, n, &f);
	}
}
