void sc_window_create_menus_gtk(sc_window_gtk *w) {

   GError *gerr = NULL;
   GtkAction *action = NULL;
   GtkActionGroup *menu_action_group = gtk_action_group_new("menu");
   GtkUIManager *menu_ui_manager = gtk_ui_manager_new();

   /* OMG I HATE XML */

   char *main_menu_xml = "<ui> \
   <menubar name=\"MenuBar\"> \
      <menu action=\"MenuGame\"> \
         <menuitem action=\"Pause\" /> \
         <menuitem action=\"RestartGame\" /> \
         <separator /> \
         <menuitem action=\"SystemMenu\" /> \
         <menuitem action=\"FontSetup\" /> \
         <menuitem action=\"SoundSetup\" /> \
         <menuitem action=\"SaveConfig\" /> \
         <separator /> \
         <menuitem action=\"Exit\" /> \
      </menu> \
      <placeholder name=\"PlaceholderNetwork\" /> \
      <menu action=\"MenuHelp\"> \
         <menuitem action=\"Manual\" /> \
         <menuitem action=\"License\" /> \
         <menuitem action=\"About\" /> \
      </menu> \
   </menubar> \
</ui>";

   char *network_menu_xml = "<ui> \
   <menubar name=\"MenuBar\"> \
      <placeholder name=\"PlaceholderNetwork\" > \
         <menu action=\"MenuNetwork\"> \
            <menuitem action=\"CreateServer\" /> \
            <menuitem action=\"ConnectClient\" /> \
            <menuitem action=\"DisconnectClient\" /> \
            <separator /> \
            <menuitem action=\"Chat\" /> \
            <menuitem action=\"Status\" /> \
         </menu> \
      </placeholder> \
   </menubar> \
</ui>";

   /* OMG what was so simple in GTK 1.2 is this complex in GTK 2.12.  x_x  */
   gtk_action_group_add_action(menu_action_group, gtk_action_new("MenuGame", "_Game", NULL, NULL));

   action = gtk_action_new("Pause", "_Pause", NULL, NULL);
   gtk_accel_map_add_entry("<Xscorch>/Game/Pause", 'z', GDK_CONTROL_MASK);
   gtk_action_set_accel_path(action, "<Xscorch>/Game/Pause");
   g_signal_connect_swapped(action, "activate", G_CALLBACK(_sc_action_game_pause_gtk), w);
   gtk_action_group_add_action(menu_action_group, action);
   g_object_unref(action);

   action = gtk_action_new("RestartGame", "_Restart Game", NULL, NULL);
   gtk_accel_map_add_entry("<Xscorch>/Game/Restart Game", 'r', GDK_CONTROL_MASK);
   gtk_action_set_accel_path(action, "<Xscorch>/Game/Restart Game");
   g_signal_connect_swapped(action, "activate", G_CALLBACK(_sc_action_game_restart_gtk), w);
   gtk_action_group_add_action(menu_action_group, action);
   g_object_unref(action);

   action = gtk_action_new("SystemMenu", "S_ystem Menu", NULL, NULL);
   gtk_accel_map_add_entry("<Xscorch>/Game/System Menu", 'y', GDK_CONTROL_MASK);
   gtk_action_set_accel_path(action, "<Xscorch>/Game/System Menu");
   g_signal_connect_swapped(action, "activate", G_CALLBACK(sc_system_menu_gtk), w);
   gtk_action_group_add_action(menu_action_group, action);
   g_object_unref(action);

   action = gtk_action_new("FontSetup", "_Font Setup ...", NULL, NULL);
   gtk_accel_map_add_entry("<Xscorch>/Game/Font Setup ...", 'f', GDK_CONTROL_MASK);
   gtk_action_set_accel_path(action, "<Xscorch>/Game/Font Setup ...");
   g_signal_connect_swapped(action, "activate", G_CALLBACK(sc_font_gtk), w);
   gtk_action_group_add_action(menu_action_group, action);
   g_object_unref(action);

   action = gtk_action_new("SoundSetup", "S_ound Setup ...", NULL, NULL);
   gtk_accel_map_add_entry("<Xscorch>/Game/Sound Setup ...", 'o', GDK_CONTROL_MASK);
   gtk_action_set_accel_path(action, "<Xscorch>/Game/Sound Setup ...");
   g_signal_connect_swapped(action, "activate", G_CALLBACK(sc_sound_setup_gtk), w);
   gtk_action_group_add_action(menu_action_group, action);
   g_object_unref(action);

   action = gtk_action_new("SaveConfig", "Sa_ve Configuration", NULL, NULL);
   gtk_accel_map_add_entry("<Xscorch>/Game/Save Configuration", 's', GDK_CONTROL_MASK);
   gtk_action_set_accel_path(action, "<Xscorch>/Game/Save Configuration");
   g_signal_connect_swapped(action, "activate", G_CALLBACK(sc_config_file_save_gtk), w);
   gtk_action_group_add_action(menu_action_group, action);
   g_object_unref(action);

   action = gtk_action_new("Exit", "E_xit", NULL, NULL);
   gtk_accel_map_add_entry("<Xscorch>/Game/Exit", 'x', GDK_CONTROL_MASK);
   gtk_action_set_accel_path(action, "<Xscorch>/Game/Exit");
   g_signal_connect_swapped(action, "activate", G_CALLBACK(_sc_action_game_exit_gtk), w);
   gtk_action_group_add_action(menu_action_group, action);
   g_object_unref(action);

   gtk_action_group_add_action(menu_action_group, gtk_action_new("MenuHelp", "_Help", NULL, NULL));

   action = gtk_action_new("Manual", "_Manual page ...", NULL, NULL);
   gtk_accel_map_add_entry("<Xscorch>/Help/Manual page ...", gdk_keyval_from_name("F1"), 0);
   gtk_action_set_accel_path(action, "<Xscorch>/Help/Manual page ...");
   g_signal_connect_swapped(action, "activate", G_CALLBACK(_sc_action_help_manual_gtk), w);
   gtk_action_group_add_action(menu_action_group, action);
   g_object_unref(action);

   action = gtk_action_new("License", "_License ...", NULL, NULL);
   g_signal_connect_swapped(action, "activate", G_CALLBACK(_sc_action_help_license_gtk), w);
   gtk_action_group_add_action(menu_action_group, action);
   g_object_unref(action);

   action = gtk_action_new("About", "_About ...", NULL, NULL);
   g_signal_connect_swapped(action, "activate", G_CALLBACK(_sc_action_help_about_gtk), w);
   gtk_action_group_add_action(menu_action_group, action);
   g_object_unref(action);

   #if USE_NETWORK

      gtk_action_group_add_action(menu_action_group, gtk_action_new("MenuNetwork", "_Network", NULL, NULL));

      action = gtk_action_new("CreateServer", "Create _Server ...", NULL, NULL);
      g_signal_connect_swapped(action, "activate", G_CALLBACK(_sc_action_network_server_gtk), w);
      gtk_action_group_add_action(menu_action_group, action);
      g_object_unref(action);

      action = gtk_action_new("ConnectClient", "Connect as _Client ...", NULL, NULL);
      g_signal_connect_swapped(action, "activate", G_CALLBACK(_sc_action_network_client_gtk), w);
      gtk_action_group_add_action(menu_action_group, action);
      g_object_unref(action);

      action = gtk_action_new("DisconnectClient", "_Disconnect", NULL, NULL);
      g_signal_connect_swapped(action, "activate", G_CALLBACK(_sc_action_network_disconnect_gtk), w);
      gtk_action_group_add_action(menu_action_group, action);
      g_object_unref(action);

      action = gtk_action_new("Chat", "C_hat ...", NULL, NULL);
      g_signal_connect_swapped(action, "activate", G_CALLBACK(sc_chat_window_gtk), w);
      gtk_action_group_add_action(menu_action_group, action);
      g_object_unref(action);

      action = gtk_action_new("Status", "S_tatus ...", NULL, NULL);
      g_signal_connect_swapped(action, "activate", G_CALLBACK(_sc_action_network_status_gtk), w);
      gtk_action_group_add_action(menu_action_group, action);
      g_object_unref(action);

   #endif /* Network? */

   /* Make the new actions available to the menu items. */
   gtk_ui_manager_insert_action_group(menu_ui_manager, menu_action_group, 0);

   /* Turn XML crap into menu structures. */
   gtk_ui_manager_add_ui_from_string(menu_ui_manager, main_menu_xml, -1, &gerr);
   if (gerr != NULL) {
      fprintf(stderr, "Failed parsing main menu structure: \n%s\n", gerr->message);
      g_error_free(gerr);
   }

   #if USE_NETWORK
      gtk_ui_manager_add_ui_from_string(menu_ui_manager, network_menu_xml, -1, &gerr);
      if (gerr != NULL) {
         fprintf(stderr, "Failed parsing main menu structure: \n%s\n", gerr->message);
         g_error_free(gerr);
      }
      w->net_server     = gtk_ui_manager_get_widget(menu_ui_manager, "/ui/NetworkMenu/Create Server ...");
      w->net_client     = gtk_ui_manager_get_widget(menu_ui_manager, "/ui/NetworkMenu/Connect as Client ...");
      w->net_disconnect = gtk_ui_manager_get_widget(menu_ui_manager, "/ui/NetworkMenu/Disconnect");
      w->net_chat       = gtk_ui_manager_get_widget(menu_ui_manager, "/ui/NetworkMenu/Chat ...");
      w->net_status     = gtk_ui_manager_get_widget(menu_ui_manager, "/ui/NetworkMenu/Status ...");
   #endif /* Network? */

   /* Install the new menus. */
   gtk_window_add_accel_group(GTK_WINDOW(w->app), gtk_ui_manager_get_accel_group(menu_ui_manager));

   /* Finally we have something we can call a menu. */
   w->mainmenu = gtk_ui_manager_get_widget(menu_ui_manager, "/MenuBar");

   /* Do it now.  Yes really. */
   gtk_ui_manager_ensure_update(menu_ui_manager);

   /* I don't see a way to free the UI Manager, oh well... */

}
