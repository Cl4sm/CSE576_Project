static void
shell_presence_changed (GsdMediaKeysManager *manager)
{
        gchar *name_owner;

        name_owner = g_dbus_proxy_get_name_owner (G_DBUS_PROXY (manager->priv->shell_proxy));

        if (name_owner) {
                shell_key_grabber_proxy_new_for_bus (G_BUS_TYPE_SESSION,
                                                     0,
                                                     name_owner,
                                                     SHELL_DBUS_PATH,
                                                     manager->priv->grab_cancellable,
                                                     on_key_grabber_ready, manager);

                g_dbus_proxy_new_for_bus (G_BUS_TYPE_SESSION,
                                          0, NULL,
                                          name_owner,
                                          SHELL_DBUS_PATH "/Screencast",
                                          SHELL_DBUS_NAME ".Screencast",
                                          manager->priv->screencast_cancellable,
                                          on_screencast_proxy_ready, manager);
                g_free (name_owner);
        } else {
                g_ptr_array_set_size (manager->priv->keys, 0);
                g_clear_object (&manager->priv->key_grabber);
                g_clear_object (&manager->priv->screencast_proxy);
        }
}