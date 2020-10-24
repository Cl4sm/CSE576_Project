static gboolean
renew_subscription_with_connection_test (gpointer user_data)
{
        GSocketClient *client;
        gchar         *address;
        int            port;

        port = ippPort ();

        address = g_strdup_printf ("%s:%d", cupsServer (), port);

        if (address && address[0] != '/') {
                client = g_socket_client_new ();

                g_debug ("Initiating test connection to CUPS server \'%s:%d\'.", cupsServer (), port);

                g_socket_client_connect_to_host_async (client,
                                                       address,
                                                       port,
                                                       NULL,
                                                       renew_subscription_with_connection_test_cb,
                                                       user_data);

                g_object_unref (client);
        } else {
                renew_subscription (user_data);
        }

        g_free (address);

        return TRUE;
}