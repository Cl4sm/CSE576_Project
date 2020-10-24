static gboolean
gsd_smartcard_manager_idle_cb (GsdSmartcardManager *self)
{
        GsdSmartcardManagerPrivate *priv = self->priv;

        gnome_settings_profile_start (NULL);

        priv->cancellable = g_cancellable_new();
        priv->settings = g_settings_new (CONF_SCHEMA);

        load_nss (self);

        gsd_smartcard_service_new_async (self,
                                         priv->cancellable,
                                         (GAsyncReadyCallback) on_service_created,
                                         self);

        gnome_settings_profile_end (NULL);

        priv->start_idle_id = 0;
        return FALSE;
}