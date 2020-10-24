static gint _sc_drawbuf_configure(GtkWidget *widget, GdkEventConfigure *event) {

   ScDrawbuf *draw = SC_DRAWBUF(widget);
   gint width = widget->allocation.width;
   gint height= widget->allocation.height;
   gint pixmap_width = -1;
   gint pixmap_height= -1;
   GdkColor black;

   /* Call parent class handler? */
   if(GTK_WIDGET_CLASS(parent_class)->configure_event != NULL) {
      if(GTK_WIDGET_CLASS(parent_class)->configure_event(widget, event)) {
         /* We must halt. */
         return(TRUE);
      } /* Can we continue? */
   } /* Call parent handler? */

   /* Get current pixmap dimensions */
   if(sc_drawbuf_get_buffer(draw) != NULL) {
      gdk_drawable_get_size(sc_drawbuf_get_buffer(draw), &pixmap_width, &pixmap_height);
   }

   /* Make sure the size actually changed ... */
   if(width != pixmap_width || height != pixmap_height) {
      /* Has the style been configured? */
      if(!draw->style_configured) {
         gdk_color_black(gtk_widget_get_colormap(widget), &black);
         gdk_window_set_background(widget->window, &black);
      }

      /* Release any existing drawing buffer */
      _sc_drawbuf_unref_pixmap_and_gc(draw);

      /* Construct new offscreen-drawable, and clear it */
      draw->screen_buffer = gdk_pixmap_new(widget->window, width, height, -1);
      gdk_draw_rectangle(draw->screen_buffer,
                         widget->style->black_gc,
                         TRUE,
                         0, 0,
                         width, height);

      /* Construct a new offscreen-GC to use */
      draw->screen_gc = gdk_gc_new(widget->window);
   }

   /* Allow other events */
   return(FALSE);

}
