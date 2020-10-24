{
    int n;
    Arg wargs[2];
    String str;
#ifndef USE_ATHENA
    char *fn;
    XmString xmstr;
#endif /* USE_ATHENA */

    str = PWUnparseStatus(pixmap_widget);

    n = 0;
#ifndef USE_ATHENA
    fn = str;
    while ((*str != ' ') || (*(str-1) == ':')) str++;
    *str = 0;
    xmstr = XmStringCreateLtoR(fn, XmSTRING_DEFAULT_CHARSET);
    XtSetArg(wargs[n], XmNlabelString, xmstr); n++;
    XtSetValues(status_widget, wargs, n);
    XmStringFree(xmstr);
    *str++ = ' ';
    n = 0;
    xmstr = XmStringCreateLtoR(str, XmSTRING_DEFAULT_CHARSET);
    XtSetArg(wargs[n], XmNlabelString, xmstr); n++;
    XtSetValues(statusb_widget, wargs, n);
    XmStringFree(xmstr);
#else /* USE_ATHENA */
    XtSetArg(wargs[n], XtNlabel, str); n++;
    XtSetValues(status_widget, wargs, n);
#endif /* USE_ATHENA */
}
