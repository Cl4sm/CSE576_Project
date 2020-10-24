void DoQuit()
{
  if (PWQueryChanged(pixmap_widget)) {
    PWGetFilename(pixmap_widget, &filename);
  RetryQuit:
    switch (PopupDialog(qsave_dialog, "Save file before quitting?",
			filename, &filename, XtGrabExclusive)) {
    case Yes:
      if (PWWriteFile(pixmap_widget, filename) != XpmSuccess) {
	sprintf(message, "Can't write file: %s", filename);
	if (PopupDialog(error_dialog, message, 
			NULL, NULL, XtGrabExclusive) == Retry) 
	  goto RetryQuit;
	else return;
      }
      break;
      
    case Cancel:
      return;
    }
  }
  exit(0);
}
