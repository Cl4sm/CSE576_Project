FILE *
XsraSelFile(toplevel, prompt, ok, cancel, failed,
	    init_path, mode, show_entry, name_return)
	Widget		toplevel;
	char		*prompt;
	char		*ok;
	char		*cancel;
	char		*failed;
	char		*init_path;
	char		*mode;
	int		(*show_entry)();
	char		**name_return;
{
	static int	firstTime = 1;
	int		i;
	Arg		arglist[20];
	XEvent		event;
	FILE		*fp;

	if (!prompt) {
		prompt = "Pathname:";
	}

	if (!ok) {
		ok = "OK";
	}

	if (!cancel) {
		cancel = "Cancel";
	}

	if (firstTime) {
		firstTime = 0;
		SFdisplay = XtDisplay(toplevel);
		SFcreateWidgets(toplevel, prompt, ok, cancel);
	} else {
		i = 0;

		XtSetArg(arglist[i], XtNlabel, prompt);			i++;
		XtSetValues(selFilePrompt, arglist, i);

		i = 0;
		XtSetArg(arglist[i], XtNlabel, ok);			i++;
		XtSetValues(selFileOK, arglist, i);

		i = 0;
		XtSetArg(arglist[i], XtNlabel, cancel);			i++;
		XtSetValues(selFileCancel, arglist, i);
	}

	SFpositionWidget(selFile);
	XtMapWidget(selFile);

#if defined(SVR4) || defined(SYSV) || defined(USG) || defined(linux)
	if (!getcwd(SFstartDir, MAXPATHLEN)) {
#else /* defined(SVR4) || defined(SYSV) || defined(USG) */
	if (!getwd(SFstartDir)) {
#endif /* defined(SVR4) || defined(SYSV) || defined(USG) */

		XtAppError(SFapp, "XsraSelFile: can't get current directory");
	}
	(void) strcat(SFstartDir, "/");
	(void) strcpy(SFcurrentDir, SFstartDir);

	if (init_path) {
		if (init_path[0] == '/') {
			(void) strcpy(SFcurrentPath, init_path);
			if (strncmp(
				SFcurrentPath,
				SFstartDir,
				strlen(SFstartDir)
			)) {
				SFsetText(SFcurrentPath);
			} else {
				SFsetText(&(SFcurrentPath[strlen(SFstartDir)]));
			}
		} else {
			(void) strcat(strcpy(SFcurrentPath, SFstartDir),
				init_path);
			SFsetText(&(SFcurrentPath[strlen(SFstartDir)]));
		}
	} else {
		(void) strcpy(SFcurrentPath, SFstartDir);
	}

	SFfunc = show_entry;

	SFtextChanged();

	XtAddGrab(selFile, True, True);

	SFdirModTimerId = XtAppAddTimeOut(SFapp, (unsigned long) 1000,
		SFdirModTimer, (XtPointer) NULL);

	while (1) {
		XtAppNextEvent(SFapp, &event);
		XtDispatchEvent(&event);
		switch (SFstatus) {
		case SEL_FILE_TEXT:
			SFstatus = SEL_FILE_NULL;
			SFtextChanged();
			break;
		case SEL_FILE_OK:
			*name_return = SFgetText();
			if (fp = SFopenFile(*name_return, mode,
					    prompt, failed)) {
				SFprepareToReturn();
				return fp;
			}
			SFstatus = SEL_FILE_NULL;
			break;
		case SEL_FILE_CANCEL:
			SFprepareToReturn();
			return NULL;
		case SEL_FILE_NULL:
			break;
		}
	}
}
