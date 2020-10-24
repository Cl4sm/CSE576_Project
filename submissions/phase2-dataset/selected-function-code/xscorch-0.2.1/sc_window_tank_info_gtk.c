void sc_window_tank_info_gtk(sc_window_gtk *w, sc_player *p) {

   char buf[SC_GTK_STRING_BUFFER];
   GtkWidget *console;

   console = sc_window_active_console_new(w, 8, 4, 46, 7, CONSOLE_NORMAL);
   sc_console_set_foreground(SC_CONSOLE(console), &w->colormap->pcolors[p->index]);

   sc_console_write_line(SC_CONSOLE(console), 36, 6, "< Close >");
   sc_active_console_add_spot(SC_ACTIVE_CONSOLE(console), 36, 6, 9, 1, NULL);
   g_signal_connect_after(G_OBJECT(console), "key_press_spot",
                          (GCallback)_sc_window_tank_info_key_gtk, w);
   g_signal_connect_after(G_OBJECT(console), "select-spot",
                          (GCallback)_sc_window_tank_close_gtk, w);
   
   sbprintf(buf, sizeof(buf), "%2d: %s (%s)", 
            p->index + 1, p->name, sc_ai_name(p->aitype));
   sc_console_write_line(SC_CONSOLE(console), 0, 0, buf);
   /* We must scale player tank life to display in game life units. */
   sbprintf(buf, sizeof(buf), "    Life:     %-5d     Shield:  %-5d", 
            INT_ROUND_DIV(p->life, p->tank->hardness),
            p->shield == NULL ? 0 : p->shield->life);
   sc_console_write_line(SC_CONSOLE(console), 0, 1, buf);
   sbprintf(buf, sizeof(buf), "    Wins:     %-2d        Kills:   %-2d", 
            p->numwins, p->kills);
   sc_console_write_line(SC_CONSOLE(console), 0, 2, buf);
   sbprintf(buf, sizeof(buf), "    Suicides: %d", 
            p->suicides);
   sc_console_write_line(SC_CONSOLE(console), 0, 3, buf);
   sbprintf(buf, sizeof(buf), "    Money:    $%-8d %c$%d this round", 
            p->money, p->money > p->oldmoney ? '+' : '-', p->money - p->oldmoney);
   sc_console_write_line(SC_CONSOLE(console), 0, 4, buf);
   sbprintf(buf, sizeof(buf), "    Battery:  %-2d        Trigger: %-2d",
            sc_player_battery_count(w->c, p), sc_player_contact_trigger_count(w->c, p));
   sc_console_write_line(SC_CONSOLE(console), 0, 5, buf);
   sbprintf(buf, sizeof(buf), "Press [Enter] to close");
   sc_console_write_line(SC_CONSOLE(console), 0, 6, buf);

   sc_display_console_attach(SC_DISPLAY(w->screen), SC_CONSOLE(console));

   sc_game_pause(w->c, w->c->game);

   /* focus on this tank */
   gtk_window_set_default(GTK_WINDOW(w->app), GTK_WIDGET(console));
   gtk_widget_grab_focus(GTK_WIDGET(console));
   
}
