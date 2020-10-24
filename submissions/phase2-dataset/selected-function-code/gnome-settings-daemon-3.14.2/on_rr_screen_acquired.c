static void
on_rr_screen_acquired (GObject      *object,
                       GAsyncResult *result,
                       gpointer      user_data)
{
        GsdXrandrManager *manager = user_data;
        GError *error = NULL;

        manager->priv->rw_screen = gnome_rr_screen_new_finish (result, &error);

        if (manager->priv->rw_screen == NULL) {
                log_msg ("Could not initialize the RANDR plugin: %s\n",
                         error->message);
                g_error_free (error);
                log_close ();
                return;
        }

        manager->priv->upower_client = up_client_new ();

        log_msg ("State of screen at startup:\n");
        log_screen (manager->priv->rw_screen);

        manager->priv->running = TRUE;
        manager->priv->settings = g_settings_new (CONF_SCHEMA);

        manager_init_devices (manager);
        register_manager_dbus (manager);

        log_close ();
}