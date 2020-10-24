Dialog CreateDialog(top_widget, name, options)
     Widget top_widget;
     String name;
     int options;
{
    int i;
    Dialog popup;
    Arg wargs[2];

    if (popup = (Dialog) XtMalloc(sizeof(_Dialog))) {
#ifdef USE_ATHENA
	XtAppAddActions(pixmap_context, actions_table,
			XtNumber(actions_table));
#endif /* USE_ATHENA */
	
	popup->top_widget = top_widget;
#ifndef USE_ATHENA
	popup->dialog_widget = XmCreatePromptDialog(top_widget, name, NULL, 0);
	popup->shell_widget = XtParent(popup->dialog_widget);

	XtUnmanageChild(XmSelectionBoxGetChild(popup->dialog_widget,
					       XmDIALOG_HELP_BUTTON));
	XtUnmanageChild(XmSelectionBoxGetChild(popup->dialog_widget,
					       XmDIALOG_OK_BUTTON));
	XtUnmanageChild(XmSelectionBoxGetChild(popup->dialog_widget,
					       XmDIALOG_APPLY_BUTTON));
	XtUnmanageChild(XmSelectionBoxGetChild(popup->dialog_widget,
					       XmDIALOG_CANCEL_BUTTON));
	
	for (i = 0; i < XtNumber(dialog_buttons); i++)
	    if (options & dialog_buttons[i].flag)
	      switch (dialog_buttons[i].flag)
		{
		    XmString xmstr;
		    
		case Okay:
		case Yes:
		case Abort:
		  xmstr = XmStringCreateLtoR(dialog_buttons[i].name,
					     XmSTRING_DEFAULT_CHARSET);
		  XtSetArg(wargs[0], XmNlabelString, xmstr);
		  XtSetValues(XmSelectionBoxGetChild(popup->dialog_widget,
						       XmDIALOG_OK_BUTTON),
			      wargs, 1);
		  XtManageChild(XmSelectionBoxGetChild(popup->dialog_widget,
						       XmDIALOG_OK_BUTTON));
                  XtRemoveAllCallbacks(XmSelectionBoxGetChild(
                                                       popup->dialog_widget,
                                                       XmDIALOG_OK_BUTTON),
                                       XmNactivateCallback);
		  XtAddCallback(XmSelectionBoxGetChild(popup->dialog_widget,
						       XmDIALOG_OK_BUTTON),
				XmNactivateCallback, SetSelected, 
				(XtPointer)dialog_buttons[i].flag);
		  XmStringFree(xmstr);
		  break;
		case No:
		  xmstr = XmStringCreateLtoR(dialog_buttons[i].name,
					     XmSTRING_DEFAULT_CHARSET);
		  XtSetArg(wargs[0], XmNlabelString, xmstr);
		  XtSetValues(XmSelectionBoxGetChild(popup->dialog_widget,
						       XmDIALOG_APPLY_BUTTON),
			      wargs, 1);
		  XtManageChild(XmSelectionBoxGetChild(popup->dialog_widget,
						       XmDIALOG_APPLY_BUTTON));
                  XtRemoveAllCallbacks(XmSelectionBoxGetChild(
                                                       popup->dialog_widget,
                                                       XmDIALOG_APPLY_BUTTON),
                                       XmNactivateCallback);
		  XtAddCallback(XmSelectionBoxGetChild(popup->dialog_widget,
						       XmDIALOG_APPLY_BUTTON),
				XmNactivateCallback, SetSelected, 
				(XtPointer)dialog_buttons[i].flag);
		  XmStringFree(xmstr);
		  break;
		case Cancel:
		case Retry:
		  xmstr = XmStringCreateLtoR(dialog_buttons[i].name,
					     XmSTRING_DEFAULT_CHARSET);
		  XtSetArg(wargs[0], XmNlabelString, xmstr);
		  XtSetValues(XmSelectionBoxGetChild(popup->dialog_widget,
						       XmDIALOG_CANCEL_BUTTON),
			      wargs, 1);
		  XtManageChild(XmSelectionBoxGetChild(popup->dialog_widget,
						      XmDIALOG_CANCEL_BUTTON));
                  XtRemoveAllCallbacks(XmSelectionBoxGetChild(
                                                       popup->dialog_widget,
                                                       XmDIALOG_CANCEL_BUTTON),
                                       XmNactivateCallback);
		  XtAddCallback(XmSelectionBoxGetChild(popup->dialog_widget,
						       XmDIALOG_CANCEL_BUTTON),
				XmNactivateCallback, SetSelected, 
				(XtPointer)dialog_buttons[i].flag);
		  XmStringFree(xmstr);
		  break;
		}
#else /* USE_ATHENA */
	popup->shell_widget = XtCreatePopupShell(name, 
						 transientShellWidgetClass, 
						 top_widget, NULL, 0);
	popup->dialog_widget = XtCreateManagedWidget("dialog", 
						     dialogWidgetClass,
						     popup->shell_widget, 
						     NULL, 0);
	for (i = 0; i < XtNumber(dialog_buttons); i++)
	    if (options & dialog_buttons[i].flag)
		XawDialogAddButton(popup->dialog_widget, 
				   dialog_buttons[i].name, SetSelected, 
                                   (XtPointer)(long)dialog_buttons[i].flag);
#endif /* USE_ATHENA */

	popup->options = options;
	return popup;
    }
    else
	return NULL;
}
