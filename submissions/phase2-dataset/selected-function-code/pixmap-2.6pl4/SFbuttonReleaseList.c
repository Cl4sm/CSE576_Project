SFbuttonReleaseList(w, n, event)
	Widget			w;
	int			n;
	XButtonReleasedEvent	*event;
{
	SFDir	*dir;

	SFbuttonPressed = 0;

	if (SFcurrentInvert[n] != -1) {
		if (n < 2) {
			SFdoNotTouchDirPtr = 1;
		}
		SFdoNotTouchVorigin = 1;
		dir = &(SFdirs[SFdirPtr + n]);
		SFreplaceText(
			dir,
			dir->entries[dir->vOrigin + SFcurrentInvert[n]].shown
		);
		SFmotionList(w, n, event);
	}
}
