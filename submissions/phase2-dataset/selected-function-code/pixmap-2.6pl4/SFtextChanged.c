{

	if ((SFtextBuffer[0] == '/') || (SFtextBuffer[0] == '~')) {
		(void) strcpy(SFcurrentPath, SFtextBuffer);

		SFtextPos = XawTextGetInsertionPoint(selFileField);
	} else {
		(void) strcat(strcpy(SFcurrentPath, SFstartDir), SFtextBuffer);

		SFtextPos = XawTextGetInsertionPoint(selFileField) +
			strlen(SFstartDir);
	}

	if (!SFworkProcAdded) {
		(void) XtAppAddWorkProc(SFapp, SFworkProc, NULL);
		SFworkProcAdded = 1;
	}

	SFupdatePath();
}
