{
  PWGetFilepath(pixmap_widget, &filename);
 RetryInsert:
#ifndef USE_ATHENA
  if (PopupFDialog(file_dialog, "Insert file:", 
		  filename, &filename, XtGrabExclusive) == Okay) {
#else /* USE_ATHENA */
  if (PopupFDialog(file_dialog, "Insert file:", 
		  filename, &filename, "r") == Okay) {
#endif  /* USE_ATHENA */
    if (PWStoreFile(pixmap_widget, filename) != XpmSuccess) {
      sprintf(message, "Can't read file: %s", filename);
      if (PopupDialog(error_dialog, message, 
		      NULL, NULL, XtGrabExclusive) == Retry)
	goto RetryInsert;
    }
    else {
      PWEngageRequest(pixmap_widget, RestoreRequest, False, Plain);
    }
  }
}
