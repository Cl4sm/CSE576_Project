void
activateZoom(Context, rebuild)
Sundata *Context;
int rebuild;
{ 
    setZoomDimension(Context);
    zoom_newhint = ' ';
    setupZoom(zoom_mode);
    if (rebuild) {
       if (Context->zoom.width!=Context->newzoom.width ||
	   Context->zoom.height!=Context->newzoom.height ||
	   Context->zoom.dx!=Context->newzoom.dx ||
	   Context->zoom.dy!=Context->newzoom.dy) {
          Context->zoom = Context->newzoom;
          shutDown(Context, 0);
          buildMap(Context, Context->wintype, 0);
	  zoom_newhint = ' ';
          setupZoom(zoom_mode);
       } else
          Context->zoom = Context->newzoom;
    }
    if (rebuild>0)
       zoom_mode = 0;
}
