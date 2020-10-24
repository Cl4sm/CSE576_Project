SFdirModTimer()
{
	static int	n = -1;
	static int	f = 0;
	char		save;
	SFDir		*dir;

	if ((!SFtwiddle) && (SFdirPtr < SFdirEnd)) {
		n++;
		if ((n > 2) || (SFdirPtr + n >= SFdirEnd)) {
			n = 0;
			f++;
			if ((f > 2) || (SFdirPtr + f >= SFdirEnd)) {
				f = 0;
			}
		}
		dir = &(SFdirs[SFdirPtr + n]);
		save = *(dir->path);
		*(dir->path) = 0;
		if (SFchdir(SFcurrentPath)) {
			*(dir->path) = save;

			/*
			 * force a re-read
			 */
			*(dir->dir) = 0;

			SFupdatePath();
		} else {
			*(dir->path) = save;
			if (
				SFcheckDir(n, dir) ||
				((f == n) && SFcheckFiles(dir))
			) {
				SFdrawList(n, SF_DO_SCROLL);
			}
		}
	}

	SFdirModTimerId = XtAppAddTimeOut(SFapp, (unsigned long) 1000,
		SFdirModTimer, (XtPointer) NULL);
}
