FillPixmapWithTile (Pixmap pixmap, Pixmap tile, int x, int y, int width, int height, int tile_x, int tile_y)
{
  if (tile != None && pixmap != None)
    {
      GC gc;
      XGCValues gcv;

      gcv.tile = tile;
      gcv.fill_style = FillTiled;
      gcv.ts_x_origin = -tile_x;
      gcv.ts_y_origin = -tile_y;
      gc =
	XCreateGC (Xdisplay, tile,
		   GCFillStyle | GCTile | GCTileStipXOrigin |
		   GCTileStipYOrigin, &gcv);
      XFillRectangle (Xdisplay, pixmap, gc, x, y, width, height);
      XFreeGC (Xdisplay, gc);
      return 1;
    }
  return 0;
}
