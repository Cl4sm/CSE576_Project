void
adjustGeom(Context, which)
struct Sundata * Context;
int which;
{
        int x, y, dx=0, dy=0, diff;
        unsigned int w, h;

        if (getPlacement(Context->win, &x, &y, &w, &h)) return;

	if (which) {
	   diff= MapGeom.height - ClockGeom.height +
	         Context->gdata->mapstrip - Context->gdata->clockstrip;
	   switch(placement) {
	      case CENTER:
		 dx = (MapGeom.width - ClockGeom.width)/2;
	         dy = diff/2;
	         break;
	      case NE:
                 dx = MapGeom.width - ClockGeom.width;
	         break;
	      case SE:
                 dx = MapGeom.width - ClockGeom.width;
	      case SW:
	         dy = diff;
	         break;	
	      default:
	         break;
	   }
	}

        if (placement) {
	    dx = dx - place_shiftx;
	    dy = dy - place_shifty;
	    if (Context->wintype) {
	       dx = -dx;
	       dy = -dy;
	    }
	    if (placement >= CENTER) {
	       Context->geom.x = x + dx - horiz_drift;
	       Context->geom.y = y + dy - vert_drift;
	       checkGeom(Context, 1);
	    }
	}
}
