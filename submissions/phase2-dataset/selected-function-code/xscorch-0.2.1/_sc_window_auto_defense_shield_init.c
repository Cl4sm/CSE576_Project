void _sc_window_auto_defense_shield_init(sc_auto_def_gtk *adg) {
/* _sc_window_auto_defense_shield_init
   Draw up the console pane for Shield Selection.
   There is allocation here so don't call it multiple times. */

   int count;
   char buf[SC_GTK_STRING_BUFFER];
   sc_window_gtk *w = adg->w;
   sc_accessory_config *ac = w->c->accessories;
   sc_accessory_info *info;

   /* Make a title on the main window. */
   sc_console_write_line(SC_CONSOLE(adg->console_main), 45, 3, "Shield Selection");
   sc_console_highlight_attach(SC_CONSOLE(adg->console_main), 
                               sc_console_get_color(SC_CONSOLE(adg->console_main), SC_CONSOLE_FORESTANDARD), 
                               NULL, TRUE, 45, 3, 16, 1);

   /* Create storage space for the working set and set up for the search. */
   info = sc_accessory_first(ac, SC_ACCESSORY_LIMIT_ALL);
   count = sc_accessory_count(ac, SC_ACCESSORY_LIMIT_ALL);
   adg->set_shield.items = (int *)malloc(count * sizeof(int));
   adg->set_shield.length = 0;
   /* Find the shields we have in inventory. */
   for(; count > 0; --count) {
      if(SC_ACCESSORY_IS_SHIELD(info) && info->inventories[adg->p->index] > 0) {
         /* We can allow them to activate this shield. */
         adg->set_shield.items[adg->set_shield.length++] = info->ident;
      }
      info = sc_accessory_next(ac, info, SC_ACCESSORY_LIMIT_ALL);
   }
   /* Shrink the memory to package just what we need. */
   adg->set_shield.items = (int *)realloc(adg->set_shield.items, adg->set_shield.length * sizeof(int));

   /* Create the shield selection window. */
   adg->console_shield = sc_window_active_console_new(w, 45, 9, 32, 3, CONSOLE_NORMAL);
   sc_console_buffer_size(SC_CONSOLE(adg->console_shield), 32, adg->set_shield.length + 1);

   /* Draw the ``nothing selected'' item and hook it up. */
   sbprintf(buf, sizeof(buf), "(none)");
   sc_console_write_line(SC_CONSOLE(adg->console_shield), 0, 0, buf);
   sc_active_console_add_row_spot(SC_ACTIVE_CONSOLE(adg->console_shield), 0, NULL);

   /* Draw the shield selections and hook them up. */
   for(count = 0; count < adg->set_shield.length; ++count) {
      /* Generate a name tag for the item in question. */
      info = sc_accessory_lookup(ac, adg->set_shield.items[count], SC_ACCESSORY_LIMIT_ALL);
      sbprintf(buf, sizeof(buf), "%s (%02i)", info->name, info->inventories[adg->p->index]);
      /* Display is offset +1 from index because of the ``(none)'' item. */
      sc_console_write_line(SC_CONSOLE(adg->console_shield), 0, count + 1, buf);
      sc_active_console_add_row_spot(SC_ACTIVE_CONSOLE(adg->console_shield), count + 1, NULL);
   }

   /*
    * Attach the shield selection signal handlers.  Right now there's
    * just enter_spot.  Can you think of others that would be useful?
    * I can't.  :)
    */
   g_signal_connect_after(G_OBJECT(adg->console_shield), "enter-spot",
                          (GCallback)_sc_autodef_shield_enter_gtk, adg);
   g_signal_connect_after(G_OBJECT(adg->console_shield), "key_press_spot",
                          (GCallback)_sc_window_auto_def_key_gtk, adg);

   /* Display the shield selection window. */
   sc_display_console_attach(SC_DISPLAY(w->screen), SC_CONSOLE(adg->console_shield));

}
