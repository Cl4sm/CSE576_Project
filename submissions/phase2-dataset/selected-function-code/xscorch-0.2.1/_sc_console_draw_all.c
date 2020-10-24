/* sc_console_draw_all
   Redraws the entire console, including all displayable text, all borders,
   and the scrollbars if they are applicable.  */

   GtkWidget *widget = (GtkWidget *)cons;

   assert(IS_SC_CONSOLE(cons));

   /* Can we even draw yet? */
   if(sc_drawbuf_get_buffer(SC_DRAWBUF(cons)) == NULL) return;

   /* Setup default foreground, background colors */
   /* (make sure to allocate them if not done already) */
   if(!cons->colors.colors_alloc) {
      cons->colors.colors_alloc = TRUE;
      gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &cons->colors.foreground, FALSE, TRUE);
      gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &cons->colors.background, FALSE, TRUE);
      gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &cons->colors.forecursor, FALSE, TRUE);
      gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &cons->colors.backcursor, FALSE, TRUE);
      gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &cons->colors.foreshadow, FALSE, TRUE);
      gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &cons->colors.backshadow, FALSE, TRUE);
      gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &cons->colors.forescroll, FALSE, TRUE);
      gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &cons->colors.backscroll, FALSE, TRUE);
      gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &cons->colors.forelight, FALSE, TRUE);
      gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &cons->colors.backlight, FALSE, TRUE);
      gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &cons->colors.foredisabled, FALSE, TRUE);
      gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &cons->colors.backdisabled, FALSE, TRUE);
      gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &cons->colors.forestandard, FALSE, TRUE);
      gdk_colormap_alloc_color(gtk_widget_get_colormap(widget), &cons->colors.backstandard, FALSE, TRUE);
   }

   /* Redraw the window frame */
   _sc_console_draw_frame(cons);

   /* Redraw each line of text */
   _sc_console_draw_region(cons, 0, 0, sc_console_get_width(cons), sc_console_get_height(cons));

   /* Make sure everything is queued for draw */
   sc_drawbuf_queue_draw(SC_DRAWBUF(widget), 0, 0, widget->allocation.width, widget->allocation.height);

}
