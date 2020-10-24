/* _sc_window_auto_defense_guidance_init
   Draw up the console pane for Guidance System selection.
   There is allocation here so don't call it multiple times. */

   int count;
   char buf[SC_GTK_STRING_BUFFER];
   sc_window_gtk *w = adg->w;
   sc_accessory_config *ac = w->c->accessories;
   sc_accessory_info *info;

   /* Make a title on the main window. */
   sc_console_write_line(SC_CONSOLE(adg->console_main), 9, 3, "Guidance System");
   sc_console_highlight_attach(SC_CONSOLE(adg->console_main), 
                               sc_console_get_color(SC_CONSOLE(adg->console_main), SC_CONSOLE_FORESTANDARD), 
                               NULL, TRUE, 9, 3, 15, 1);

   /* Create storage space for the working set and set up for the search. */
   info = sc_accessory_first(ac, SC_ACCESSORY_LIMIT_ALL);
   count = sc_accessory_count(ac, SC_ACCESSORY_LIMIT_ALL);
   adg->set_guidance.items = (int *)malloc(count * sizeof(int));
   adg->set_guidance.length = 0;
   /* Find the guidance systems we have in inventory. */
   for(; count > 0; --count) {
      if(SC_ACCESSORY_IS_GUIDANCE(info) && info->inventories[adg->p->index] > 0) {
         /* We can allow them to activate this guidance system. */
         adg->set_guidance.items[adg->set_guidance.length++] = info->ident;
      }
      info = sc_accessory_next(ac, info, SC_ACCESSORY_LIMIT_ALL);
   }
   /* Shrink the memory to package just what we need. */
   adg->set_guidance.items = (int *)realloc(adg->set_guidance.items, adg->set_guidance.length * sizeof(int));

   /* Create the guidance system window. */
   adg->console_guidance = sc_window_active_console_new(w, 9, 9, 32, 3, CONSOLE_NORMAL);
   sc_console_buffer_size(SC_CONSOLE(adg->console_guidance), 32, adg->set_guidance.length + 1);

   /* Draw the ``nothing selected'' item and hook it up. */
   sbprintf(buf, sizeof(buf), "(none)");
   sc_console_write_line(SC_CONSOLE(adg->console_guidance), 0, 0, buf);
   sc_active_console_add_row_spot(SC_ACTIVE_CONSOLE(adg->console_guidance), 0, NULL);

   /* Draw the guidance system selections and hook them up. */
   for(count = 0; count < adg->set_guidance.length; ++count) {
      /* Generate a name tag for the item in question. */
      info = sc_accessory_lookup(ac, adg->set_guidance.items[count], SC_ACCESSORY_LIMIT_ALL);
      sbprintf(buf, sizeof(buf), "%s (%02i)", info->name, info->inventories[adg->p->index]);
      /* Display is offset +1 from index because of the ``(none)'' item. */
      sc_console_write_line(SC_CONSOLE(adg->console_guidance), 0, count + 1, buf);
      sc_active_console_add_row_spot(SC_ACTIVE_CONSOLE(adg->console_guidance), count + 1, NULL);
   }

   /*
    * Attach the guidance system signal handlers.  Right now there's
    * just enter_spot.  Can you think of others that would be useful?
    * I can't.  :)
    */
   g_signal_connect_after(G_OBJECT(adg->console_guidance), "enter-spot",
                          (GCallback)_sc_autodef_guidance_enter_gtk, adg);
   g_signal_connect_after(G_OBJECT(adg->console_guidance), "key_press_spot",
                          (GCallback)_sc_window_auto_def_key_gtk, adg);

   /* Display the guidance system window. */
   sc_display_console_attach(SC_DISPLAY(w->screen), SC_CONSOLE(adg->console_guidance));

}
