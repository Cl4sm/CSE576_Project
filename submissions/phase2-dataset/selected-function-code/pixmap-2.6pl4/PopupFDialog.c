int PopupFDialog(popup, message, suggestion, answer, grab)
    Dialog popup;
    String message, suggestion, *answer;
    XtGrabKind grab;
{
  Position popup_x, popup_y, top_x, top_y;
  Dimension popup_width, popup_height, top_width, top_height, border_width;
  int n;
  Arg wargs[4];

  n = 0;
  XtSetArg(wargs[n], XmNselectionLabelString, 
	   XmStringCreateLtoR(message, XmSTRING_DEFAULT_CHARSET)); n++;
  if (suggestion)
    {
      XtSetArg(wargs[n], XmNtextString, 
	       XmStringCreateLtoR(suggestion, XmSTRING_DEFAULT_CHARSET)); n++;
      XtManageChild(XmFileSelectionBoxGetChild(popup->dialog_widget, 
					   XmDIALOG_TEXT));
      XtRemoveAllCallbacks(XmFileSelectionBoxGetChild(popup->dialog_widget,
						      XmDIALOG_TEXT),
                           XmNactivateCallback);
      XtAddCallback(XmFileSelectionBoxGetChild(popup->dialog_widget,
					       XmDIALOG_TEXT),
		    XmNactivateCallback, SetSelected, (XtPointer)(Okay | Yes));
    }
  else XtUnmanageChild(XmFileSelectionBoxGetChild(popup->dialog_widget, 
					      XmDIALOG_TEXT));
  XtSetValues(popup->dialog_widget, wargs, n);
  
  n = 0;
  XtSetArg(wargs[n], XtNx, &top_x); n++;
  XtSetArg(wargs[n], XtNy, &top_y); n++;
  XtSetArg(wargs[n], XtNwidth, &top_width); n++;
  XtSetArg(wargs[n], XtNheight, &top_height); n++;
  XtGetValues(popup->top_widget, wargs, n);

  n = 0;
  XtSetArg(wargs[n], XtNwidth, &popup_width); n++;
  XtSetArg(wargs[n], XtNheight, &popup_height); n++;
  XtSetArg(wargs[n], XtNborderWidth, &border_width); n++;
  XtGetValues(popup->shell_widget, wargs, n);

  popup_x = max(0, 
		min(top_x + ((Position)top_width - (Position)popup_width) / 2, 
		    (Position)DisplayWidth(XtDisplay(popup->shell_widget), 
			       DefaultScreen(XtDisplay(popup->shell_widget))) -
		    (Position)popup_width - 2 * (Position)border_width));
  popup_y = max(0, 
		min(top_y+((Position)top_height - (Position)popup_height) / 2,
		    (Position)DisplayHeight(XtDisplay(popup->shell_widget), 
			       DefaultScreen(XtDisplay(popup->shell_widget))) -
		    (Position)popup_height - 2 * (Position)border_width));
  n = 0;
  XtSetArg(wargs[n], XtNx, popup_x); n++;
  XtSetArg(wargs[n], XtNy, popup_y); n++;
  XtSetValues(popup->shell_widget, wargs, n);

  selected = Empty;

  XtManageChild(popup->dialog_widget);
  unsetKillfromWM(popup->shell_widget);

#ifndef sparc
  XtSetKeyboardFocus(popup->top_widget, popup->shell_widget);
#endif /* sparc */
  
  while ((selected & popup->options) == Empty) {
      XEvent event;
      XtAppNextEvent(
		 XtDisplayToApplicationContext(XtDisplay(popup->shell_widget)),
		 &event);
      XtDispatchEvent(&event);
  }

  PopdownFDialog(popup, answer);

  return (selected & popup->options);
}
