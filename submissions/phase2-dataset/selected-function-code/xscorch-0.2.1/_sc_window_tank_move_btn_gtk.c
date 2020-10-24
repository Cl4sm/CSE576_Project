
   char buf[SC_GTK_STRING_BUFFER];
   int fuel;

   switch((long)spot->data) {
      case 1:
         if(sc_player_move(w->c, w->c->plorder[w->c->game->curplayer], -1)) {
            fuel = sc_player_total_fuel(w->c->accessories, w->c->plorder[w->c->game->curplayer]);
            sbprintf(buf, sizeof(buf), "%4d", fuel);
            sc_console_write_line(SC_CONSOLE(widget), 22, 0, buf);
         }
         return(TRUE);
      case 2:
         if(sc_player_move(w->c, w->c->plorder[w->c->game->curplayer], +1)) {
            fuel = sc_player_total_fuel(w->c->accessories, w->c->plorder[w->c->game->curplayer]);
            sbprintf(buf, sizeof(buf), "%4d", fuel);
            sc_console_write_line(SC_CONSOLE(widget), 22, 0, buf);
         }
         return(TRUE);
      case 3:
         sc_display_console_detach_all(SC_DISPLAY(w->screen));
         sc_game_unpause(w->c, w->c->game);
         return(TRUE);
   }
   return(FALSE);

}
