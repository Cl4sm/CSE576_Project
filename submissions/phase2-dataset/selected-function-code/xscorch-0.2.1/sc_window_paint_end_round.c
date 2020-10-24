void sc_window_paint_end_round(sc_window *w_) {

   GtkWidget *console;
   sc_window_gtk *w = (sc_window_gtk *)w_;
   char buf[SC_GTK_STRING_BUFFER];

   console = sc_window_active_console_new(w, 4, 2, 80, w->c->numplayers + 2, CONSOLE_NORMAL);

   sbprintf(buf, sizeof(buf), "End of round %d, press [Enter] to continue.", w->c->curround + 1);
   sc_console_write_line(SC_CONSOLE(console), 0, 0, buf);
   sc_console_write_line(SC_CONSOLE(console), 3, 1, "#  Type         Name              Wins Kill Suic Xby Money      Winnings");
   _sc_window_end_populate_gtk(w, console);
   sc_display_console_attach(SC_DISPLAY(w->screen), SC_CONSOLE(console));

   sc_console_write_line(SC_CONSOLE(console), 60, 0, "< Continue >");
   sc_active_console_add_spot(SC_ACTIVE_CONSOLE(console), 60, 0, 12, 1, NULL);
   g_signal_connect(G_OBJECT(console), "select_spot", 
                    (GCallback)_sc_window_end_select_gtk, w);
   
   /* focus on this window */
   gtk_window_set_default(GTK_WINDOW(w->app), GTK_WIDGET(console));
   gtk_widget_grab_focus(GTK_WIDGET(console));

}
