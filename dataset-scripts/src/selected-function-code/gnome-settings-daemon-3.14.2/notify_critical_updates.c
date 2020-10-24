static void
notify_critical_updates (GsdUpdatesManager *manager, GPtrArray *array)
{
        const gchar *message;
        const gchar *title;
        gboolean ret;
        GError *error = NULL;
        NotifyNotification *notification;

        /* if the number of critical updates is the same as the last notification,
         * then skip the notifcation as we don't want to bombard the user every hour */
        if (array->len == manager->priv->number_updates_critical_last_shown) {
                g_debug ("ignoring as user ignored last warning");
                return;
        }

        /* save for comparison later */
        manager->priv->number_updates_critical_last_shown = array->len;

        /* TRANSLATORS: title in the libnotify popup */
        title = ngettext ("Update", "Updates", array->len);

        /* TRANSLATORS: message when there are security updates */
        message = ngettext ("An important software update is available",
                            "Important software updates are available", array->len);

        /* close any existing notification */
        if (manager->priv->notification_updates != NULL) {
                notify_notification_close (manager->priv->notification_updates, NULL);
                manager->priv->notification_updates = NULL;
        }

        /* do the bubble */
        g_debug ("title=%s, message=%s", title, message);
        notification = notify_notification_new (title,
                                                message,
                                                GSD_UPDATES_ICON_URGENT);
        notify_notification_set_hint_string (notification, "desktop-entry", "gpk-update-viewer");
        notify_notification_set_app_name (notification, _("Software Updates"));
        notify_notification_set_timeout (notification, 15000);
        notify_notification_set_urgency (notification, NOTIFY_URGENCY_CRITICAL);
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
        /* track so we can prevent doubled notifications */
        manager->priv->notification_updates = notification;
        g_object_add_weak_pointer (G_OBJECT (manager->priv->notification_updates),
                                   (void **) &manager->priv->notification_updates);
}