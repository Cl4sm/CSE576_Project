void DoResize()
{
  char x;
  int width, height;

  format = "";
 RetryResize:
  if (PopupDialog(input_dialog, "Resize to WIDTHxHEIGHT:", 
		  format, &format, XtGrabExclusive) == Okay) {
    sscanf(format, "%d%c%d", &width, &x, &height);
    if ((width >0) && (height > 0) && (x == 'x')) {
      PWStoreToBuffer(pixmap_widget);
      PWResize(pixmap_widget, (Dimension)width, (Dimension)height);
      PWChangeNotify(pixmap_widget, NULL, NULL);
      PWSetChanged(pixmap_widget);
      FixStatus();
    }
    else {
      sprintf(message, "Wrong format: %s", format);
      if (PopupDialog(error_dialog, message, 
		      NULL, NULL, XtGrabExclusive) == Retry)
	goto RetryResize;
    }
  }
}
