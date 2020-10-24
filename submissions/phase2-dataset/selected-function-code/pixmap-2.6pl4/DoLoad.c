{
  int status;

  if (PWQueryChanged(pixmap_widget)) {
    PWGetFilename(pixmap_widget, &filename);
  RetryLoadSave:
    switch (PopupDialog(qsave_dialog, "Save file before loading?",
			filename, &filename, XtGrabExclusive)) {
    case Yes:
      status = PWWriteFile(pixmap_widget, filename);
      if (status != XpmSuccess) {
	sprintf(message, "Can't write file: %s - %s", filename,
		XpmGetErrorString(status));
	if (PopupDialog(error_dialog, message, 
			NULL, NULL, XtGrabExclusive) == Retry) 
	  goto RetryLoadSave;
      }
      break;
      
    case Cancel:
      return;
    }
  }
  PWGetFilepath(pixmap_widget, &filename);
 RetryLoad:
  if (PopupFDialog(file_dialog, "Load file:", 
#ifndef USE_ATHENA
		  filename, &filename, XtGrabExclusive) == Okay) {
#else /* USE_ATHENA */
		  filename, &filename, "r") == Okay) {
#endif /* USE_ATHENA */
    status = PWReadFile(pixmap_widget, filename);
    if (status != XpmSuccess) {
      sprintf(message, "Can't read file: %s - %s", filename,
	      XpmGetErrorString(status));
      if (PopupDialog(error_dialog, message, 
		      NULL, NULL, XtGrabExclusive) == Retry)
	goto RetryLoad;
    }
    else {
      PWChangeNotify(pixmap_widget, NULL, NULL);
      PWClearChanged(pixmap_widget);
      FixStatus();
    }
  }
}
