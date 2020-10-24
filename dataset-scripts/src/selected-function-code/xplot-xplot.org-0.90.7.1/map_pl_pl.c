  return lXPoint_from_dXPoint(dr);
}  

lXPoint map_pl_pl(struct plotter *pl_from, struct plotter *pl_to, lXPoint xp)
{

  dXPoint dr;
  dXPoint dxp;
  coord x;
  coord y;

#define pl pl_from
  dxp = dXPoint_from_lXPoint(xp);

  dxp = tosub(pl, dxp);

  x = unmap_coord(pl->x_type, pl_x_left, pl_x_right, pl->size.x, dxp.x);
  y = unmap_coord(pl->y_type, pl_y_bottom, pl_y_top, pl->size.y, dxp.y);
    
#undef pl
#define pl pl_to

  dr.x = map_coord(pl->x_type, pl_x_left, pl_x_right, pl->size.x, x);

  dr.y = map_coord(pl->y_type, pl_y_bottom, pl_y_top, pl->size.y, y);

  dr = tomain(pl,dr);
#undef pl