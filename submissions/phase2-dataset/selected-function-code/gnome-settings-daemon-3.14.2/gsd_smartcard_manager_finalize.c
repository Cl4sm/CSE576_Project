static void
gsd_smartcard_manager_finalize (GObject *object)
{
        GsdSmartcardManager *self;
        GsdSmartcardManagerPrivate *priv;

        g_return_if_fail (object != NULL);
        g_return_if_fail (GSD_IS_SMARTCARD_MANAGER (object));

        self = GSD_SMARTCARD_MANAGER (object);
        priv = self->priv;

        g_return_if_fail (self->priv != NULL);

        if (priv->start_idle_id != 0)
                g_source_remove (priv->start_idle_id);

        gsd_smartcard_manager_stop (self);

        G_OBJECT_CLASS (gsd_smartcard_manager_parent_class)->finalize (object);
}