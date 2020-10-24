     Widget w;
     XtPointer client_data, call_data;
{
    Pixmap image, image_mask;
    Window root;
    unsigned int width, height, border_width, depth;
    int x, y;

    if (!image_visible)
      {
#ifndef USE_ATHENA
	XmToggleButtonGadgetSetState(edit_menu[0].widget, False, False);
#endif /* USE_ATHENA */
	return;
      }
#ifndef USE_ATHENA
    else XmToggleButtonGadgetSetState(edit_menu[0].widget, True, False);
#endif /* USE_ATHENA */
    
    PWGetUnzoomedPixmap(pixmap_widget, &image, &image_mask);
    XGetGeometry(XtDisplay(image_widget), image, &root, &x, &y, 
		 &width, &height, &border_width, &depth);
    XResizeWindow(XtDisplay(image_widget), XtWindow(image_shell), 
		  width, height);
    
    XSetWindowBackgroundPixmap(XtDisplay(pixmap_widget), 
			       XtWindow(image_widget), image); 

    if (image_mask) 
      {
	XShapeCombineMask(XtDisplay(image_widget), 
			  XtWindow(image_widget), ShapeBounding,
			  0, 0, image_mask, ShapeSet);
	XShapeCombineMask(XtDisplay(pixmap_widget), 
			  XtWindow(image_shell), ShapeBounding,
			  0, 0, image_mask, ShapeSet);
      }
    else 
      {
	XShapeCombineMask(XtDisplay(image_widget), 
			  XtWindow(image_widget), ShapeBounding,
			  0, 0, None, ShapeSet);
	XShapeCombineMask(XtDisplay(pixmap_widget), 
			  XtWindow(image_shell), ShapeBounding,
			  0, 0, None, ShapeSet);
      }

    XClearWindow(XtDisplay(pixmap_widget), XtWindow(image_widget));
    XFreePixmap(XtDisplay(pixmap_widget), image);
    if (image_mask) XFreePixmap(XtDisplay(image_widget), image_mask);
}
