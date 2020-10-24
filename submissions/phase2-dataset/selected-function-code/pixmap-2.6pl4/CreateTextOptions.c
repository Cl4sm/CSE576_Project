TextOptions* CreateTextOptions(parent, name ) 
    Widget	parent;
    String	name;
{
    Widget	popup, shell, textOptions, 
		title, 
		status, 
		font_label, font, 
		text_label, text, 
		close, apply;
    String	font_name, text_string;
    
    TextOptions *text_options = (TextOptions *)XtCalloc(1,
							sizeof(TextOptions));

#ifdef USE_ATHENA
    XtAppAddActions(pixmap_context, actions, XtNumber(actions));
#endif

#ifndef USE_ATHENA
    textOptions = XmCreateFormDialog(parent, name, NULL, 0);
    popup = textOptions;
    shell = XtParent(popup);
    
    title = XmCreateLabelGadget(textOptions, "title", NULL, 0);
    XtManageChild(title);

    status = XmCreateLabelGadget(textOptions, "fontStatus", NULL, 0);
    XtManageChild(status);

    font_label = XmCreateLabelGadget(textOptions, "fontLabel", NULL, 0);
    XtManageChild(font_label);

    font = (Widget) XmCreateTextField(textOptions, "fontName", NULL, 0);
    XtManageChild(font);
    XtAddCallback(font, XmNactivateCallback, Apply, (XtPointer)text_options);

    text_label = XmCreateLabelGadget(textOptions, "textLabel", NULL, 0);
    XtManageChild(text_label);
    
    text = (Widget) XmCreateTextField(textOptions, "textString", NULL, 0);
    XtManageChild(text);
    XtAddCallback(text, XmNactivateCallback, Apply, (XtPointer)text_options);
    
    close = XmCreatePushButtonGadget(textOptions, "close", NULL, 0);
    XtManageChild(close);
    XtAddCallback(close, XmNactivateCallback, PopdownTextOptions,
		  (XtPointer)text_options);

    apply = XmCreatePushButtonGadget(textOptions, "apply", NULL, 0);
    XtManageChild(apply);
    XtAddCallback(apply, XmNactivateCallback, Apply, (XtPointer)text_options);
    
#else
    popup = XtCreatePopupShell(
			"textOptionsShell", transientShellWidgetClass, 
			parent,
			NULL, 0 );
    shell = popup;
    
    textOptions = XtCreateManagedWidget( 
			"textOptions", formWidgetClass, 
			popup,
			NULL, 0 );

    title = XtCreateManagedWidget(
			"title", labelWidgetClass, 
			textOptions,
			NULL, 0 );

    status = XtCreateManagedWidget(
			"fontStatus", labelWidgetClass, 
			textOptions,
			NULL, 0 );

    font_label = XtCreateManagedWidget(
			"fontLabel", labelWidgetClass, 
			textOptions,
			NULL, 0 );

    font = XtCreateManagedWidget(
			"fontName", asciiTextWidgetClass, 
			textOptions,
			NULL, 0 );

    text_label = XtCreateManagedWidget(
			"textLabel", labelWidgetClass, 
			textOptions,
			NULL, 0 );

    text = XtCreateManagedWidget(
			"textString", asciiTextWidgetClass, 
			textOptions,
			NULL, 0 );

    close = XtCreateManagedWidget(
			"close", commandWidgetClass, 
			textOptions,
			NULL, 0 );
    XtAddCallback(close, XtNcallback, PopdownTextOptions,
		  (XtPointer)text_options);

    apply = XtCreateManagedWidget(
			"apply", commandWidgetClass, 
			textOptions,
			NULL, 0 );
    XtAddCallback(apply, XtNcallback, Apply, (XtPointer) text_options);
#endif

    text_options->up     = False;
    text_options->popup  = popup;
    text_options->shell  = shell;
    text_options->text   = text;
    text_options->font   = font;
    text_options->status = status;

#ifndef USE_ATHENA
    text_options->font_name = (String)XmTextFieldGetString(text_options->font);
#else
    XtVaGetValues( text_options->font, 
                   XtNstring, &font_name, 
                   NULL );
    text_options->font_name = XtNewString( font_name );
#endif
    text_options->font_struct = XLoadQueryFont(XtDisplay(parent),
					       text_options->font_name);

#ifndef USE_ATHENA
    text_options->text_string=(String)XmTextFieldGetString(text_options->text);
#else
    XtVaGetValues( text_options->text, 
                   XtNstring, &text_string, 
                   NULL );
    text_options->text_string = XtNewString(text_string);
#endif

    
    return text_options;
}
