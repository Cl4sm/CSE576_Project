static gboolean _sc_window_auto_def_close_gtk(__libj_unused GtkWidget *widget,
                                              __libj_unused ScActiveConsoleSpot *spot,
                                              sc_auto_def_gtk *adg) {
/* _sc_window_auto_def_close_gtk
   Enact the auto defense settings and restart the auto defense loop. */

   sc_window_gtk *w = adg->w;

   if(!adg->ready) return(FALSE);

   #if SC_GTK_DEBUG_GTK && __debugging_macros
      SC_DEBUG_ENTER("accepting autodef close%s", "");
   #endif /* debug */

   /* Enact the user requests. */
   _sc_window_auto_def_engage(adg);

   /* Destroy the display and restart the auto defense player loop. */
   sc_display_console_detach_all(SC_DISPLAY(w->screen));
   sc_game_set_state_now(w->c, w->c->game, SC_STATE_AUTO_DEFENSE_LOOP);
   sc_status_message((sc_window *)w, "");
   _sc_auto_def_gtk_free(&adg);
   return(TRUE);

}
