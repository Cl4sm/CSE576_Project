	  unparse_coord(pl->y_type, pl_y_top));
#endif
}

void draw_pointer_marks(PLOTTER pl, GC gc)
{
  const int x_pm_l = 6;

#if 0
  printf("draw_pointer_marks, pl->state is %d\n", pl->state);
#endif

  if (pl->win == 0) return;

  XDrawLine(pl->dpy, pl->win, gc,
	    pl->pointer_marks.x, pl->origin.y + pl->size.y - 1,
	    pl->pointer_marks.x, pl->origin.y + pl->size.y - 1 - x_pm_l);
  XDrawLine(pl->dpy, pl->win, gc,
	    pl->origin.x, pl->pointer_marks.y,
	    pl->origin.x + x_pm_l, pl->pointer_marks.y);

  XDrawLine(pl->dpy, pl->win, gc,
	    pl->pointer_marks.x - x_pm_l, pl->pointer_marks.y,
	    pl->pointer_marks.x + x_pm_l, pl->pointer_marks.y);
  XDrawLine(pl->dpy, pl->win, gc,
	    pl->pointer_marks.x, pl->pointer_marks.y - x_pm_l,
	    pl->pointer_marks.x, pl->pointer_marks.y + x_pm_l);

  switch (pl->state == SLAVE? pl->master_state : pl->state) {
  case ZOOM:
  case HZOOM:
  case VZOOM:
    XDrawRectangle(pl->dpy, pl->win, gc,
		   min(pl->dragstart.x, pl->dragend.x), 
		   min(pl->dragstart.y, pl->dragend.y),
		   abs(pl->dragend.x - pl->dragstart.x),
		   abs(pl->dragend.y - pl->dragstart.y));
    break;
  case HDRAG:
    XDrawLine(pl->dpy, pl->win, gc,
	      pl->dragstart.x, pl->dragstart.y,
	      pl->dragend.x, pl->dragstart.y);
    break;
  case VDRAG:
    XDrawLine(pl->dpy, pl->win, gc,
	      pl->dragstart.x, pl->dragstart.y,
	      pl->dragstart.x, pl->dragend.y);
    break;
  case DRAG:
    XDrawLine(pl->dpy, pl->win, gc,
	      pl->dragstart.x, pl->dragstart.y, pl->dragend.x, pl->dragend.y);
    break;

  default:
    /* we are called in motion notify */