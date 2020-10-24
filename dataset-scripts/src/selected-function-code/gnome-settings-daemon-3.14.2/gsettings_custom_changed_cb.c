static void
gsettings_custom_changed_cb (GSettings           *settings,
                             const char          *settings_key,
                             GsdMediaKeysManager *manager)
{
        char **bindings;
        int i, j, n_bindings;

        bindings = g_settings_get_strv (settings, settings_key);
        n_bindings = g_strv_length (bindings);

        /* Handle additions */
        for (i = 0; i < n_bindings; i++) {
                if (g_hash_table_lookup (manager->priv->custom_settings,
                                         bindings[i]))
                        continue;
                update_custom_binding (manager, bindings[i]);
        }

        /* Handle removals */
        for (i = 0; i < manager->priv->keys->len; i++) {
                gboolean found = FALSE;
                MediaKey *key = g_ptr_array_index (manager->priv->keys, i);
                if (key->key_type != CUSTOM_KEY)
                        continue;

                for (j = 0; j < n_bindings && !found; j++)
                        found = strcmp (bindings[j], key->custom_path) == 0;

                if (found)
                        continue;

                ungrab_media_key (key, manager);
                g_hash_table_remove (manager->priv->custom_settings,
                                     key->custom_path);
                g_ptr_array_remove_index_fast (manager->priv->keys, i);
                --i; /* make up for the removed key */
        }
        g_strfreev (bindings);
}