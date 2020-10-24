static gboolean _sc_window_tank_move_key_gtk(GtkWidget *widget, 
                                             __libj_unused ScActiveConsoleSpot *spot,
                                             GdkEventKey *key, sc_window_gtk *w) {

   char buf[SC_GTK_STRING_BUFFER];
   int fuel;

   switch(key->keyval) {
      case GDK_Left:
      case GDK_KP_Left:
         if(sc_player_move(w->c, w->c->plorder[w->c->game->curplayer], -1)) {
            fuel = sc_player_total_fuel(w->c->accessories, w->c->plorder[w->c->game->curplayer]);
            sbprintf(buf, sizeof(buf), "%4d", fuel);
            sc_console_write_line(SC_CONSOLE(widget), 22, 0, buf);
         }
         return(TRUE);
      case GDK_Right:
      case GDK_KP_Right:
         if(sc_player_move(w->c, w->c->plorder[w->c->game->curplayer], +1)) {
            fuel = sc_player_total_fuel(w->c->accessories, w->c->plorder[w->c->game->curplayer]);
            sbprintf(buf, sizeof(buf), "%4d", fuel);
            sc_console_write_line(SC_CONSOLE(widget), 22, 0, buf);
         }
         return(TRUE);
      case GDK_Return:
      case GDK_KP_Enter:
      case GDK_Escape:
         sc_display_console_detach_all(SC_DISPLAY(w->screen));
         sc_game_unpause(w->c, w->c->game);
         return(TRUE);
   }
   return(FALSE);

}
