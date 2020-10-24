static gboolean
ax_slowkeys_warning_post_bubble (GsdA11yKeyboardManager *manager,
                                 gboolean                enabled)
{
        gboolean    res;
        const char *title;
        const char *message;
        GError     *error;

        title = enabled ?
                _("Slow Keys Turned On") :
                _("Slow Keys Turned Off");
        message = _("You just held down the Shift key for 8 seconds.  This is the shortcut "
                    "for the Slow Keys feature, which affects the way your keyboard works.");

        if (manager->priv->notification != NULL) {
                notify_notification_close (manager->priv->notification, NULL);
        }

        manager->priv->notification = notify_notification_new (title,
                                                               message,
                                                               "preferences-desktop-accessibility-symbolic");
        notify_notification_set_app_name (manager->priv->notification, _("Universal Access"));
        notify_notification_set_timeout (manager->priv->notification, 0);
        notify_notification_set_urgency (manager->priv->notification, NOTIFY_URGENCY_CRITICAL);

        notify_notification_add_action (manager->priv->notification,
                                        "reject",
                                        enabled ? _("Turn Off") : _("Turn On"),
                                        (NotifyActionCallback) on_slow_keys_action,
                                        manager,
                                        NULL);
        notify_notification_add_action (manager->priv->notification,
                                        "accept",
                                        enabled ? _("Leave On") : _("Leave Off"),
                                        (NotifyActionCallback) on_slow_keys_action,
                                        manager,
                                        NULL);

        g_signal_connect (manager->priv->notification,
                          "closed",
                          G_CALLBACK (on_notification_closed),
                          manager);

        error = NULL;
        res = notify_notification_show (manager->priv->notification, &error);
        if (! res) {
                g_warning ("GsdA11yKeyboardManager: unable to show notification: %s", error->message);
                g_error_free (error);
                notify_notification_close (manager->priv->notification, NULL);
        }

        return res;
}