/* sc_window_auto_defense
   Display the auto defense window. */

   sc_auto_def_gtk *adg;
   sc_window_gtk *w = (sc_window_gtk *)w_;
   char buf[SC_GTK_STRING_BUFFER];

   /* Put together the persistant information. */
   adg = _sc_auto_def_gtk_new();
   if(adg == NULL || w == NULL || p == NULL) {
      /* Terminating conditions! */
      free(adg);
      sc_game_set_state_now(w->c, w->c->game, SC_STATE_AUTO_DEFENSE_LOOP);
      return;
   }
   adg->w = w;
   adg->p = p;


   /*   H E A D E R   */

   /* Make a new window.                               x   y   w   h */
   adg->console_main = sc_window_active_console_new(w, 8,  4, 70, 20, CONSOLE_NORMAL);
   sc_display_console_attach(SC_DISPLAY(w->screen), SC_CONSOLE(adg->console_main));
   sc_console_set_foreground(SC_CONSOLE(adg->console_main), &w->colormap->pcolors[p->index]);
   gtk_widget_set_sensitive(GTK_WIDGET(adg->console_main), FALSE);

   /* Put a highlighted title on the new window. */
   sbprintf(buf, sizeof(buf), "Tank Defense Controller for %s", p->name);
   sc_console_write_line(SC_CONSOLE(adg->console_main),
                         70 / 2 - 14 - strlenn(p->name) / 2, 0, buf);
   sc_console_highlight_attach(SC_CONSOLE(adg->console_main),
                               NULL, NULL, TRUE,
                               70 / 2 - 14 - strlenn(p->name) / 2, 0,
                               28 + strlenn(p->name), 1);


   /*   S E L E C T I O N S   */

   /* Display the guidance device selection. */
   adg->auto_guidance = NULL;
   _sc_window_auto_defense_guidance_init(adg);

   /* Display the shield selection. */
   adg->auto_shield = NULL;
   _sc_window_auto_defense_shield_init(adg);

   /* Display the parachute selection. */
   adg->chute_height = 8;
   _sc_window_auto_defense_parachute_init(adg);

   /* Display the contact trigger selection. */
   adg->triggers = p->contacttriggers;
   _sc_window_auto_defense_triggers_init(adg);


   /*   F O O T E R   */

   /* Float the exit instructions on their own console to fix null-focus crap. */
   adg->console_activate = sc_window_active_console_new(w, 8, 23, 70, 1, CONSOLE_BORDERLESS);

   /* Display the EXIT instructions. */
   sc_console_write_line(SC_CONSOLE(adg->console_activate), 0, 0,
                         "Press [Enter] to activate, or [Escape] to cancel");

   /* Set up the EXIT button. */
   sc_console_write_line(SC_CONSOLE(adg->console_activate), 58, 0, "< Activate >");
   sc_active_console_add_spot(SC_ACTIVE_CONSOLE(adg->console_activate), 58, 0, 12, 1, NULL);

   /* Connect the signal handlers for EXIT conditions. */
   g_signal_connect_after(G_OBJECT(adg->console_activate), "key_press_spot",
                          (GCallback)_sc_window_auto_def_key_gtk, adg);
   g_signal_connect_after(G_OBJECT(adg->console_activate), "select-spot",
                          (GCallback)_sc_window_auto_def_close_gtk, adg);

   sc_display_console_attach(SC_DISPLAY(w->screen), SC_CONSOLE(adg->console_activate));

   /* Focus on this window. */
   gtk_window_set_default(GTK_WINDOW(w->app), GTK_WIDGET(adg->console_activate));
   /* I'd do console_main here but it gives focus to some null handler. */
   gtk_widget_grab_focus(GTK_WIDGET(adg->console_guidance));

   /* Everything is ready and running. */
   adg->ready = true;

}
