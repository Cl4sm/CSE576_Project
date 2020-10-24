/* sc_window_lottery_result
   Paste up a window describing the result of the lottery. */

   sc_window_gtk *w = (sc_window_gtk *)w_;
   char buf[SC_GTK_STRING_BUFFER];
   GtkWidget *console;

   /* Make sure there's something to display. */
   if(w->c->lottery == NULL || w->c->lottery->winner == NULL) {
      printf("Warning:  called sc_window_lottery_result with no winners!\n");
      sc_game_set_state_now(w->c, w->c->game, SC_STATE_TURN_BEGIN);
      return;
   }

   /* Create a new console window.           x   y   w   h */
   console = sc_window_active_console_new(w, 8,  4, 36,  7, CONSOLE_NORMAL);

   /* Display the title, highlighted. */
   sc_console_write_line(SC_CONSOLE(console), 10, 0, "Lottery Winner!");
   sc_console_highlight_attach(SC_CONSOLE(console), &w->colormap->white, NULL, TRUE, 10, 0, 15, 1);

   /* Show the winner's name. */
   sbprintf(buf, sizeof(buf), "%s won the lottery!", w->c->lottery->winner->name);
   sc_console_write_line(SC_CONSOLE(console), 0, 2, buf);

   /* If we've been instructed to, show the stake. */
   if(showstake) {
      sbprintf(buf, sizeof(buf), "You just received a bundle of:");
      sc_console_write_line(SC_CONSOLE(console), 0, 3, buf);
      sbprintf(buf, sizeof(buf), w->c->lottery->stake->name);
      sc_console_write_line(SC_CONSOLE(console), 0, 4, buf);
   }

   /* Tell them how to exit this display. */
   sc_console_write_line(SC_CONSOLE(console), 0, 6, "Press [Enter] to close");
   sc_display_console_attach(SC_DISPLAY(w->screen), SC_CONSOLE(console));

   /* Display the close button. */
   sc_console_write_line(SC_CONSOLE(console), 27, 6, "< Close >");
   sc_active_console_add_spot(SC_ACTIVE_CONSOLE(console), 27, 6, 9, 1, NULL);

   /* Connect the exit methods. */
   g_signal_connect_after(G_OBJECT(console), "key_press_spot", 
                          (GCallback)_sc_window_lottery_key_gtk, w);
   g_signal_connect_after(G_OBJECT(console), "select-spot", 
                          (GCallback)_sc_window_lottery_close_gtk, w);

   /* focus on this window */
   gtk_window_set_default(GTK_WINDOW(w->app), GTK_WIDGET(console));
   gtk_widget_grab_focus(GTK_WIDGET(console));

}
