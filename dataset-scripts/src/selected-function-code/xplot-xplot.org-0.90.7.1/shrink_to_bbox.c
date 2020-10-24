
/*
 * compute the bounding box of the current view
 */
void shrink_to_bbox(struct plotter *pl, int x, int y)
{
  command *c;
  
  int nmapped = 0;
  int ndots = 0;
  int virgin = 1;
  
  coord saved_x_left = pl_x_left;
  coord saved_x_right = pl_x_right;
  coord saved_y_bottom = pl_y_bottom;
  coord saved_y_top = pl_y_top;

  /* get mapped indicator set correctly for what new window size
   * before we scale  the other axis */
  for (c = pl->commands; c != NULL; c = c->next)
    compute_window_coords(pl, c);

#ifdef LOTS_OF_DEBUGGING_PRINTS
  fprintf(stderr, "C_S_P: view %d OLD %s %s %s %s\n",
	  pl->viewno,
	  unparse_coord(pl->x_type, pl_x_left),
	  unparse_coord(pl->x_type, pl_x_right),
	  unparse_coord(pl->y_type, pl_y_bottom),
	  unparse_coord(pl->y_type, pl_y_top));
#endif

  for (c = pl->commands; c != NULL; c = c->next)
    if ( c->mapped && ! c->decoration)
      {
	nmapped++;

	switch(c->type) {
	case LINE:
	case DLINE:
	  if ( x )
	    {
	      if (virgin || ccmp(pl->x_type, c->xb, pl_x_left, <))
		pl_x_left = c->xb;
	      if (virgin || ccmp(pl->x_type, c->xb, pl_x_right, >))
		pl_x_right = c->xb;
	    }
	  if ( y )
	    {
	      if (virgin || ccmp(pl->y_type, c->yb, pl_y_bottom, <))
		pl_y_bottom = c->yb;
	      if (virgin || ccmp(pl->y_type, c->yb, pl_y_top, >))
		pl_y_top = c->yb;
	    }
	  virgin = 0;
	  
	case X:
	case DOT:
	case PLUS:
	case BOX:
	case DIAMOND:
	case UTICK:
	case DTICK:
	case LTICK:
	case RTICK:
	case HTICK:
	case VTICK:
	case UARROW:
	case DARROW:
	case LARROW:
	case RARROW:
	case INVISIBLE:
	case TEXT:
	  ndots++;
	  if ( x )
	    {
	      if (virgin || ccmp(pl->x_type, c->xa, pl_x_left, <))
		pl_x_left = c->xa;
	      if (virgin || ccmp(pl->x_type, c->xa, pl_x_right, >))
		pl_x_right = c->xa;
	    }
	  if ( y )
	    {
	      if (virgin || ccmp(pl->y_type, c->ya, pl_y_bottom, <))
		pl_y_bottom = c->ya;
	      if (virgin || ccmp(pl->y_type, c->ya, pl_y_top, >))
		pl_y_top = c->ya;
	    }
	  virgin = 0;
	case TITLE:
	case XLABEL:
	case YLABEL:
	  break;
	}
	
      }

  /* make sure top/bottom are not equal, somehow  */
  if ( ccmp(pl->x_type, pl_x_left, pl_x_right, ==) )
    {
      pl_x_left = saved_x_left;
      pl_x_right = saved_x_right;
    }
  if ( ccmp(pl->y_type, pl_y_bottom, pl_y_top, ==) )
    {
      pl_y_bottom = saved_y_bottom;
      pl_y_top = saved_y_top;
    }

#if 0  /* this code is broken, round_{down,up} take more than one argument */
  /* make points 5/95 % rather than extreme - settle for rounding */
  if ( y )
    {
      pl_y_bottom = (impls[(int)pl->y_type]->round_down)(pl_y_bottom);
      pl_y_top = (impls[(int)pl->y_type]->round_up)(pl_y_top);
    }
#endif


#ifdef LOTS_OF_DEBUGGING_PRINTS
  fprintf(stderr, "C_S_P: nmapped=%d ndots=%d NEW %s %s %s %s\n",
	  nmapped, ndots,
	  unparse_coord(pl->x_type, pl_x_left),
	  unparse_coord(pl->x_type, pl_x_right),