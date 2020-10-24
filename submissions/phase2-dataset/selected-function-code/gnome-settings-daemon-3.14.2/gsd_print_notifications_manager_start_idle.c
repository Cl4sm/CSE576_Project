static gboolean
gsd_print_notifications_manager_start_idle (gpointer data)
{
        GsdPrintNotificationsManager *manager = data;

        gnome_settings_profile_start (NULL);

        manager->priv->printing_printers = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);


        if (server_is_local (cupsServer ())) {
                manager->priv->num_dests = cupsGetDests (&manager->priv->dests);
                g_debug ("Got dests from local CUPS server.");

                renew_subscription_timeout_enable (manager, TRUE, FALSE);

                g_bus_get (G_BUS_TYPE_SYSTEM,
                           NULL,
                           gsd_print_notifications_manager_got_dbus_connection,
                           data);
        } else {
                cups_connection_test (manager);
        }

        scp_handler (manager, TRUE);

        gnome_settings_profile_end (NULL);

        return G_SOURCE_REMOVE;
}