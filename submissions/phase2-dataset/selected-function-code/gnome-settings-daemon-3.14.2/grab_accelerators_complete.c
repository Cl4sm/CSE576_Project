static void
grab_accelerators_complete (GObject      *object,
                            GAsyncResult *result,
                            gpointer      user_data)
{
        GVariant *actions;
        gboolean retry = FALSE;
        GError *error = NULL;
        GsdMediaKeysManager *manager = user_data;

        shell_key_grabber_call_grab_accelerators_finish (SHELL_KEY_GRABBER (object),
                                                         &actions, result, &error);

        if (error) {
                retry = (error->code == G_DBUS_ERROR_UNKNOWN_METHOD);
                if (!retry)
                        g_warning ("%d: %s", error->code, error->message);
                g_error_free (error);
        } else {
                int i;
                for (i = 0; i < manager->priv->keys->len; i++) {
                        MediaKey *key;

                        key = g_ptr_array_index (manager->priv->keys, i);
                        g_variant_get_child (actions, i, "u", &key->accel_id);
                }
        }

        if (retry) {
                guint id;
                id = g_timeout_add_seconds (SHELL_GRABBER_RETRY_INTERVAL,
                                            retry_grabs, manager);
                g_source_set_name_by_id (id, "[gnome-settings-daemon] retry_grabs");
        }
}