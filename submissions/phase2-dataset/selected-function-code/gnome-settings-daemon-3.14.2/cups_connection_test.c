static gboolean
cups_connection_test (gpointer user_data)
{
        GsdPrintNotificationsManager *manager = (GsdPrintNotificationsManager *) user_data;
        GSocketClient                *client;
        gchar                        *address;
        int                           port = ippPort ();

        if (!manager->priv->dests) {
                address = g_strdup_printf ("%s:%d", cupsServer (), port);

                client = g_socket_client_new ();

                g_debug ("Initiating test connection to CUPS server \'%s:%d\'.", cupsServer (), port);

                g_socket_client_connect_to_host_async (client,
                                                       address,
                                                       port,
                                                       NULL,
                                                       cups_connection_test_cb,
                                                       manager);

                g_object_unref (client);
                g_free (address);
        }

        if (manager->priv->dests) {
                manager->priv->cups_connection_timeout_id = 0;

                return FALSE;
        } else {
                return TRUE;
        }
}