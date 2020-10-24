void
gsd_print_notifications_manager_stop (GsdPrintNotificationsManager *manager)
{
        TimeoutData *data;
        ReasonData  *reason_data;
        GList       *tmp;

        g_debug ("Stopping print-notifications manager");

        cupsFreeDests (manager->priv->num_dests, manager->priv->dests);
        manager->priv->num_dests = 0;
        manager->priv->dests = NULL;

        if (manager->priv->cups_dbus_subscription_id > 0 &&
            manager->priv->cups_bus_connection != NULL) {
                g_dbus_connection_signal_unsubscribe (manager->priv->cups_bus_connection,
                                                      manager->priv->cups_dbus_subscription_id);
                manager->priv->cups_dbus_subscription_id = 0;
        }

        renew_subscription_timeout_enable (manager, FALSE, FALSE);

        if (manager->priv->check_source_id > 0) {
                g_source_remove (manager->priv->check_source_id);
                manager->priv->check_source_id = 0;
        }

        if (manager->priv->subscription_id >= 0)
                cancel_subscription (manager->priv->subscription_id);

        g_clear_pointer (&manager->priv->printing_printers, g_hash_table_destroy);

        g_clear_object (&manager->priv->cups_bus_connection);

        for (tmp = manager->priv->timeouts; tmp; tmp = g_list_next (tmp)) {
                data = (TimeoutData *) tmp->data;
                if (data)
                        g_source_remove (data->timeout_id);
        }
        g_list_free_full (manager->priv->timeouts, free_timeout_data);

        for (tmp = manager->priv->active_notifications; tmp; tmp = g_list_next (tmp)) {
                reason_data = (ReasonData *) tmp->data;
                if (reason_data) {
                        if (reason_data->notification_close_id > 0 &&
                            g_signal_handler_is_connected (reason_data->notification,
                                                           reason_data->notification_close_id)) {
                                g_signal_handler_disconnect (reason_data->notification,
                                                             reason_data->notification_close_id);
                                reason_data->notification_close_id = 0;
                        }

                        notify_notification_close (reason_data->notification, NULL);
                }
        }
        g_list_free_full (manager->priv->active_notifications, free_reason_data);

        scp_handler (manager, FALSE);
}