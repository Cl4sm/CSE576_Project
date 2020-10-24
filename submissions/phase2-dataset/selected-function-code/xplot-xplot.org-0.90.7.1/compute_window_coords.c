};

static void compute_window_coords(struct plotter *pl, command *com)
{
  int loc1;
  int loc2;

  if (com->type == TITLE
      || com->type == XLABEL
      || com->type == YLABEL
      ) {
    /* complete special case */
    com->mapped = TRUE;
    com->needs_redraw = TRUE;
    return;
  }

  if (com->type == INVISIBLE) {
    com->mapped = FALSE;
    com->needs_redraw = FALSE;
    return;
  }


#define ccmp(ctype, c1, c2, op) (cmp_coord(ctype, c1, c2) op 0)
#define xcmp(xa, xb, op) ccmp(pl->x_type, xa, xb, op)
#define ycmp(ya, yb, op) ccmp(pl->y_type, ya, yb, op)

  if (xcmp(com->xa, pl_x_left, <))
    if (ycmp(com->ya, pl_y_top, >)) loc1 = 0;
    else
      if (ycmp(com->ya, pl_y_bottom, <)) loc1 = 6;
      else loc1 = 3;
  else
    if (xcmp(com->xa, pl_x_right, >))
      if (ycmp(com->ya, pl_y_top, >)) loc1 = 2;
      else
	if (ycmp(com->ya, pl_y_bottom, <)) loc1 = 8;
	else loc1 = 5;
    else
      if (ycmp(com->ya, pl_y_top, >)) loc1 = 1;
      else
	if (ycmp(com->ya, pl_y_bottom, <)) loc1 = 7;
	else loc1 = 4;

  switch(com->type) {
  default:
    panic("compute_window_coords: unknown command type");
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
  case TEXT:
    loc2 = loc1;
    break;
  case DLINE:
  case LINE:
    if (xcmp(com->xb, pl_x_left, <))
      if (ycmp(com->yb, pl_y_top, >)) loc2 = 0;
      else
	if (ycmp(com->yb, pl_y_bottom, <)) loc2 = 6;
	else loc2 = 3;
    else
      if (xcmp(com->xb, pl_x_right, >))
	if (ycmp(com->yb, pl_y_top, >)) loc2 = 2;
	else
	  if (ycmp(com->yb, pl_y_bottom, <)) loc2 = 8;
	  else loc2 = 5;
      else
	if (ycmp(com->yb, pl_y_top, >)) loc2 = 1;
	else
	  if (ycmp(com->yb, pl_y_bottom, <)) loc2 = 7;
	  else loc2 = 4;
    break;
  }

  com->mapped = FALSE;
  switch(in_rect_table[loc1][loc2]) {
  case NO:
    return;
  case MAYBE:
  case YES:
    break;
  default:
    panic("compute_window_coords: unknown value from table");
  }
  
#ifdef WINDOW_COORDS_IN_COMMAND_STRUCT
  com->a.x = map_coord(pl->x_type, pl_x_left, pl_x_right, pl->size.x,
		       com->xa);
  com->a.y = (pl->size.y - 1) -
    map_coord(pl->y_type, pl_y_bottom, pl_y_top, pl->size.y,
	      com->ya);

  switch(com->type) {
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
  case RARROW:
  case UARROW:
  case DARROW:
  case LARROW:
  case TEXT:
    break;
  case DLINE:
  case LINE:
    com->b.x = map_coord(pl->x_type, pl_x_left, pl_x_right, pl->size.x,
			 com->xb);
    com->b.y = (pl->size.y - 1) -
      map_coord(pl->y_type, pl_y_bottom, pl_y_top, pl->size.y,
		com->yb);
    break;
  default:
    panic("compute_window_coords: unknown command type");
  }

  com->a = tomain(pl,com->a);
  com->b = tomain(pl,com->b);
#endif
  com->mapped = TRUE;
  com->needs_redraw = TRUE;