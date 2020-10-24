static void
cups_connection_test_cb (GObject      *source_object,
                         GAsyncResult *res,
                         gpointer      user_data)
{
        GsdPrintNotificationsManager *manager = (GsdPrintNotificationsManager *) user_data;
        GSocketConnection            *connection;
        GError                       *error = NULL;

        connection = g_socket_client_connect_to_host_finish (G_SOCKET_CLIENT (source_object),
                                                             res,
                                                             &error);

        if (connection) {
                g_debug ("Test connection to CUPS server \'%s:%d\' succeeded.", cupsServer (), ippPort ());

                g_io_stream_close (G_IO_STREAM (connection), NULL, NULL);
                g_object_unref (connection);

                manager->priv->num_dests = cupsGetDests (&manager->priv->dests);
                g_debug ("Got dests from remote CUPS server.");

                renew_subscription_timeout_enable (manager, TRUE, TRUE);
                manager->priv->check_source_id = g_timeout_add_seconds (CHECK_INTERVAL, process_new_notifications, manager);
                g_source_set_name_by_id (manager->priv->check_source_id, "[gnome-settings-daemon] process_new_notifications");
        } else {
                g_debug ("Test connection to CUPS server \'%s:%d\' failed.", cupsServer (), ippPort ());
                if (manager->priv->cups_connection_timeout_id == 0) {
                        manager->priv->cups_connection_timeout_id =
                                g_timeout_add_seconds (CUPS_CONNECTION_TEST_INTERVAL, cups_connection_test, manager);
                        g_source_set_name_by_id (manager->priv->cups_connection_timeout_id, "[gnome-settings-daemon] cups_connection_test");
                }
        }
}