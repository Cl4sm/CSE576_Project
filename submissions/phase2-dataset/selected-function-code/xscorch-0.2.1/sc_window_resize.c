void sc_window_resize(sc_window *w_) {

   sc_window_gtk *w = (sc_window_gtk *)w_;
   sc_config *c = w->c;

   /* There went the landbuffer... */
   _sc_window_unref_land_buffer(w);
   w->landbuffer = gdk_pixmap_new(w->app->window, c->fieldwidth, c->fieldheight, -1);

   /*  We just seriously fsck'd things up  */
   gtk_widget_set_size_request(sc_display_get_drawbuf(SC_DISPLAY(w->screen)),
                               c->fieldwidth, c->fieldheight);
   sc_status_setup(c->window);
   sc_window_idle(c->window);

   /* Redraw everything */
   sc_land_generate(c, c->land);
   sc_window_paint(c->window, 0, 0,
                   c->land->width, c->land->height,
                   SC_REGENERATE_LAND | SC_REDRAW_LAND);
   sc_pixmap_copy_gtk(sc_display_get_buffer(SC_DISPLAY(w->screen)),
                      sc_display_get_gc(SC_DISPLAY(w->screen)),
                      w->logo, w->logo_m,
                      c->land->width - sc_pixmap_width_gtk(w->logo),
                      c->land->height - sc_pixmap_height_gtk(w->logo));

}
