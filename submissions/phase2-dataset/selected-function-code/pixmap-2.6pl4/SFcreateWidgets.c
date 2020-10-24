static void
SFcreateWidgets(toplevel, prompt, ok, cancel)
	Widget	toplevel;
	char	*prompt;
	char	*ok;
	char	*cancel;
{
	int		i, n;
	int		listWidth, listHeight;
	int		listSpacing = 10;
	int		scrollThickness = 15;
	int		hScrollX, hScrollY;
	int		vScrollX, vScrollY;
	Cursor
			xtermCursor,
			sbRightArrowCursor,
			dotCursor;
	Arg		arglist[20];

	i = 0;
	XtSetArg(arglist[i], XtNtransientFor, toplevel);		i++;

	selFile = XtAppCreateShell("selFile", "SelFile",
		transientShellWidgetClass, SFdisplay, arglist, i);

	/* Add WM_DELETE_WINDOW protocol */
	XtAppAddActions(XtWidgetToApplicationContext(selFile),
		actions, XtNumber(actions));
	XtOverrideTranslations(selFile,
		XtParseTranslationTable(wmDeleteWindowTranslation));

	i = 0;
	XtSetArg(arglist[i], XtNdefaultDistance, 30);			i++;
	selFileForm = XtCreateManagedWidget("selFileForm",
		formWidgetClass, selFile, arglist, i);

	i = 0;
	XtSetArg(arglist[i], XtNlabel, prompt);				i++;
	XtSetArg(arglist[i], XtNresizable, True);			i++;
	XtSetArg(arglist[i], XtNtop, XtChainTop);			i++;
	XtSetArg(arglist[i], XtNbottom, XtChainTop);			i++;
	XtSetArg(arglist[i], XtNleft, XtChainLeft);			i++;
	XtSetArg(arglist[i], XtNright, XtChainLeft);			i++;
	XtSetArg(arglist[i], XtNborderWidth, 0);			i++;
	selFilePrompt = XtCreateManagedWidget("selFilePrompt",
		labelWidgetClass, selFileForm, arglist, i);

	i = 0;
	XtSetArg(arglist[i], XtNforeground, &SFfore);			i++;
	XtSetArg(arglist[i], XtNbackground, &SFback);			i++;
	XtGetValues(selFilePrompt, arglist, i);

	SFinitFont();

	SFentryWidth = SFbesideText + SFcharsPerEntry * SFcharWidth +
			SFbesideText;
	SFentryHeight = SFaboveAndBelowText + SFcharHeight +
			SFaboveAndBelowText;

	listWidth = SFlineToTextH + SFentryWidth + SFlineToTextH + 1 +
			scrollThickness;
	listHeight = SFlineToTextV + SFentryHeight + SFlineToTextV + 1 +
			SFlineToTextV + SFlistSize * SFentryHeight +
			SFlineToTextV + 1 + scrollThickness;

	SFpathScrollWidth = 3 * listWidth + 2 * listSpacing + 4;

	hScrollX = -1;
	hScrollY = SFlineToTextV + SFentryHeight + SFlineToTextV + 1 +
			SFlineToTextV + SFlistSize * SFentryHeight +
			SFlineToTextV;
	SFhScrollWidth = SFlineToTextH + SFentryWidth + SFlineToTextH;

	vScrollX = SFlineToTextH + SFentryWidth + SFlineToTextH;
	vScrollY = SFlineToTextV + SFentryHeight + SFlineToTextV;
	SFvScrollHeight = SFlineToTextV + SFlistSize * SFentryHeight +
			SFlineToTextV;

	SFupperX = SFlineToTextH + SFentryWidth + SFlineToTextH - 1;
	SFlowerY = SFlineToTextV + SFentryHeight + SFlineToTextV + 1 +
			SFlineToTextV;
	SFupperY = SFlineToTextV + SFentryHeight + SFlineToTextV + 1 +
			SFlineToTextV + SFlistSize * SFentryHeight - 1;

	SFtextX = SFlineToTextH + SFbesideText;
	SFtextYoffset = SFlowerY + SFaboveAndBelowText + SFcharAscent;

	SFsegs[0].x1 = 0;
	SFsegs[0].y1 = vScrollY;
	SFsegs[0].x2 = vScrollX - 1;
	SFsegs[0].y2 = vScrollY;
	SFsegs[1].x1 = vScrollX;
	SFsegs[1].y1 = 0;
	SFsegs[1].x2 = vScrollX;
	SFsegs[1].y2 = vScrollY - 1;

	SFcompletionSegs[0].x1 = SFcompletionSegs[0].x2 = SFlineToTextH;
	SFcompletionSegs[1].x1 = SFcompletionSegs[1].x2 =
		SFlineToTextH + SFentryWidth - 1;

	i = 0;
	XtSetArg(arglist[i], XtNwidth, 3 * listWidth + 2 * listSpacing + 4);
									i++;
	XtSetArg(arglist[i], XtNborderColor, SFfore);			i++;

	XtSetArg(arglist[i], XtNfromVert, selFilePrompt);		i++;
	XtSetArg(arglist[i], XtNvertDistance, 10);			i++;
	XtSetArg(arglist[i], XtNresizable, True);			i++;
	XtSetArg(arglist[i], XtNtop, XtChainTop);			i++;
	XtSetArg(arglist[i], XtNbottom, XtChainTop);			i++;
	XtSetArg(arglist[i], XtNleft, XtChainLeft);			i++;
	XtSetArg(arglist[i], XtNright, XtChainLeft);			i++;
	XtSetArg(arglist[i], XtNstring, SFtextBuffer);			i++;
	XtSetArg(arglist[i], XtNlength, MAXPATHLEN);			i++;
	XtSetArg(arglist[i], XtNeditType, XawtextEdit);			i++;
	XtSetArg(arglist[i], XtNwrap, XawtextWrapWord);			i++;
	XtSetArg(arglist[i], XtNresize, XawtextResizeHeight);		i++;
	XtSetArg(arglist[i], XtNuseStringInPlace, True);		i++;
	selFileField = XtCreateManagedWidget("selFileField",
		asciiTextWidgetClass, selFileForm, arglist, i);

	XtOverrideTranslations(selFileField,
		XtParseTranslationTable(oneLineTextEditTranslations));
	XtSetKeyboardFocus(selFileForm, selFileField);

	i = 0;
	XtSetArg(arglist[i], XtNorientation, XtorientHorizontal);	i++;
	XtSetArg(arglist[i], XtNwidth, SFpathScrollWidth);		i++;
	XtSetArg(arglist[i], XtNheight, scrollThickness);		i++;
	XtSetArg(arglist[i], XtNborderColor, SFfore);			i++;
	XtSetArg(arglist[i], XtNfromVert, selFileField);		i++;
	XtSetArg(arglist[i], XtNvertDistance, 30);			i++;
	XtSetArg(arglist[i], XtNtop, XtChainTop);			i++;
	XtSetArg(arglist[i], XtNbottom, XtChainTop);			i++;
	XtSetArg(arglist[i], XtNleft, XtChainLeft);			i++;
	XtSetArg(arglist[i], XtNright, XtChainLeft);			i++;
	selFileHScroll = XtCreateManagedWidget("selFileHScroll",
		scrollbarWidgetClass, selFileForm, arglist, i);

	XtAddCallback(selFileHScroll, XtNjumpProc,
		SFpathSliderMovedCallback, (XtPointer) NULL);
	XtAddCallback(selFileHScroll, XtNscrollProc,
		SFpathAreaSelectedCallback, (XtPointer) NULL);

	i = 0;
	XtSetArg(arglist[i], XtNwidth, listWidth);			i++;
	XtSetArg(arglist[i], XtNheight, listHeight);			i++;
	XtSetArg(arglist[i], XtNborderColor, SFfore);			i++;
	XtSetArg(arglist[i], XtNfromVert, selFileHScroll);		i++;
	XtSetArg(arglist[i], XtNvertDistance, 10);			i++;
	XtSetArg(arglist[i], XtNtop, XtChainTop);			i++;
	XtSetArg(arglist[i], XtNbottom, XtChainTop);			i++;
	XtSetArg(arglist[i], XtNleft, XtChainLeft);			i++;
	XtSetArg(arglist[i], XtNright, XtChainLeft);			i++;
	selFileLists[0] = XtCreateManagedWidget("selFileList1",
		compositeWidgetClass, selFileForm, arglist, i);

	i = 0;
	XtSetArg(arglist[i], XtNwidth, listWidth);			i++;
	XtSetArg(arglist[i], XtNheight, listHeight);			i++;
	XtSetArg(arglist[i], XtNborderColor, SFfore);			i++;
	XtSetArg(arglist[i], XtNfromHoriz, selFileLists[0]);		i++;
	XtSetArg(arglist[i], XtNfromVert, selFileHScroll);		i++;
	XtSetArg(arglist[i], XtNhorizDistance, listSpacing);		i++;
	XtSetArg(arglist[i], XtNvertDistance, 10);			i++;
	XtSetArg(arglist[i], XtNtop, XtChainTop);			i++;
	XtSetArg(arglist[i], XtNbottom, XtChainTop);			i++;
	XtSetArg(arglist[i], XtNleft, XtChainLeft);			i++;
	XtSetArg(arglist[i], XtNright, XtChainLeft);			i++;
	selFileLists[1] = XtCreateManagedWidget("selFileList2",
		compositeWidgetClass, selFileForm, arglist, i);

	i = 0;
	XtSetArg(arglist[i], XtNwidth, listWidth);			i++;
	XtSetArg(arglist[i], XtNheight, listHeight);			i++;
	XtSetArg(arglist[i], XtNborderColor, SFfore);			i++;
	XtSetArg(arglist[i], XtNfromHoriz, selFileLists[1]);		i++;
	XtSetArg(arglist[i], XtNfromVert, selFileHScroll);		i++;
	XtSetArg(arglist[i], XtNhorizDistance, listSpacing);		i++;
	XtSetArg(arglist[i], XtNvertDistance, 10);			i++;
	XtSetArg(arglist[i], XtNtop, XtChainTop);			i++;
	XtSetArg(arglist[i], XtNbottom, XtChainTop);			i++;
	XtSetArg(arglist[i], XtNleft, XtChainLeft);			i++;
	XtSetArg(arglist[i], XtNright, XtChainLeft);			i++;
	selFileLists[2] = XtCreateManagedWidget("selFileList3",
		compositeWidgetClass, selFileForm, arglist, i);

	for (n = 0; n < 3; n++) {

		i = 0;
		XtSetArg(arglist[i], XtNx, vScrollX);			i++;
		XtSetArg(arglist[i], XtNy, vScrollY);			i++;
		XtSetArg(arglist[i], XtNwidth, scrollThickness);	i++;
		XtSetArg(arglist[i], XtNheight, SFvScrollHeight);	i++;
		XtSetArg(arglist[i], XtNborderColor, SFfore);		i++;
		selFileVScrolls[n] = XtCreateManagedWidget("selFileVScroll",
			scrollbarWidgetClass, selFileLists[n], arglist, i);

		XtAddCallback(selFileVScrolls[n], XtNjumpProc,
			SFvFloatSliderMovedCallback, (XtPointer)(long)n);
		XtAddCallback(selFileVScrolls[n], XtNscrollProc,
			SFvAreaSelectedCallback, (XtPointer)(long)n);

		i = 0;

		XtSetArg(arglist[i], XtNorientation, XtorientHorizontal);
									i++;
		XtSetArg(arglist[i], XtNx, hScrollX);			i++;
		XtSetArg(arglist[i], XtNy, hScrollY);			i++;
		XtSetArg(arglist[i], XtNwidth, SFhScrollWidth);		i++;
		XtSetArg(arglist[i], XtNheight, scrollThickness);	i++;
		XtSetArg(arglist[i], XtNborderColor, SFfore);		i++;
		selFileHScrolls[n] = XtCreateManagedWidget("selFileHScroll",
			scrollbarWidgetClass, selFileLists[n], arglist, i);

		XtAddCallback(selFileHScrolls[n], XtNjumpProc,
			SFhSliderMovedCallback, (XtPointer)(long)n);
		XtAddCallback(selFileHScrolls[n], XtNscrollProc,
			SFhAreaSelectedCallback, (XtPointer)(long)n);
	}

	i = 0;
	XtSetArg(arglist[i], XtNlabel, ok);				i++;
	XtSetArg(arglist[i], XtNresizable, True);			i++;
	XtSetArg(arglist[i], XtNcallback, SFokSelect);			i++;
	XtSetArg(arglist[i], XtNborderColor, SFfore);			i++;
	XtSetArg(arglist[i], XtNfromVert, selFileLists[0]);		i++;
	XtSetArg(arglist[i], XtNvertDistance, 30);			i++;
	XtSetArg(arglist[i], XtNtop, XtChainTop);			i++;
	XtSetArg(arglist[i], XtNbottom, XtChainTop);			i++;
	XtSetArg(arglist[i], XtNleft, XtChainLeft);			i++;
	XtSetArg(arglist[i], XtNright, XtChainLeft);			i++;
	selFileOK = XtCreateManagedWidget("selFileOK", commandWidgetClass,
		selFileForm, arglist, i);

	i = 0;
	XtSetArg(arglist[i], XtNlabel, cancel);				i++;
	XtSetArg(arglist[i], XtNresizable, True);			i++;
	XtSetArg(arglist[i], XtNcallback, SFcancelSelect);		i++;
	XtSetArg(arglist[i], XtNborderColor, SFfore);			i++;
	XtSetArg(arglist[i], XtNfromHoriz, selFileOK);			i++;
	XtSetArg(arglist[i], XtNfromVert, selFileLists[0]);		i++;
	XtSetArg(arglist[i], XtNhorizDistance, 30);			i++;
	XtSetArg(arglist[i], XtNvertDistance, 30);			i++;
	XtSetArg(arglist[i], XtNtop, XtChainTop);			i++;
	XtSetArg(arglist[i], XtNbottom, XtChainTop);			i++;
	XtSetArg(arglist[i], XtNleft, XtChainLeft);			i++;
	XtSetArg(arglist[i], XtNright, XtChainLeft);			i++;
	selFileCancel = XtCreateManagedWidget("selFileCancel",
		commandWidgetClass, selFileForm, arglist, i);

	XtSetMappedWhenManaged(selFile, False);
	XtRealizeWidget(selFile);

	/* Add WM_DELETE_WINDOW protocol */
	SFwmDeleteWindow = XInternAtom(SFdisplay, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(SFdisplay, XtWindow(selFile), &SFwmDeleteWindow, 1);

	SFcreateGC();

	xtermCursor = XCreateFontCursor(SFdisplay, XC_xterm);

	sbRightArrowCursor = XCreateFontCursor(SFdisplay, XC_sb_right_arrow);
	dotCursor = XCreateFontCursor(SFdisplay, XC_dot);

	XDefineCursor(SFdisplay, XtWindow(selFileForm), xtermCursor);
	XDefineCursor(SFdisplay, XtWindow(selFileField), xtermCursor);

	for (n = 0; n < 3; n++) {
		XDefineCursor(SFdisplay, XtWindow(selFileLists[n]),
			sbRightArrowCursor);
	}
	XDefineCursor(SFdisplay, XtWindow(selFileOK), dotCursor);
	XDefineCursor(SFdisplay, XtWindow(selFileCancel), dotCursor);

	for (n = 0; n < 3; n++) {
		XtAddEventHandler(selFileLists[n], ExposureMask, True,
			SFexposeList, (XtPointer)(long)n);
		XtAddEventHandler(selFileLists[n], EnterWindowMask, False,
			SFenterList, (XtPointer)(long)n);
		XtAddEventHandler(selFileLists[n], LeaveWindowMask, False,
			SFleaveList, (XtPointer)(long)n);
		XtAddEventHandler(selFileLists[n], PointerMotionMask, False,
			SFmotionList, (XtPointer)(long)n);
		XtAddEventHandler(selFileLists[n], ButtonPressMask, False,
			SFbuttonPressList, (XtPointer)(long)n);
		XtAddEventHandler(selFileLists[n], ButtonReleaseMask, False,
			SFbuttonReleaseList, (XtPointer)(long)n);
	}

	XtAddEventHandler(selFileField, KeyPressMask, False,
		SFmodVerifyCallback, (XtPointer) NULL);

	SFapp = XtWidgetToApplicationContext(selFile);

}
