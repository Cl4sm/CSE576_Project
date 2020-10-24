    Widget      button;
    TextOptions *text_options;
{
    Position	   x, y;
    Dimension	   width, height;

    if (!text_options->up) {

        XtVaGetValues( button, 
                       XtNwidth,  &width,
                       XtNheight, &height,
                       NULL );
        
        XtTranslateCoords(button, 
			  (Position) (width / 2), (Position) (height / 2),
			  &x, &y );
    
        XtVaSetValues( text_options->shell, 
                       XtNx, x,
                       XtNy, y,
                       NULL );

#ifndef USE_ATHENA
	XtManageChild(text_options->popup);
#else
	XtPopup(text_options->popup, XtGrabNone);
#endif
    unsetKillfromWM(text_options->popup);
    text_options->up = True;
    } else text_options->up = False;

}
