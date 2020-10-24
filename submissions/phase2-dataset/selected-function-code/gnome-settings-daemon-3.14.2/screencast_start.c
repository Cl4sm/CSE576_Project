static void
screencast_start (GsdMediaKeysManager *manager)
{
        guint max_length;
        g_dbus_proxy_call (manager->priv->screencast_proxy,
                           "Screencast",
                           /* Translators: this is a filename used for screencast
                            * recording, where "%d" and "%t" date and time, e.g.
                            * "Screencast from 07-17-2013 10:00:46 PM.webm" */
                           /* xgettext:no-c-format */
                           g_variant_new_parsed ("(%s, @a{sv} {})",
                                                 _("Screencast from %d %t.webm")),
                           G_DBUS_CALL_FLAGS_NONE, -1,
                           manager->priv->screencast_cancellable,
                           NULL, NULL);

        max_length = g_settings_get_uint (manager->priv->settings, "max-screencast-length");

        if (max_length > 0) {
                manager->priv->screencast_timeout_id = g_timeout_add_seconds (max_length,
                                                                              screencast_timeout,
                                                                              manager);
                g_source_set_name_by_id (manager->priv->screencast_timeout_id, "[gnome-settings-daemon] screencast_timeout");
        }
        manager->priv->screencast_recording = TRUE;
}