void sc_window_main_menu(sc_window *w_) {

   sc_window_gtk *w = (sc_window_gtk *)w_;
   static const char *mainmenu[] = {
      "XScorch Options:",
      "   Setup Players/Rounds",
      "   Setup Economics",
      "   Setup Physics",
      "   Setup Landscape",
      "   Setup Weapons",
      "   Setup Graphics",
      "   Setup Gameplay Options",
      "   Setup AI Controller",
      "   Setup Sound Support",
      "   Save Options",
      "   Begin Game",
      "   Exit XScorch",
      "You may use mouse or arrow",
      "keys to select menu options.",
      NULL
   };
   const char **current;
   GtkWidget *console;

   /* Detach any currently visible consoles */
   sc_display_console_detach_all(SC_DISPLAY(w->screen));

   /* Setup the background image */   
   sc_window_paint_blank(w_);
   sc_window_paint(w_, 0, 0, w->c->fieldwidth, w->c->fieldheight,
                   SC_REGENERATE_LAND | SC_REDRAW_LAND);
   sc_pixmap_copy_gtk(sc_display_get_buffer(SC_DISPLAY(w->screen)),
                      sc_display_get_gc(SC_DISPLAY(w->screen)),
                      w->logo, w->logo_m,
                      w->c->fieldwidth - sc_pixmap_width_gtk(w->logo),
                      w->c->fieldheight - sc_pixmap_height_gtk(w->logo));

   /* Put strings into menu; activate the relevant rows */
   console = sc_window_active_console_new(w, 4, 2, 30, 15, CONSOLE_NORMAL);
   for(current = mainmenu; *current != NULL; ++current) {
      sc_console_write_line(SC_CONSOLE(console), 0, current - mainmenu, *current);
      if(**current == ' ') {
         /* We assume this line is a menu entry, if indented */
         sc_active_console_add_row_spot(SC_ACTIVE_CONSOLE(console), current - mainmenu, NULL);
      }
   }

   /* Must be connected after default handler is run */
   g_signal_connect_after(G_OBJECT(console), "select_spot",
                          (GCallback)_sc_main_menu_select_gtk, w);
   g_object_set_data(G_OBJECT(console), "user_data", w);
   sc_display_console_attach(SC_DISPLAY(w->screen), SC_CONSOLE(console));

   /* Setup the general interface */
   gtk_window_set_default(GTK_WINDOW(w->app), GTK_WIDGET(console));
   gtk_widget_grab_focus(GTK_WIDGET(console));

}
