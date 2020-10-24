int
main(int argc, char *argv[], char *envp[])
{
    int i, n;
    Arg wargs[5];
    Widget w, pview_widget, cview_widget, vpane_widget;
#ifndef USE_ATHENA
    Widget formh;
    static int Resize_id = Resize, Filename_id = Filename;
#else /* USE_ATHENA */
    Widget radio_group; XtPointer radio_data;
#endif /* USE_ATHENA */
    int x, y;
    unsigned int width = 0, height = 0;
    Resources resources;
    
    top_widget = XtAppInitialize(&pixmap_context, "Pixmap", 
				 options, XtNumber(options), 
				 &argc, argv, NULL, NULL, 0);
    XtGetApplicationResources(top_widget, &resources, appResources, 
			      XtNumber(appResources), NULL, 0);	
#ifndef USE_ATHENA
#ifndef NO_EDITRES
    XtAddEventHandler(top_widget, 0, True, _XEditResCheckMessages, NULL);
#endif
#endif /* USE_ATHENA */

    if (argc > 1) {
      if ((argc ==3) && (strcmp(argv[1], "-size") == 0))
	  XParseGeometry(argv[2], &x, &y, &width, &height);
      else
	{
	  fprintf(stderr, "%s %s", argv[0], usage);
	  exit (-1);
	}
    }

    /* some intializations */
    dpy = XtDisplay(top_widget);
    screen = DefaultScreen(dpy);
    ncolors = 1<<DisplayPlanes(dpy, screen);
#ifdef DEBUG
    printf("Num colors %d\n",ncolors);
    printf("Transparent entry in colormap %d\n", TRANSPARENT(dpy, screen));
    XSynchronize(dpy, True);
#endif /* DEBUG */

    if (resources.privateColormap) {
	XColor xcolor;
	/* create a private colormap */
	cmap = XCreateColormap(dpy, DefaultRootWindow(dpy),
			       DefaultVisual(dpy, screen), AllocNone);
	/* allocate black and white  */
	xcolor.red = 0xffff;
	xcolor.blue = 0xffff;
	xcolor.green = 0xffff;
	XAllocColor(dpy, cmap, &xcolor);
	white = xcolor.pixel;
	xcolor.red = 0;
	xcolor.blue = 0;
	xcolor.green = 0;
	XAllocColor(dpy, cmap, &xcolor);
	black = xcolor.pixel;

	n = 0;
	XtSetArg(wargs[n], XtNcolormap, cmap); n++;
	XtSetValues(top_widget, wargs, n);
    } else {
	cmap = DefaultColormap(dpy,screen);
	white = WhitePixel(dpy,screen);
	black = BlackPixel(dpy,screen);
    }

#ifdef USE_ATHENA
    check_mark = XCreateBitmapFromData(XtDisplay(top_widget),
				      RootWindowOfScreen(XtScreen(top_widget)),
				      xlogo16_bits, 
				      xlogo16_width, 
				      xlogo16_height);
#else
#if XmVersion > 1001 /* Motif 1.2 or > */
    /* Disable Motif 1.2 drag and drop */
    XtVaSetValues(XmGetXmDisplay(dpy), XmNdragInitiatorProtocolStyle, 
		  XmDRAG_NONE, NULL);
#endif
#endif /* USE_ATHENA */
    XtAppAddActions(pixmap_context, actions_table, XtNumber(actions_table));
#ifndef USE_ATHENA
    parent_widget = XtCreateWidget("parent", xmFormWidgetClass,
				   top_widget, NULL, 0);
    formy_widget = XmCreateMenuBar(parent_widget, "formy", NULL, 0);
    
#else /* USE_ATHENA */

    parent_widget = XtCreateWidget("parent", panedWidgetClass,
				   top_widget, NULL, 0);

    formy_widget = XtCreateWidget("formy", formWidgetClass,
				  parent_widget, NULL, 0);

#endif /* USE_ATHENA */

    MakeMenu(formy_widget, "fileMenu", "fileButton",
	     file_menu, XtNumber(file_menu), TheCallback);
        
    MakeMenu(formy_widget, "editMenu", "editButton",
             edit_menu, XtNumber(edit_menu), TheCallback );

    MakeMenu(formy_widget, "colorMenu", "colorButton",
             color_menu, XtNumber(color_menu), TheCallback );

    extensionMenu_widget = 
      MakeMenu(formy_widget, "extensionMenu", "extensionButton",
               extension_menu, XtNumber(extension_menu),
	       ExtensionMenuCallback);

#ifndef USE_ATHENA
    status_widget = XmCreateCascadeButtonGadget(formy_widget, "status",
				                NULL, 0);
    XtManageChild(status_widget);
    XtAddCallback(status_widget, XmNactivateCallback, TheCallback, 
                  &Filename_id);
    statusb_widget = XmCreateCascadeButtonGadget(formy_widget, "statusb",
			 	                 NULL, 0);
    XtManageChild(statusb_widget);
    XtAddCallback(statusb_widget, XmNactivateCallback, TheCallback, 
                  &Resize_id);


    infoButton_widget = XmCreateCascadeButtonGadget(formy_widget, 
						 "infoButton", NULL, 0);
    XtManageChild(infoButton_widget);
    XtVaSetValues(formy_widget, 
		  XmNmenuHelpWidget, infoButton_widget,
		  NULL, NULL);
    XtAddCallback(infoButton_widget, XmNactivateCallback, (XtCallbackProc)InfoCallback,
		  NULL);

    n = 0;
    XtSetArg(wargs[n], XmNtopAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(wargs[n], XmNtopWidget, formy_widget); n++;
    pane_widget = XmCreateForm(parent_widget, "pane", wargs, n);

    form_widget = XmCreateRadioBox(pane_widget, "form", NULL, 0);
    XtSetArg(wargs[0], XmNisHomogeneous, False);
    XtSetValues(form_widget, wargs, 1);
        
    for (i = 0; i < XtNumber(buttons); i++) {
      if (buttons[i].trap == TOGGLE)
	{
	  w = XmCreateToggleButtonGadget(form_widget, buttons[i].name, 
					 NULL, 0);
	  XtAddCallback(w, XmNvalueChangedCallback, TheCallback, 
			&buttons[i].id);
	}
      else 
	{
	  if ((!strcmp(buttons[i].name, "flipHoriz")) ||
	      (!strcmp(buttons[i].name, "left")) ||
	      (!strcmp(buttons[i].name, "rotateLeft")))
	    {
	      formh = form_widget;
	      form_widget = XmCreateRowColumn(formh, "formh", NULL, 0);
	      XtManageChild(form_widget);
	    }
	  
	  w = XmCreatePushButtonGadget(form_widget, buttons[i].name, NULL, 0);
	  XtAddCallback(w, XmNactivateCallback, TheCallback, &buttons[i].id);

	  if ((!strcmp(buttons[i].name, "flipVert")) ||
	      (!strcmp(buttons[i].name, "right")) ||
	      (!strcmp(buttons[i].name, "rotateRight")))
	    form_widget = formh;
	}

      buttons[i].widget = w;
      XtManageChild(w);
      
      if (buttons[i].id == Point) 
	XmToggleButtonGadgetSetState(buttons[i].widget, True, False);
    }

    n = 0;
    XtSetArg(wargs[n], XmNleftAttachment, XmATTACH_WIDGET); n++;
    XtSetArg(wargs[n], XmNleftWidget, form_widget); n++;
    vpane_widget = (Widget) XmCreatePanedWindow(pane_widget, "vPane", 
						wargs, n);

    /* the color pane should be created before the pixmap widget as the
       creation of the pixmap widget will ``add'' a new color in the pane */
    n = 0;
    cview_widget = (Widget)XmCreateScrolledWindow(vpane_widget, "colorView",
						 wargs, n);
    n = 0;
    color_pane_widget = XmCreateRowColumn(cview_widget, "colorPane",
					  wargs, n);
    XmScrolledWindowSetAreas(cview_widget,
			     XmCreateScrollBar(cview_widget, "cviewHS",
					       NULL, 0),
			     XmCreateScrollBar(cview_widget, "cviewVS",
					       NULL, 0),
			     color_pane_widget);

    n = 0;
    pview_widget = (Widget)XmCreateScrolledWindow(vpane_widget, "pixmapView",
						 wargs, n);

#else /* USE_ATHENA */

    infoButton_widget = XtCreateManagedWidget("infoButton",
					commandWidgetClass, 
					formy_widget, NULL, 0);
    status_widget = XtCreateManagedWidget("status", labelWidgetClass,
					  formy_widget, NULL, 0);

    XtAddCallback(infoButton_widget, XtNcallback, InfoCallback, NULL);
    pane_widget = XtCreateWidget("pane", panedWidgetClass,
				 parent_widget, NULL, 0);

    form_widget = XtCreateWidget("form", formWidgetClass, 
				 pane_widget, NULL, 0);
        
    for (i = 0; i < XtNumber(buttons); i++) {
	w = XtCreateManagedWidget(buttons[i].name, 
				  ((buttons[i].trap == TOGGLE) ? 
				   toggleWidgetClass : commandWidgetClass),
				  form_widget, NULL, 0);
	XtAddCallback(w,
		      XtNcallback,
		      TheCallback,
		      &buttons[i].id);

	buttons[i].widget = w;

	if (buttons[i].id == Point) {
	    radio_group = buttons[i].widget;
	    radio_data  = buttons[i].name;
	}
    }

    vpane_widget = XtCreateWidget("vPane", panedWidgetClass,
				  pane_widget, NULL, 0);

    /* the color pane should be created before the pixmap widget as the
       creation of the pixmap widget will ``add'' a new color in the pane */
    cview_widget = XtCreateWidget("colorView", viewportWidgetClass,
				  vpane_widget, NULL, 0);

    color_pane_widget = XtCreateWidget("colorPane", boxWidgetClass,
				       cview_widget, NULL, 0);

    pview_widget = XtCreateWidget("pixmapView", viewportWidgetClass,
				  vpane_widget, NULL, 0);
#endif /* USE_ATHENA */

    /* foreach extension editor, create and manage editor buttons */ 
    CreatePortEditorButtons(form_widget,
#ifndef USE_ATHENA
			    NULL);
#else /* USE_ATHENA */
			    radio_group);
#endif /* USE_ATHENA */
    ManagePortEditorButtons(True); 

    n = 0;
    XtSetArg(wargs[n], XtNaddColorNtfyProc,
	     (AddColorNotifyProc)colorToAdd); n++;
    XtSetArg(wargs[n], XtNextensionNtfyProc,
	     (ExtensionNotifyProc)FixExtensionMenu); n++;
    if (width) XtSetArg(wargs[n], XtNpixmapWidth, width), n++;
    if (height) XtSetArg(wargs[n], XtNpixmapHeight, height), n++;
    pixmap_widget = XtCreateManagedWidget("pixmap", pixmapWidgetClass,
					  pview_widget, wargs, n);

    /* register extension callbacks with pixmap widget */
    PWSetDrawPointProc(pixmap_widget, DrawPointExtensions);
    PWSetRedrawProc(   pixmap_widget, DrawExtensions);
    PWSetTranslateProc(pixmap_widget, TranslateExtensions);
    PWSetRotateProc(   pixmap_widget, RotateExtensions);
    PWSetFlipProc(     pixmap_widget, FlipExtensions);

#ifndef USE_ATHENA

    XmScrolledWindowSetAreas(pview_widget,
			     XmCreateScrollBar(pview_widget, "pviewHS",
					       NULL, 0),
			     XmCreateScrollBar(pview_widget, "pviewVS",
					       NULL, 0),
			     pixmap_widget);

#endif /* USE_ATHENA */ 

    XtManageChild(formy_widget);
    XtManageChild(form_widget);
    XtManageChild(color_pane_widget);
    XtManageChild(pview_widget);
    XtManageChild(cview_widget);
    XtManageChild(vpane_widget);
    XtManageChild(pane_widget);
    XtManageChild(parent_widget);

    XtSetKeyboardFocus(top_widget, pixmap_widget);
    
    PWSetForeground(pixmap_widget, black);
    current_color = black;

    loadPixEditColors();
    
    image_shell = XtCreatePopupShell("image", transientShellWidgetClass,
				     top_widget, NULL, 0);
#ifndef USE_ATHENA
    XtSetArg(wargs[0], XmNlabelString, 
	     XmStringCreateLtoR("", XmSTRING_DEFAULT_CHARSET));
    image_widget = XmCreateLabel(image_shell, "label", wargs, 1);
    XtManageChild(image_widget);
#else /* USE_ATHENA */
    image_widget = XtCreateManagedWidget("label", labelWidgetClass,
					 image_shell, NULL, 0);
#endif /* USE_ATHENA */

    Notify(pixmap_widget, FixImage);
    ColorNotify(pixmap_widget, (UseColorNotifyProc)FixColor);

    FixStatus();
    
    info_dialog  = CreateDialog(top_widget, "info", Okay);
    input_dialog = CreateDialog(top_widget, "input", Okay | Cancel);
    error_dialog = CreateDialog(top_widget, "error", Abort | Retry);    
    qsave_dialog = CreateDialog(top_widget, "qsave", Yes | No | Cancel);
    file_dialog  = CreateFDialog(top_widget, "file", Okay | Cancel);

    extensionEditor = CreateExtensionEditor(top_widget, "extEditor");

    textOptions = CreateTextOptions(top_widget, "textOptions");
    PWSetFont(pixmap_widget, textOptions->font_struct);
    PWSetText(pixmap_widget, textOptions->text_string);

#ifdef USE_ATHENA
    XawToggleSetCurrent(radio_group, radio_data);
#endif /* USE_ATHENA */
    PWEngageRequest(pixmap_widget, PointRequest, True, Plain);

    XtRealizeWidget(top_widget);
    XtAppMainLoop(pixmap_context);
    return(0);
}
