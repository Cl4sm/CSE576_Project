static MediaKey *
media_key_new_for_path (GsdMediaKeysManager *manager,
			char                *path)
{
        GSettings *settings;
        char *command, *binding;
        MediaKey *key;

        g_debug ("media_key_new_for_path: %s", path);

	settings = g_hash_table_lookup (manager->priv->custom_settings, path);
	if (settings == NULL) {
		settings = g_settings_new_with_path (CUSTOM_BINDING_SCHEMA, path);

		g_signal_connect (settings, "changed",
				  G_CALLBACK (custom_binding_changed), manager);
		g_hash_table_insert (manager->priv->custom_settings,
				     g_strdup (path), settings);
	}

        command = g_settings_get_string (settings, "command");
        binding = g_settings_get_string (settings, "binding");

        if (*command == '\0' && *binding == '\0') {
                g_debug ("Key binding (%s) is incomplete", path);
                g_free (command);
                g_free (binding);
                return NULL;
        }
        g_free (binding);

        key = g_new0 (MediaKey, 1);
        key->key_type = CUSTOM_KEY;
        key->modes = GSD_KEYBINDING_MODE_LAUNCHER;
        key->custom_path = g_strdup (path);
        key->custom_command = command;

        return key;
}