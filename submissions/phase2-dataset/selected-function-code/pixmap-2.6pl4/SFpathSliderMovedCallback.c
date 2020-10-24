void
SFpathSliderMovedCallback(w, client_data, new)
	Widget		w;
	XtPointer	client_data;
	float	*new;
{
	SFDir		*dir;
	int		n;
	XawTextPosition	pos;
	int	SFdirPtrSave;

	SFdirPtrSave = SFdirPtr;
	SFdirPtr = (*new) * SFdirEnd;
	if (SFdirPtr == SFdirPtrSave) {
		return;
	}

	SFdrawLists(SF_DO_SCROLL);

	n = 2;
	while (SFdirPtr + n >= SFdirEnd) {
		n--;
	}

	dir = &(SFdirs[SFdirPtr + n]);

	pos = dir->path - SFcurrentPath;

	if (!strncmp(SFcurrentPath, SFstartDir, strlen(SFstartDir))) {
		pos -= strlen(SFstartDir);
		if (pos < 0) {
			pos = 0;
		}
	}

	XawTextSetInsertionPoint(selFileField, pos);
}
