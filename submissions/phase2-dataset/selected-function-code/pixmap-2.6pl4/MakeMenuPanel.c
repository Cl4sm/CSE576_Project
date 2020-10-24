void MakeMenuPanel(father, menu, num, callback)
     Widget father;
     ButtonRec	   menu[];
     Cardinal       num;
     XtCallbackProc callback;
{
    Widget	bw;
    int		i;

    for (i = 0; i < num; i++) {
#ifndef USE_ATHENA
	if (menu[i].id == Dummy)
	  bw = XmCreateSeparatorGadget(father, menu[i].name, NULL, 0);
	else if (menu[i].trap == TOGGLE)
	  {
	      bw = XmCreateToggleButtonGadget(father, menu[i].name, NULL, 0);
	      XtAddCallback(bw, XmNvalueChangedCallback, callback,
			    (XtPointer)&menu[i].id);
	  }
	else
	  {
	      bw =  XmCreatePushButtonGadget(father, menu[i].name, NULL, 0);
	      XtAddCallback(bw, XmNactivateCallback, callback,
			    (XtPointer)&menu[i].id);
	  }
	menu[i].widget = bw;
	XtManageChild(bw);
#else /* USE_ATHENA */
        bw = XtCreateManagedWidget( menu[i].name,
                                    ((menu[i].id != Dummy) ?
                                      smeBSBObjectClass : smeLineObjectClass),
                                    father, NULL, 0);
        XtAddCallback( bw, XtNcallback, callback, (XtPointer) &menu[i].id );
        menu[i].widget = bw;
#endif /* USE_ATHENA */
    }
}
