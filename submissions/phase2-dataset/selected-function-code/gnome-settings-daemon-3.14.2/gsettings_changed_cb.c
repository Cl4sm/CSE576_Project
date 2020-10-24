static void
gsettings_changed_cb (GSettings           *settings,
                      const gchar         *settings_key,
                      GsdMediaKeysManager *manager)
{
        int      i;

        /* Give up if we don't have proxy to the shell */
        if (!manager->priv->key_grabber)
                return;

	/* handled in gsettings_custom_changed_cb() */
        if (g_str_equal (settings_key, "custom-keybindings"))
		return;

	/* not needed here */
        if (g_str_equal (settings_key, "max-screencast-length"))
		return;

	/* Handled by the core */
        if (g_str_equal (settings_key, "active"))
                return;

        /* Find the key that was modified */
        if (manager->priv->keys == NULL)
                return;

        for (i = 0; i < manager->priv->keys->len; i++) {
                MediaKey *key;

                key = g_ptr_array_index (manager->priv->keys, i);

                /* Skip over hard-coded and GConf keys */
                if (key->settings_key == NULL)
                        continue;
                if (strcmp (settings_key, key->settings_key) == 0) {
                        grab_media_key (key, manager);
                        break;
                }
        }
}