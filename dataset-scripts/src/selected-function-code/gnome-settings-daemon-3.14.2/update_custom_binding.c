static void
update_custom_binding (GsdMediaKeysManager *manager,
                       char                *path)
{
        MediaKey *key;
        int i;

        /* Remove the existing key */
        for (i = 0; i < manager->priv->keys->len; i++) {
                key = g_ptr_array_index (manager->priv->keys, i);

                if (key->custom_path == NULL)
                        continue;
                if (strcmp (key->custom_path, path) == 0) {
                        g_debug ("Removing custom key binding %s", path);
                        ungrab_media_key (key, manager);
                        g_ptr_array_remove_index_fast (manager->priv->keys, i);
                        break;
                }
        }

        /* And create a new one! */
        key = media_key_new_for_path (manager, path);
        if (key) {
                g_debug ("Adding new custom key binding %s", path);
                g_ptr_array_add (manager->priv->keys, key);

                grab_media_key (key, manager);
        }
}