SFpositionWidget(w)
    Widget w;
{
    Arg args[3];
    Cardinal num_args;
    Dimension width, height, b_width;
    int x, y, max_x, max_y;
    Window root, child;
    int dummyx, dummyy;
    unsigned int dummymask;
    
    XQueryPointer(XtDisplay(w), XtWindow(w), &root, &child, &x, &y,
		  &dummyx, &dummyy, &dummymask);
    num_args = 0;
    XtSetArg(args[num_args], XtNwidth, &width); num_args++;
    XtSetArg(args[num_args], XtNheight, &height); num_args++;
    XtSetArg(args[num_args], XtNborderWidth, &b_width); num_args++;
    XtGetValues(w, args, num_args);

    width += 2 * b_width;
    height += 2 * b_width;

    x -= ( (Position) width/2 );
    if (x < 0) x = 0;
    if ( x > (max_x = (Position) (XtScreen(w)->width - width)) ) x = max_x;

    y -= ( (Position) height/2 );
    if (y < 0) y = 0;
    if ( y > (max_y = (Position) (XtScreen(w)->height - height)) ) y = max_y;
    
    num_args = 0;
    XtSetArg(args[num_args], XtNx, x); num_args++;
    XtSetArg(args[num_args], XtNy, y); num_args++;
    XtSetValues(w, args, num_args);
}
