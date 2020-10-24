SFcheckDir(n, dir)
	int		n;
	SFDir		*dir;
{
	struct stat	statBuf;
	int		i;

	if (
		(!stat(".", &statBuf)) &&
		(statBuf.st_mtime != dir->mtime)
	) {

		/*
		 * If the pointer is currently in the window that we are about
		 * to update, we must warp it to prevent the user from
		 * accidentally selecting the wrong file.
		 */
		if (SFcurrentInvert[n] != -1) {
			XWarpPointer(
				SFdisplay,
				None,
				XtWindow(selFileLists[n]),
				0,
				0,
				0,
				0,
				0,
				0
			);
		}

		for (i = dir->nEntries - 1; i >= 0; i--) {
			if (dir->entries[i].shown != dir->entries[i].real) {
				XtFree(dir->entries[i].shown);
			}
			XtFree(dir->entries[i].real);
		}
		XtFree((char *) dir->entries);
		if (SFgetDir(dir)) {
			SFunreadableDir(dir);
		}
		if (dir->vOrigin > dir->nEntries - SFlistSize) {
			dir->vOrigin = dir->nEntries - SFlistSize;
		}
		if (dir->vOrigin < 0) {
			dir->vOrigin = 0;
		}
		if (dir->hOrigin > dir->nChars - SFcharsPerEntry) {
			dir->hOrigin = dir->nChars - SFcharsPerEntry;
		}
		if (dir->hOrigin < 0) {
			dir->hOrigin = 0;
		}
		dir->beginSelection = -1;
		dir->endSelection = -1;
		SFdoNotTouchVorigin = 1;
		if ((dir + 1)->dir) {
			(void) SFfindFile(dir, (dir + 1)->dir);
		} else {
			(void) SFfindFile(dir, dir->path);
		}

		if (!SFworkProcAdded) {
			(void) XtAppAddWorkProc(SFapp, SFworkProc, NULL);
			SFworkProcAdded = 1;
		}

		return 1;
	}

	return 0;
}
