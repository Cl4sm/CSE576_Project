void FixColor(w, current)
     Widget w;
     Pixel current;
{
  Widget colorButton;
  Arg wargs[1];
  PWColorInfo *color;
#ifndef USE_ATHENA
  XmString xmstr;
#endif /* USE_ATHENA */

  current_color = current;
  color = PWGetColor(w, current);

#ifndef USE_ATHENA
  xmstr = XmStringCreateLtoR(color->c_name, CHARSET);
  XtSetArg(wargs[0], XmNlabelString, xmstr);
#else /* USE_ATHENA */  
  XtSetArg(wargs[0], XtNlabel, color->c_name);
#endif /* USE_ATHENA */

  colorButton = XtNameToWidget( formy_widget, "colorButton" );
  XtSetValues( colorButton, wargs, 1 );
#ifndef USE_ATHENA
  XmStringFree(xmstr);
#endif /* USE_ATHENA */
}
