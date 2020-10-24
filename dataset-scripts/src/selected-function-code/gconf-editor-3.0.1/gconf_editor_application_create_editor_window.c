GtkWidget *
gconf_editor_application_create_editor_window (int type)
{
	GtkWidget *window;
	GConfEditorWindow *gconfwindow;

	window = g_object_new (GCONF_TYPE_EDITOR_WINDOW, "editor-type", type, NULL);

	gconfwindow = GCONF_EDITOR_WINDOW (window);

	if (gconfwindow->client == NULL) {
		gconf_editor_application_window_destroyed (window);
		return NULL;
	}

	gconf_tree_model_set_client (GCONF_TREE_MODEL (gconfwindow->tree_model), gconfwindow->client);
	gconf_list_model_set_client (GCONF_LIST_MODEL (gconfwindow->list_model), gconfwindow->client);

	if (!gconf_util_can_edit_defaults ())
		gtk_action_set_sensitive (gtk_ui_manager_get_action (gconfwindow->ui_manager, "/GConfEditorMenu/FileMenu/NewDefaultsWindow"),
					  FALSE);

	if (!gconf_util_can_edit_mandatory ())
		gtk_action_set_sensitive (gtk_ui_manager_get_action (gconfwindow->ui_manager, "/GConfEditorMenu/FileMenu/NewMandatoryWindow"),
					  FALSE);
	
	g_signal_connect (window, "destroy",
			  G_CALLBACK (gconf_editor_application_window_destroyed), NULL);
	
	editor_windows = g_slist_prepend (editor_windows, window);

	gconf_editor_window_expand_first (gconfwindow);

	return window;
}