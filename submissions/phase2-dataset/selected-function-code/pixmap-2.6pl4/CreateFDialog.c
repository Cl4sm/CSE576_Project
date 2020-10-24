     Widget top_widget;
     String name;
     int options;
{
    Dialog popup;
#ifndef USE_ATHENA
    int i;
    Arg wargs[2];
    XmString xmstr;
#endif /* USE_ATHENA */

    if ((popup = (Dialog) XtMalloc(sizeof(_Dialog))) != (Dialog) 0) {
	popup->top_widget = top_widget;
	popup->options = options;
#ifndef USE_ATHENA
	xmstr = XmStringCreateLtoR("*.xpm", XmSTRING_DEFAULT_CHARSET);
	XtSetArg(wargs[0], XmNdirMask, xmstr);
	popup->dialog_widget = XmCreateFileSelectionDialog(top_widget, name, 
							   wargs, 1);
	XmStringFree(xmstr);
	popup->shell_widget = XtParent(popup->dialog_widget);

	XtUnmanageChild(XmFileSelectionBoxGetChild(popup->dialog_widget,
					       XmDIALOG_HELP_BUTTON));
	XtUnmanageChild(XmFileSelectionBoxGetChild(popup->dialog_widget,
					       XmDIALOG_OK_BUTTON));
	XtUnmanageChild(XmFileSelectionBoxGetChild(popup->dialog_widget,
					       XmDIALOG_CANCEL_BUTTON));
	
	for (i = 0; i < XtNumber(dialog_buttons); i++)
	    if (options & dialog_buttons[i].flag)
	      switch (dialog_buttons[i].flag)
		{
		case Okay:
		case Yes:
		case Abort:
		  XtSetArg(wargs[0], XmNlabelString, 
			   XmStringCreateLtoR(dialog_buttons[i].name,
					      XmSTRING_DEFAULT_CHARSET));
		  XtSetValues(XmFileSelectionBoxGetChild(
						       popup->dialog_widget,
						       XmDIALOG_OK_BUTTON),
			      wargs, 1);
		  XtManageChild(XmFileSelectionBoxGetChild(
						       popup->dialog_widget,
						       XmDIALOG_OK_BUTTON));
		  XtRemoveAllCallbacks(XmFileSelectionBoxGetChild(
						       popup->dialog_widget,
						       XmDIALOG_OK_BUTTON),
                                       XmNactivateCallback);
		  XtAddCallback(XmFileSelectionBoxGetChild(
						       popup->dialog_widget,
						       XmDIALOG_OK_BUTTON),
				XmNactivateCallback, SetSelected, 
				(XtPointer)dialog_buttons[i].flag);
		  break;
		case No:
		  XtSetArg(wargs[0], XmNlabelString, 
			   XmStringCreateLtoR(dialog_buttons[i].name,
					      XmSTRING_DEFAULT_CHARSET));
		  XtSetValues(XmFileSelectionBoxGetChild(
						       popup->dialog_widget,
						       XmDIALOG_HELP_BUTTON),
			      wargs, 1);
		  XtManageChild(XmFileSelectionBoxGetChild(
						       popup->dialog_widget,
						       XmDIALOG_HELP_BUTTON));
		  XtRemoveAllCallbacks(XmFileSelectionBoxGetChild(
						       popup->dialog_widget,
						       XmDIALOG_HELP_BUTTON),
                                       XmNactivateCallback);
		  XtAddCallback(XmFileSelectionBoxGetChild(
						       popup->dialog_widget,
						       XmDIALOG_HELP_BUTTON),
				XmNactivateCallback, SetSelected, 
				(XtPointer)dialog_buttons[i].flag);
		  break;
		case Cancel:
		case Retry:
		  XtSetArg(wargs[0], XmNlabelString, 
			   XmStringCreateLtoR(dialog_buttons[i].name,
					      XmSTRING_DEFAULT_CHARSET));
		  XtSetValues(XmFileSelectionBoxGetChild(
						      popup->dialog_widget,
						      XmDIALOG_CANCEL_BUTTON),
			      wargs, 1);
		  XtManageChild(XmFileSelectionBoxGetChild(
						      popup->dialog_widget,
						      XmDIALOG_CANCEL_BUTTON));
		  XtRemoveAllCallbacks(XmFileSelectionBoxGetChild(
						       popup->dialog_widget,
						       XmDIALOG_CANCEL_BUTTON),
                                       XmNactivateCallback);
		  XtAddCallback(XmFileSelectionBoxGetChild(
						      popup->dialog_widget,
						      XmDIALOG_CANCEL_BUTTON),
				XmNactivateCallback, SetSelected, 
				(XtPointer)dialog_buttons[i].flag);
		  break;
		}
#endif /* USE_ATHENA */
	return popup;
    }
    else
	return NULL;
}
