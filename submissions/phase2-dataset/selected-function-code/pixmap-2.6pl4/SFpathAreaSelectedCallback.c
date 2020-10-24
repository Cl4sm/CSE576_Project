void
SFpathAreaSelectedCallback(w, client_data, pnew)
	Widget		w;
	XtPointer	client_data;
	int		pnew;
{
	int	new;
	float	f;

	new = SFdirPtr + (((double) pnew) / SFpathScrollWidth) * SFdirEnd;

	if (new > SFdirEnd - 3) {
		new = SFdirEnd - 3;
	}

	if (new < 0) {
		new = 0;
	}

	f = ((double) new) / SFdirEnd;

	XawScrollbarSetThumb(
		w,
		f,
		(float) (((double) ((SFdirEnd < 3) ? SFdirEnd : 3)) /
			SFdirEnd)
	);

	SFpathSliderMovedCallback(w, (XtPointer) NULL, &f);
}
