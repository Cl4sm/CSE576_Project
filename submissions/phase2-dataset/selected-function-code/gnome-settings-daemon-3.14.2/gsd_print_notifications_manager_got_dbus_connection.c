static void
gsd_print_notifications_manager_got_dbus_connection (GObject      *source_object,
                                                     GAsyncResult *res,
                                                     gpointer      user_data)
{
        GsdPrintNotificationsManager *manager = (GsdPrintNotificationsManager *) user_data;
        GError                       *error = NULL;

        manager->priv->cups_bus_connection = g_bus_get_finish (res, &error);

        if (manager->priv->cups_bus_connection != NULL) {
                manager->priv->cups_dbus_subscription_id =
                        g_dbus_connection_signal_subscribe (manager->priv->cups_bus_connection,
                                                            NULL,
                                                            CUPS_DBUS_INTERFACE,
                                                            NULL,
                                                            CUPS_DBUS_PATH,
                                                            NULL,
                                                            0,
                                                            on_cups_notification,
                                                            manager,
                                                            NULL);
        } else {
                g_warning ("Connection to message bus failed: %s", error->message);
                g_error_free (error);
        }
}