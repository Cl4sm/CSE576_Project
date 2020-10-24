static gboolean _sc_window_auto_def_key_gtk(__libj_unused GtkWidget *widget,
                                            __libj_unused ScActiveConsoleSpot *spot,
                                            GdkEventKey *key, sc_auto_def_gtk *adg) {
/* _sc_window_auto_def_key_gtk
   Enact the auto defense settings and restart the auto defense loop. */

   sc_window_gtk *w = adg->w;

   if(!adg->ready) return(FALSE);

   #if SC_GTK_DEBUG_GTK && __debugging_macros
      SC_DEBUG_ENTER("got autodef cancel (esc)%s", "");
   #endif /* debug */

   switch(key->keyval) {
      case GDK_Return:
      case GDK_KP_Enter:
         /* Push out the settings the user gave us. */
         _sc_window_auto_def_engage(adg);
      case GDK_Escape:
         /* Destroy the display and restart the auto defense player loop. */
         sc_display_console_detach_all(SC_DISPLAY(w->screen));
         sc_game_set_state_now(w->c, w->c->game, SC_STATE_AUTO_DEFENSE_LOOP);
         _sc_auto_def_gtk_free(&adg);
         return(TRUE);
   }
   return(FALSE);

}
