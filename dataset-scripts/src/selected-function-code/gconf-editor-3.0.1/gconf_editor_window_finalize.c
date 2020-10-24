static void
gconf_editor_window_finalize (GObject *object)
{
	GConfEditorWindow *window = GCONF_EDITOR_WINDOW(object);
	GConfClient       *client;

	client = gconf_client_get_default ();

	g_object_unref (window->ui_manager);

	if (window->tearoffs_notify_id)
		gconf_client_notify_remove (client, window->tearoffs_notify_id);

	if (window->icons_notify_id)
		gconf_client_notify_remove (client, window->icons_notify_id);

	g_object_unref (client);
	g_object_unref (window->client);

	G_OBJECT_CLASS (gconf_editor_window_parent_class)->finalize (object);
    
}