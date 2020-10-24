    Widget	w;		/* apply button */
    XtPointer	client_data;	/* TextOptions* */
    XtPointer	call_data;	/* unused */
{
    TextOptions *text_options = (TextOptions *) client_data;
    String	new_font_name = NULL;
    String	new_text_string = NULL;
    
    /* get new font name and new text string */
#ifndef USE_ATHENA
    /* Big hack for Motif: avoid arbitrary resizing problems */
    Dimension width, height;
    XtVaGetValues(text_options->popup, XmNwidth, &width, XmNheight, &height,
		  NULL);
    
    new_font_name = (String) XmTextFieldGetString(text_options->font);
    new_text_string = (String) XmTextFieldGetString(text_options->text);
    
#else
    XtVaGetValues( text_options->font, 
                   XtNstring, &new_font_name, 
                   NULL );
    new_font_name = XtNewString( new_font_name );

    XtVaGetValues( text_options->text, 
                   XtNstring, &new_text_string, 
                   NULL );
    new_text_string = XtNewString( new_text_string );
#endif

    /* if new font name != current font name, ... */

    if (strcmp( new_font_name, text_options->font_name )) {

        Display     *dpy      = XtDisplay(text_options->popup);
        XFontStruct *new_font = XLoadQueryFont( dpy, new_font_name );

        if (new_font) {
#ifndef USE_ATHENA
	    XmString xmstr;

	    xmstr = XmStringCreateLtoR("Valid font, text ready to paste",
				       XmSTRING_DEFAULT_CHARSET);
	    XtVaSetValues(text_options->status, XmNlabelString, xmstr, NULL);
	    XmStringFree(xmstr);
	    
	    /* Big hack for Motif: avoid arbitrary resizing problems */
	    XtVaSetValues(text_options->shell, XmNwidth, width,
			  XmNheight, height, NULL);
#else    
            XtVaSetValues(text_options->status, 
			  XtNlabel, "Valid font, text ready to paste",
			  NULL );
#endif
        }
        else {
#ifndef USE_ATHENA
	    XmString xmstr;

	    xmstr = XmStringCreateLtoR("No font with this name!",
				       XmSTRING_DEFAULT_CHARSET);
	    XtVaSetValues(text_options->status, XmNlabelString, xmstr, NULL);
	    XmStringFree(xmstr);
	    
	    /* Big hack for Motif: avoid arbitrary resizing problems */
	    XtVaSetValues(text_options->shell, XmNwidth, width,
			  XmNheight, height, NULL);
#else    
            XtVaSetValues( text_options->status, 
                           XtNlabel, "No font with this name!", 
                           NULL );
#endif
        }

        if (text_options->font_struct)
            XFreeFont( dpy, text_options->font_struct );
        XtFree( text_options->font_name );

        text_options->font_struct = new_font;
        text_options->font_name   = XtNewString( new_font_name );

        PWSetFont( pixmap_widget, text_options->font_struct );
    }

    /* if new text string != current text string, ... */
    if (strcmp( new_text_string, text_options->text_string ) != 0) {

        XtFree( text_options->text_string );
        text_options->text_string = XtNewString( new_text_string );
        PWSetText( pixmap_widget, text_options->text_string );
    }

#ifndef USE_ATHENA
#endif

    XtFree( new_font_name );
    XtFree( new_text_string );
}
