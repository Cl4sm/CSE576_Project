void _sc_window_auto_defense_triggers_init(sc_auto_def_gtk *adg) {
/* _sc_window_auto_defense_triggers_init
   Draw up the console pane for Contact Trigger settings.
   There is allocation here so don't call it multiple times. */

   sc_window_gtk *w = adg->w;

   /* Create the contact triggers window. */
   adg->console_triggers = sc_window_active_console_new(w, 17, 16, 16, 5, CONSOLE_BORDERLESS);

   sc_console_write_line(SC_CONSOLE(adg->console_triggers),
                         0, 0, "Contact Triggers");
   sc_console_highlight_attach(SC_CONSOLE(adg->console_triggers),
                               NULL, NULL, TRUE,
                               0, 0, 16, 1);
   adg->trigger_toggle = SC_AC_TOGGLE(sc_ac_toggle_new(6, 1, 4, 2));
   sc_ac_toggle_set(adg->trigger_toggle, adg->triggers ? TRUE : FALSE);
   sc_active_console_add_gadget_spot(SC_ACTIVE_CONSOLE(adg->console_triggers),
                                     SC_GADGET(adg->trigger_toggle), NULL);

   /* Desensitize the toggle if the player can't use triggers. */
   if(!(adg->p->ac_state & SC_ACCESSORY_STATE_TRIGGER))
      gtk_widget_set_sensitive(GTK_WIDGET(adg->console_triggers), FALSE);

   /* Handle signals; most are handled natively by the toggle... */
   g_signal_connect_after(G_OBJECT(adg->console_triggers), "key_press_spot",
                          (GCallback)_sc_window_auto_def_key_gtk, adg);

   /* Display the contact triggers window. */
   sc_display_console_attach(SC_DISPLAY(w->screen), SC_CONSOLE(adg->console_triggers));

}
