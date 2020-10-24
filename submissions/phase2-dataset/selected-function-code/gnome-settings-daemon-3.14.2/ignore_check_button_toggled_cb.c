static void
ignore_check_button_toggled_cb (GtkToggleButton *button,
                                gpointer user_data)
{
        GsdLdsmDialog *dialog = (GsdLdsmDialog *)user_data;
        GSettings *settings;
        gchar **settings_list;
        gboolean ignore, updated;
        gint i;
        GSList *ignore_paths = NULL;

        settings = g_settings_new (SETTINGS_HOUSEKEEPING_DIR);

        settings_list = g_settings_get_strv (settings, "ignore-paths");

        for (i = 0; i < G_N_ELEMENTS (settings_list); i++) {
                if (settings_list[i] != NULL)
                        ignore_paths = g_slist_append (ignore_paths, g_strdup (settings_list[i]));
        }

        ignore = gtk_toggle_button_get_active (button);
        updated = update_ignore_paths (&ignore_paths, dialog->priv->mount_path, ignore);

        g_strfreev (settings_list);

        if (updated) {
                GSList *l;
                GPtrArray *array = g_ptr_array_new ();

                for (l = ignore_paths; l != NULL; l = l->next)
                        g_ptr_array_add (array, l->data);
                g_ptr_array_add (array, NULL);

                if (!g_settings_set_strv (settings, "ignore-paths", (const gchar **) array->pdata)) {
                        g_warning ("Cannot change ignore preference - failed to commit changes");
                }

                g_ptr_array_free (array, FALSE);
        }

        g_slist_foreach (ignore_paths, (GFunc) g_free, NULL);
        g_slist_free (ignore_paths);
        g_object_unref (settings);
}