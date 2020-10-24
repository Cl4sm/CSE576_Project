static void
gconf_editor_window_selection_changed (GtkTreeSelection *selection, GConfEditorWindow *window)
{
	GtkTreeIter iter;
	gchar *base_title;

	switch (window->type) {
	case GCONF_EDITOR_WINDOW_TYPE_DEFAULTS:
		base_title = _("Configuration Editor (Default settings)");
		break;
	case GCONF_EDITOR_WINDOW_TYPE_MANDATORY:
		base_title = _("Configuration Editor (Mandatory settings)");
		break;
	default:
		base_title = _("Configuration Editor");
	}
		
	if (selection == NULL)
		gtk_window_set_title (GTK_WINDOW (window), base_title);
	else {
		gchar *name, *title, *path;
		GtkTreeIter child_iter;

		if (!gtk_tree_selection_get_selected (selection, NULL, &iter)) {
			gtk_window_set_title (GTK_WINDOW (window), base_title);
			return;
		}

		gtk_tree_model_sort_convert_iter_to_child_iter (GTK_TREE_MODEL_SORT (window->sorted_tree_model), &child_iter, &iter);

		name = gconf_tree_model_get_gconf_name (GCONF_TREE_MODEL (window->tree_model), &child_iter);

		if (strlen (name) > 0) {
			title = g_strdup_printf ("%s - %s", base_title, name);
		}
		else {
			title = g_strdup (base_title);
		}
		
		gtk_window_set_title (GTK_WINDOW (window), title);
		g_free (title);
		g_free (name);

		path = gconf_tree_model_get_gconf_path (GCONF_TREE_MODEL (window->tree_model), &child_iter);
		gtk_statusbar_pop (GTK_STATUSBAR (window->statusbar), 0);
		gtk_statusbar_push (GTK_STATUSBAR (window->statusbar), 0, path);

		gconf_list_model_set_root_path (GCONF_LIST_MODEL (window->list_model), path);

		g_free (path);
	}
}