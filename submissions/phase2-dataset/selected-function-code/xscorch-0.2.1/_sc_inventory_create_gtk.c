/* sc_inventory_create_gtk
   Construct the inventory window for given player. */

   sc_inventory_gtk *inv;
   int windowheight;
   int i;

   /* Weapons */
   int wpcount;
   sc_weapon_config *wc = w->c->weapons;
   sc_weapon_info *wpinfo = sc_weapon_first(wc, SC_WEAPON_LIMIT_ALL);

   /* Accessories */
   int account;
   sc_accessory_config *ac = w->c->accessories;
   sc_accessory_info *acinfo = sc_accessory_first(ac, SC_ACCESSORY_LIMIT_ALL);

   /* Compute the window size */
   windowheight = w->c->fieldheight / sc_console_get_row_height(SC_CONSOLE(w->status)) - 16;
   if(windowheight < SC_MIN_WINDOW_SIZE) windowheight = SC_MIN_WINDOW_SIZE;

   /* How many weapons/accessories, so we can decide height of panes? */
   wpcount = sc_weapon_count(w->c->weapons, SC_WEAPON_LIMIT_ALL);
   account = sc_accessory_count(w->c->accessories, SC_ACCESSORY_LIMIT_ALL);
   inv = (sc_inventory_gtk *)malloc(sizeof(sc_inventory_gtk));
   inv->w = w;
   inv->p = p;

   /* Construct the info, weapons, and accessories panels. */
   inv->weaponpane      = sc_window_active_console_new(w, 4, 7, 40, windowheight, CONSOLE_NORMAL);
   inv->accessorypane   = sc_window_active_console_new(w, 50, 7, 40, windowheight, CONSOLE_NORMAL);
   inv->infopane        = sc_window_active_console_new(w, 4, 2, 86, 2, CONSOLE_NORMAL);

   /* Weapons and accessories maintain offscreen buffers for scrolling */
   sc_console_buffer_size(SC_CONSOLE(inv->weaponpane), 40, wpcount);
   sc_console_buffer_size(SC_CONSOLE(inv->accessorypane), 40, account);

   /* Construct the bottom informational console. */
   inv->invinfopane = sc_window_console_new(w, 4, windowheight + 10, 86, 4, CONSOLE_NORMAL);
   sc_console_highlight_attach(SC_CONSOLE(inv->invinfopane), &w->colormap->white, NULL, FALSE,
                               0, 0, sc_console_get_width(SC_CONSOLE(inv->invinfopane)), 2);
   sc_console_write_line(SC_CONSOLE(inv->invinfopane), 0, 2,
                         "Keyboard: [Right] buys a bundle, [Left] sells, and [Tab]/[Up]/[Down] navigate.");
   sc_console_write_line(SC_CONSOLE(inv->invinfopane), 0, 3,
                         "Mouse:  [Left click] buys, [Right click] sells.  Press [Enter] when you're done.");
   sc_display_console_attach(SC_DISPLAY(w->screen), SC_CONSOLE(inv->invinfopane));

   /* Hook up the various callbacks and add hotspots for
      each weapon which is in the panel. */
   g_signal_connect_after(G_OBJECT(inv->weaponpane), "key_press_spot",
                          (GCallback)_sc_inventory_weapon_key_gtk, inv);
   g_signal_connect_after(G_OBJECT(inv->weaponpane), "button_press_spot",
                          (GCallback)_sc_inventory_weapon_button_gtk, inv);
   g_signal_connect_after(G_OBJECT(inv->weaponpane), "enter_spot",
                          (GCallback)_sc_inventory_weapon_enter_gtk, inv);
   g_signal_connect_after(G_OBJECT(inv->weaponpane), "leave_spot",
                          (GCallback)_sc_inventory_info_leave_gtk, inv);
   sc_display_console_attach(SC_DISPLAY(w->screen), SC_CONSOLE(inv->weaponpane));
   for(i = 0; i < wpcount; ++i) {
      sc_active_console_add_row_spot(SC_ACTIVE_CONSOLE(inv->weaponpane), i, wpinfo);
      wpinfo = sc_weapon_next(wc, wpinfo, SC_WEAPON_LIMIT_ALL);
   }

   /* Hook up the various callbacks and add hotspots for
      each accessory which is in the panel. */
   g_signal_connect_after(G_OBJECT(inv->accessorypane), "key_press_spot",
                          (GCallback)_sc_inventory_accessory_key_gtk, inv);
   g_signal_connect_after(G_OBJECT(inv->accessorypane), "button_press_spot",
                          (GCallback)_sc_inventory_accessory_button_gtk, inv);
   g_signal_connect_after(G_OBJECT(inv->accessorypane), "enter_spot",
                          (GCallback)_sc_inventory_accessory_enter_gtk, inv);
   g_signal_connect_after(G_OBJECT(inv->accessorypane), "leave_spot",
                          (GCallback)_sc_inventory_info_leave_gtk, inv);
   sc_display_console_attach(SC_DISPLAY(w->screen), SC_CONSOLE(inv->accessorypane));
   for(i = 0; i < account; ++i) {
      sc_active_console_add_row_spot(SC_ACTIVE_CONSOLE(inv->accessorypane), i, acinfo);
      acinfo = sc_accessory_next(ac, acinfo, SC_ACCESSORY_LIMIT_ALL);
   }

   /* Display the panes, and hookup miscellaneous signals. */
   sc_display_console_attach(SC_DISPLAY(w->screen), SC_CONSOLE(inv->infopane));
   sc_console_set_foreground(SC_CONSOLE(inv->infopane), &w->colormap->pcolors[p->index]);
   sc_console_write_line(SC_CONSOLE(inv->infopane), 70, 1, "< Continue >");
   sc_active_console_add_spot(SC_ACTIVE_CONSOLE(inv->infopane), 70, 1, 12, 1, NULL);
   g_signal_connect_after(G_OBJECT(inv->infopane), "select_spot",
                          (GCallback)_sc_inventory_continue_gtk, inv);

   /* Paint every pane. */
   _sc_inventory_weapon_paint_gtk(inv);
   _sc_inventory_accessory_paint_gtk(inv);
   _sc_inventory_info_paint_gtk(inv);

   /* Focus on the weapon pane */
   gtk_window_set_default(GTK_WINDOW(w->app), GTK_WIDGET(inv->infopane));
   gtk_widget_grab_focus(GTK_WIDGET(inv->weaponpane));

}
