static void
on_service_created (GObject             *source_object,
                    GAsyncResult        *result,
                    GsdSmartcardManager *self)
{
        GsdSmartcardManagerPrivate *priv = self->priv;
        GsdSmartcardService *service;
        GError *error = NULL;

        service = gsd_smartcard_service_new_finish (result, &error);

        if (service == NULL) {
                g_warning("Couldn't create session bus service: %s", error->message);
                g_error_free (error);
                return;
        }

        priv->service = service;

        watch_smartcards_async (self,
                                priv->cancellable,
                                (GAsyncReadyCallback) on_smartcards_watched,
                                NULL);

}