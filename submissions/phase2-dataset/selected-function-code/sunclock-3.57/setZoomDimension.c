void
setZoomDimension(Context)
struct Sundata * Context;
{
    double rx, ry;

    setZoomAspect(Context, 3);

    Context->newzoom.width = (int) 
       ((double) Context->geom.width * Context->newzoom.fx + 0.25);
    Context->newzoom.height = (int)
       ((double) Context->geom.height * Context->newzoom.fy + 0.25);

    rx = 0.5/Context->newzoom.fx;
    ry = 0.5/Context->newzoom.fy;
    Context->newzoom.dx = (int) 
       ((double) Context->newzoom.width * (Context->newzoom.fdx-rx) + 0.25);
    Context->newzoom.dy = (int)
       ((double) Context->newzoom.height * (Context->newzoom.fdy-ry) + 0.25);

    if (Context->newzoom.dx < 0) Context->newzoom.dx = 0;
    if (Context->newzoom.dy < 0) Context->newzoom.dy = 0;
    if (Context->newzoom.dx+Context->geom.width>Context->newzoom.width)
        Context->newzoom.dx = Context->newzoom.width - Context->geom.width;
    if (Context->newzoom.dy+Context->geom.height>Context->newzoom.height)
        Context->newzoom.dy = Context->newzoom.height - Context->geom.height;

    if (verbose && !button_pressed)
       fprintf(stderr, "Zoom (%.2f, %.2f)  centering at (%.2f, %.2f)\n",
	  Context->newzoom.fx, Context->newzoom.fy, 
          Context->newzoom.fdx, Context->newzoom.fdy);
}
