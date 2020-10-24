static void
notify_normal_updates_maybe (GsdUpdatesManager *manager, GPtrArray *array)
{
        const gchar *message;
        const gchar *title;
        gboolean ret;
        GError *error = NULL;
        guint64 time_last_notify;
        guint64 time_now;
        guint freq_updates_notify;
        NotifyNotification *notification;

        /* find out if enough time has passed since the last notification */
        time_now = g_get_real_time () / 1000000;
        freq_updates_notify = g_settings_get_int (manager->priv->settings_gsd,
                                                  GSD_SETTINGS_FREQUENCY_UPDATES_NOTIFICATION);
        g_settings_get (manager->priv->settings_gsd,
                        GSD_SETTINGS_LAST_UPDATES_NOTIFICATION,
                        "t", &time_last_notify);
        if (time_last_notify > 0 &&
            (guint64) freq_updates_notify > time_now - time_last_notify) {
                g_debug ("not showing non-critical notification as already shown %i hours ago",
                        (guint) (time_now - time_last_notify) / (60 * 60));
                return;
        }

        /* TRANSLATORS: title in the libnotify popup */
        title = ngettext ("Update", "Updates", array->len);

        /* TRANSLATORS: message when there are non-security updates */
        message = ngettext ("A software update is available.",
                            "Software updates are available.", array->len);

        /* close any existing notification */
        if (manager->priv->notification_updates != NULL) {
                notify_notification_close (manager->priv->notification_updates, NULL);
                manager->priv->notification_updates = NULL;
        }

        /* do the bubble */
        g_debug ("title=%s, message=%s", title, message);
        notification = notify_notification_new (title,
                                                message,
                                                GSD_UPDATES_ICON_NORMAL);
        notify_notification_set_hint_string (notification, "desktop-entry", "gpk-update-viewer");
        notify_notification_set_app_name (notification, _("Software Updates"));
        notify_notification_set_timeout (notification, 15000);
        notify_notification_set_urgency (notification, NOTIFY_URGENCY_NORMAL);
        notify_notification_add_action (notification, "ignore",
                                        /* TRANSLATORS: don't install updates now */
                                        _("Not Now"),
                                        libnotify_action_cb,
                                        manager, NULL);
        notify_notification_add_action (notification, "show-update-viewer",
                                        /* TRANSLATORS: button: open the update viewer to install updates */
                                        _("Install updates"), libnotify_action_cb, manager, NULL);
        g_signal_connect (notification, "closed",
                          G_CALLBACK (on_notification_closed), NULL);
        ret = notify_notification_show (notification, &error);
        if (!ret) {
                g_warning ("error: %s", error->message);
                g_error_free (error);
        }

        /* reset notification time */
        g_settings_set (manager->priv->settings_gsd,
                        GSD_SETTINGS_LAST_UPDATES_NOTIFICATION,
                        "t", time_now);

        /* track so we can prevent doubled notifications */
        manager->priv->notification_updates = notification;
        g_object_add_weak_pointer (G_OBJECT (manager->priv->notification_updates),
                                   (void **) &manager->priv->notification_updates);
}