static Widget
MakeMenu( menubar, menuName, buttonName, menu, num, callback )
    Widget	   menubar;
    String	   menuName;
    String	   buttonName;
    ButtonRec	   menu[];
    Cardinal       num;
    XtCallbackProc callback;
{
    Widget	mw;
    Widget	bw;

#ifndef USE_ATHENA
    Arg wargs[1];

    mw = XmCreatePulldownMenu(menubar, menuName, NULL, 0);

    XtSetArg(wargs[0], XmNsubMenuId, mw);
    bw = XmCreateCascadeButtonGadget(menubar, buttonName, wargs, 1);
    XtManageChild(bw);
    if (menu == edit_menu) 
      XtAddCallback(bw, XmNcascadingCallback, FixMenuCB, NULL);
#else /* USE_ATHENA */
    mw = XtCreatePopupShell( menuName, simpleMenuWidgetClass,
                             menubar, NULL, 0 );

    bw = XtCreateManagedWidget( buttonName, menuButtonWidgetClass, 
                                menubar, NULL, 0 );
#endif /* USE_ATHENA */
    
    MakeMenuPanel(mw, menu, num, callback);

    return mw;
}
