static void
require_nothing (GsdUpdatesFirmware *firmware)
{
        const gchar *message;
        gboolean ret;
        GError *error = NULL;
        NotifyNotification *notification;

        /* TRANSLATORS: we need to remove an replug so the new hardware can re-request the firmware */
        message = _("Your hardware has been set up and is now ready to use.");

        /* TRANSLATORS: title of libnotify bubble */
        notification = notify_notification_new (_("Additional software was installed"), message, NULL);
        notify_notification_set_hint_string (notification, "desktop-entry", "gpk-update-viewer");
        notify_notification_set_app_name (notification, _("Software Updates"));
        notify_notification_set_timeout (notification, NOTIFY_EXPIRES_NEVER);
        notify_notification_set_urgency (notification, NOTIFY_URGENCY_LOW);
        g_signal_connect (notification, "closed",
                          G_CALLBACK (on_notification_closed), NULL);

        /* show the bubble */
        ret = notify_notification_show (notification, &error);
        if (!ret) {
                g_warning ("error: %s", error->message);
                g_error_free (error);
        }
}