{
	static int	alloc;
	static int	wasTwiddle = 0;
	char		*begin, *end;
	int		i, j;
	int		len;
	int		prevChange;
	int		SFdirPtrSave, SFdirEndSave;
	SFDir		*dir;

	if (!SFdirs) {
		SFdirs = (SFDir *) XtMalloc((alloc = 10) * sizeof(SFDir));
		dir = &(SFdirs[0]);
		dir->dir = XtMalloc(2);
		(void) strcpy(dir->dir, "/");
		(void) SFchdir("/");
		(void) SFgetDir(dir);
		for (j = 1; j < alloc; j++) {
			SFdirs[j].dir = NULL;
		}
		dir->path = SFcurrentPath + 1;
		dir->vOrigin = 0;
		dir->hOrigin = 0;
		dir->changed = 1;
		dir->beginSelection = -1;
		dir->endSelection = -1;
		SFgetHomeDirs();
	}

	SFdirEndSave = SFdirEnd;
	SFdirEnd = 1;

	SFdirPtrSave = SFdirPtr;
	SFdirPtr = 0;

	begin = NULL;

	if (SFcurrentPath[0] == '~') {
		if (!SFtwiddle) {
			SFtwiddle = 1;
			dir = &(SFdirs[0]);
			SFrootDir = *dir;
			*dir = SFhomeDir;
			dir->changed = 1;
		}
		end = SFcurrentPath;
		SFdoNotTouchDirPtr = 1;
		wasTwiddle = 1;
	} else {
		if (SFtwiddle) {
			SFtwiddle = 0;
			dir = &(SFdirs[0]);
			*dir = SFrootDir;
			dir->changed = 1;
		}
		end = SFcurrentPath + 1;
	}

	i = 0;

	prevChange = 0;

	while (*end) {
		while (*end++ == '/') {
			;
		}
		end--;
		begin = end;
		while ((*end) && (*end++ != '/')) {
			;
		}
		if ((end - SFcurrentPath <= SFtextPos) && (*(end - 1) == '/')) {
			SFdirPtr = i - 1;
			if (SFdirPtr < 0) {
				SFdirPtr = 0;
			}
		}
		if (*begin) {
			if (*(end - 1) == '/') {
				char save = *end;

				if (SFtwiddle) {
					if (SFfindHomeDir(begin, end)) {
						return;
					}
				}
				*end = 0;
				i++;
				SFdirEnd++;
				if (i >= alloc) {
					SFdirs = (SFDir *) XtRealloc(
						(char *) SFdirs,
						(unsigned) ((alloc *= 2) *
							sizeof(SFDir))
					);
					for (j = alloc / 2; j < alloc; j++) {
						SFdirs[j].dir = NULL;
					}
				}
				dir = &(SFdirs[i]);
				if (
					(!(dir->dir)) ||
					prevChange ||
					strcmp(dir->dir, begin)
				) {
					if (dir->dir) {
						SFfree(i);
					}
					prevChange = 1;
					len = strlen(begin) + 1;
					dir->dir = XtMalloc((unsigned) len);
					(void) strcpy(dir->dir, begin);
					dir->path = end;
					dir->vOrigin = 0;
					dir->hOrigin = 0;
					dir->changed = 1;
					dir->beginSelection = -1;
					dir->endSelection = -1;
					(void) SFfindFile(dir - 1, begin);
					if (
						SFchdir(SFcurrentPath) ||
						SFgetDir(dir)
					) {
						SFunreadableDir(dir);
						break;
					}
				}
				*end = save;
				if (!save) {
					SFunselect();
				}
			} else {
				if (SFfindFile(&(SFdirs[SFdirEnd-1]), begin)) {
					return;
				}
			}
		} else {
			SFunselect();
		}
	}

	if ((end == SFcurrentPath + 1) && (!SFtwiddle)) {
		SFunselect();
	}

	for (i = SFdirEnd; i < alloc; i++) {
		if (SFdirs[i].dir) {
			SFfree(i);
		}
	}

	if (SFdoNotTouchDirPtr) {
		if (wasTwiddle) {
			wasTwiddle = 0;
			SFdirPtr = SFdirEnd - 2;
			if (SFdirPtr < 0) {
				SFdirPtr = 0;
			}
		} else {
			SFdirPtr = SFdirPtrSave;
		}
		SFdoNotTouchDirPtr = 0;
	}

	if ((SFdirPtr != SFdirPtrSave) || (SFdirEnd != SFdirEndSave)) {
		XawScrollbarSetThumb(
			selFileHScroll,
			(float) (((double) SFdirPtr) / SFdirEnd),
			(float) (((double) ((SFdirEnd < 3) ? SFdirEnd : 3)) /
				SFdirEnd)
		);
	}

	if (SFdirPtr != SFdirPtrSave) {
		SFdrawLists(SF_DO_SCROLL);
	} else {
		for (i = 0; i < 3; i++) {
			if (SFdirPtr + i < SFdirEnd) {
				if (SFdirs[SFdirPtr + i].changed) {
					SFdirs[SFdirPtr + i].changed = 0;
					SFdrawList(i, SF_DO_SCROLL);
				}
			} else {
				SFclearList(i, SF_DO_SCROLL);
			}
		}
	}
}
