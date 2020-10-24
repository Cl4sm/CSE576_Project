static gboolean
window_expose_event (GtkWidget *widget, GdkEventExpose *event)
{
  GdkRegion *region;
  GtkWidget *child;
  cairo_t *cr;

  /* get our child (in this case, the event box) */ 
  child = gtk_bin_get_child (GTK_BIN (widget));

  /* create a cairo context to draw to the window */
  cr = gdk_cairo_create (widget->window);

  /* the source data is the (composited) event box */
  gdk_cairo_set_source_pixmap (cr, child->window,
			       child->allocation.x,
			       child->allocation.y);

  /* draw no more than our expose event intersects our child */
  region = gdk_region_rectangle (&child->allocation);
  gdk_region_intersect (region, event->region);
  gdk_cairo_region (cr, region);
  cairo_clip (cr);

  /* composite, with a 50% opacity */
  cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
  cairo_paint_with_alpha (cr, 1.0);

  /* we're done */
  cairo_destroy (cr);

  return FALSE;
}