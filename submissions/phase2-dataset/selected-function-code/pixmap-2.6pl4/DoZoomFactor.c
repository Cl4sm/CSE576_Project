{
    Dimension square_size;
    Arg args[2];
    char message[80], buffer[80], *returned;

    XtSetArg(args[0], XtNsquareSize, &square_size);
    XtGetValues(pixmap_widget, args, 1);
    sprintf(buffer, "%d", square_size);
    strcpy(message, "Set Zoom Factor:");
    if ((PopupDialog(input_dialog, message, 
		     buffer, &returned, XtGrabExclusive) == Okay) &&
	(returned))
      {
	  if ((square_size = atoi(returned)) > 0)
	    {
		XtSetArg(args[0], XtNsquareSize, square_size);
		XtSetValues(pixmap_widget, args, 1);
#ifdef USE_ATHENA
		PWRedraw(pixmap_widget);
#endif /* USE_ATHENA */
	    }
      }
}
