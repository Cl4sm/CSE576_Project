
   char buf[SC_GTK_STRING_BUFFER];
   GtkWidget *console;
   int x;

   if(p->x < w->c->fieldwidth / 2) {
      x = w->c->fieldwidth / (2 * sc_console_get_col_width(SC_CONSOLE(w->status))) + 1;
   } else {
      x = 2;
   }
   console = sc_window_active_console_new(w, x, 2, 26, 8, CONSOLE_NORMAL);
   sc_console_set_foreground(SC_CONSOLE(console), &w->colormap->pcolors[p->index]);

   sc_console_write_line(SC_CONSOLE(console), 0, 0, p->name);
   
   sbprintf(buf, sizeof(buf), "%4d", sc_player_total_fuel(w->c->accessories, p));
   sc_console_write_line(SC_CONSOLE(console), 22, 0, buf);
   
   sc_console_write_line(SC_CONSOLE(console), 0, 1, "Right means left and left");
   sc_console_write_line(SC_CONSOLE(console), 0, 2, "means right. Go which way?");
   sc_console_write_line(SC_CONSOLE(console), 0, 3, "(arrows/buttons move tank,");
   sc_console_write_line(SC_CONSOLE(console), 0, 4, "  [Enter] closes window)");

   g_signal_connect_after(G_OBJECT(console), "key_press_spot",
                          (GCallback)_sc_window_tank_move_key_gtk, w);
   g_signal_connect_after(G_OBJECT(console), "select-spot",
                          (GCallback)_sc_window_tank_move_btn_gtk, w);

   sc_console_write_line(SC_CONSOLE(console), 2, 6, "< Left >");
   sc_active_console_add_spot(SC_ACTIVE_CONSOLE(console), 2, 6, 8, 1, (void *)1);
   
   sc_console_write_line(SC_CONSOLE(console), 15,6, "< Right >");
   sc_active_console_add_spot(SC_ACTIVE_CONSOLE(console), 15,6, 9, 1, (void *)2);
   
   sc_console_write_line(SC_CONSOLE(console), 15,7, "< Close >");
   sc_active_console_add_spot(SC_ACTIVE_CONSOLE(console), 15,7, 9, 1, (void *)3);
   
   sc_display_console_attach(SC_DISPLAY(w->screen), SC_CONSOLE(console));
   
   sc_game_pause(w->c, w->c->game);

   /* focus on this tank */
   gtk_window_set_default(GTK_WINDOW(w->app), GTK_WIDGET(console));
   gtk_widget_grab_focus(GTK_WIDGET(console));
   
}
