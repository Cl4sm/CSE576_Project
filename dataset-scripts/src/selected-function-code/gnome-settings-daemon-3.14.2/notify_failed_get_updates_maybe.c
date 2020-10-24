static void
notify_failed_get_updates_maybe (GsdUpdatesManager *manager)
{
        const gchar *button;
        const gchar *message;
        const gchar *title;
        gboolean ret;
        GError *error = NULL;
        NotifyNotification *notification;

        /* give the user a break */
        if (manager->priv->failed_get_updates_count++ < MAX_FAILED_GET_UPDATES) {
                g_debug ("failed GetUpdates, but will retry %i more times before notification",
                         MAX_FAILED_GET_UPDATES - manager->priv->failed_get_updates_count);
                goto out;
        }

        /* TRANSLATORS: the updates mechanism */
        title = _("Updates");

        /* TRANSLATORS: we failed to get the updates multiple times,
         * and now we need to inform the user that something might be wrong */
        message = _("Unable to access software updates");

        /* TRANSLATORS: try again, this time launching the update viewer */
        button = _("Try again");

        notification = notify_notification_new (title,
                                                message,
                                                GSD_UPDATES_ICON_NORMAL);
        notify_notification_set_hint_string (notification, "desktop-entry", "gpk-update-viewer");
        notify_notification_set_app_name (notification, _("Software Updates"));
        notify_notification_set_timeout (notification, 120*1000);
        notify_notification_set_urgency (notification, NOTIFY_URGENCY_NORMAL);
        notify_notification_add_action (notification, "show-update-viewer",
                                        button,
                                        libnotify_action_cb,
                                        manager, NULL);
        g_signal_connect (notification, "closed",
                          G_CALLBACK (on_notification_closed), NULL);
        ret = notify_notification_show (notification, &error);
        if (!ret) {
                g_warning ("failed to show notification: %s",
                           error->message);
                g_error_free (error);
        }
out:
        /* reset, even if the message failed */
        manager->priv->failed_get_updates_count = 0;
}