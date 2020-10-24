void DoSaveAs()
{
  PWGetFilename(pixmap_widget, &filename);
 RetrySave:
#ifndef USE_ATHENA
  if (PopupFDialog(file_dialog, "Save file:", 
		   filename, &filename, XtGrabExclusive) == Okay) {
#else /* USE_ATHENA */
  if (PopupFDialog(file_dialog, "Save file:", 
		   filename, &filename, "w") == Okay) {
#endif /* USE_ATHENA */
    if (PWWriteFile(pixmap_widget, filename) != XpmSuccess) {
      sprintf(message, "Can't write file: %s", filename);
      if (PopupDialog(error_dialog, message, 
		      NULL, NULL, XtGrabExclusive) == Retry)
	goto RetrySave;
    }
    else {
      PWClearChanged(pixmap_widget);
      FixStatus();
    }
  }
}
