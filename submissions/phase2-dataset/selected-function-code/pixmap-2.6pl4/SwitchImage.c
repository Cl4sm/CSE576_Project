void SwitchImage()
{
    if (image_visible) {
      XtPopdown(image_shell);
      image_visible = False;
    }
    else {
      Position image_x, image_y;
      int n;
      Arg wargs[3];
      
      XtTranslateCoords(XtParent(pixmap_widget), 
			10, 10, &image_x, &image_y);
      
      n = 0;
      XtSetArg(wargs[n], XtNx, image_x); n++;
      XtSetArg(wargs[n], XtNy, image_y); n++;
      XtSetValues(image_shell, wargs, n);
      
      image_visible = True;
      
      XtPopup(image_shell, XtGrabNone);
      unsetKillfromWM(image_shell);
      FixImage(pixmap_widget, NULL, NULL);
    }
}
