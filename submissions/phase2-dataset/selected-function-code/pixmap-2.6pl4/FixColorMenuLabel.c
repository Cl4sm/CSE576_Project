void FixColorMenuLabel(old_name, name)
     char *old_name;
     char *name;
{
  Widget colorButton;
  Arg wargs[1];
#ifndef USE_ATHENA
  XmString xmstr;

  xmstr = XmStringCreateLtoR(name, CHARSET);
  XtSetArg(wargs[0], XmNlabelString, xmstr);
#else /* USE_ATHENA */
  XtSetArg(wargs[0], XtNlabel, name);
#endif /* USE_ATHENA */

  colorButton = XtNameToWidget( formy_widget, "colorButton" );
  XtSetValues( colorButton, wargs, 1 );

#ifndef USE_ATHENA
  XmStringFree(xmstr);
#endif /* USE_ATHENA */
}
