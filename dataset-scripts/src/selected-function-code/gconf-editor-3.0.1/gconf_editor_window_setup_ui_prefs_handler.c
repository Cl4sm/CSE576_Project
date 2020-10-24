static void
gconf_editor_window_setup_ui_prefs_handler (GConfEditorWindow *window)
{
	GConfClient *client;

	client = gconf_client_get_default ();

	gtk_ui_manager_set_add_tearoffs (
		window->ui_manager, gconf_client_get_bool (client, "/desktop/gnome/interface/menus_have_tearoff", 
							   NULL));

	window->tearoffs_notify_id = gconf_client_notify_add (
						client,
						"/desktop/gnome/interface/menus_have_tearoff",
						(GConfClientNotifyFunc) gconf_editor_window_have_tearoffs_notify,
						window, NULL, NULL);

	gconf_editor_window_set_have_icons (
		window, gconf_client_get_bool (client, "/desktop/gnome/interface/menus_have_icons", NULL));

	window->icons_notify_id = gconf_client_notify_add (
						client,
						"/desktop/gnome/interface/menus_have_icons",
						(GConfClientNotifyFunc) gconf_editor_window_have_icons_notify,
						window, NULL, NULL);

	g_object_unref (client);
}