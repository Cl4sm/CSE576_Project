static void
init_kbd (GsdMediaKeysManager *manager)
{
        char **custom_paths;
        int i;

        gnome_settings_profile_start (NULL);

        /* Media keys
         * Add hard-coded shortcuts first so that they can't be preempted */
        for (i = 0; i < G_N_ELEMENTS (media_keys); i++) {
                if (media_keys[i].hard_coded)
                        add_key (manager, i);
        }
        for (i = 0; i < G_N_ELEMENTS (media_keys); i++) {
                if (media_keys[i].hard_coded == NULL)
                        add_key (manager, i);
        }

        /* Custom shortcuts */
        custom_paths = g_settings_get_strv (manager->priv->settings,
                                            "custom-keybindings");

        for (i = 0; i < g_strv_length (custom_paths); i++) {
                MediaKey *key;

                g_debug ("Setting up custom keybinding %s", custom_paths[i]);

                key = media_key_new_for_path (manager, custom_paths[i]);
                if (!key) {
                        continue;
                }
                g_ptr_array_add (manager->priv->keys, key);
        }
        g_strfreev (custom_paths);

        grab_media_keys (manager);

        gnome_settings_profile_end (NULL);
}