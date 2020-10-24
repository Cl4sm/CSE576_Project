static void
renew_subscription_with_connection_test_cb (GObject      *source_object,
                                            GAsyncResult *res,
                                            gpointer      user_data)
{
        GSocketConnection *connection;
        GError            *error = NULL;

        connection = g_socket_client_connect_to_host_finish (G_SOCKET_CLIENT (source_object),
                                                             res,
                                                             &error);

        if (connection) {
                g_debug ("Test connection to CUPS server \'%s:%d\' succeeded.", cupsServer (), ippPort ());

                g_io_stream_close (G_IO_STREAM (connection), NULL, NULL);
                g_object_unref (connection);

                renew_subscription (user_data);
        } else {
                g_debug ("Test connection to CUPS server \'%s:%d\' failed.", cupsServer (), ippPort ());
        }
}