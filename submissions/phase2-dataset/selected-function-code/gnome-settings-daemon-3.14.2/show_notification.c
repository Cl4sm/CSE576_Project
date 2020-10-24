static gboolean
show_notification (gpointer user_data)
{
        NotifyNotification *notification;
        TimeoutData        *data = (TimeoutData *) user_data;
        ReasonData         *reason_data;
        GList              *tmp;

        if (!data)
                return FALSE;

        notification = notify_notification_new (data->primary_text,
                                                data->secondary_text,
                                                "printer-symbolic");

        notify_notification_set_app_name (notification, _("Printers"));
        notify_notification_set_hint (notification,
                                      "resident",
                                      g_variant_new_boolean (TRUE));
        notify_notification_set_timeout (notification, REASON_TIMEOUT);

        reason_data = g_new0 (ReasonData, 1);
        reason_data->printer_name = g_strdup (data->printer_name);
        reason_data->reason = g_strdup ("connecting-to-device");
        reason_data->notification = notification;
        reason_data->manager = data->manager;

        reason_data->notification_close_id =
                g_signal_connect (notification,
                                  "closed",
                                  G_CALLBACK (notification_closed_cb),
                                  reason_data);

        reason_data->manager->priv->active_notifications =
                g_list_append (reason_data->manager->priv->active_notifications, reason_data);

        notify_notification_show (notification, NULL);

        tmp = g_list_find (data->manager->priv->timeouts, data);
        if (tmp) {
                data->manager->priv->timeouts = g_list_remove_link (data->manager->priv->timeouts, tmp);
                g_list_free_full (tmp, free_timeout_data);
        }

        return FALSE;
}