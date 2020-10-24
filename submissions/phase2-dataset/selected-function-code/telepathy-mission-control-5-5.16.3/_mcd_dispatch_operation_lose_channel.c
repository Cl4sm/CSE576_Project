static void
_mcd_dispatch_operation_lose_channel (McdDispatchOperation *self,
                                      McdChannel *channel)
{
    const gchar *object_path;
    const GError *error = NULL;

    if (G_UNLIKELY (channel != self->priv->channel))
    {
        g_warning ("%s: apparently lost %p but my channel is %p",
                   G_STRFUNC, channel, self->priv->channel);
        return;
    }

    /* steal the reference */
    self->priv->channel = NULL;

    object_path = mcd_channel_get_object_path (channel);
    error = mcd_channel_get_error (channel);

    if (object_path == NULL)
    {
        /* This shouldn't happen, but McdChannel is twisty enough that I
         * can't be sure */
        g_critical ("McdChannel has already lost its TpChannel: %p",
            channel);
    }
    else if (!mcd_dispatch_operation_may_signal_finished (self))
    {
        /* We're still invoking approvers, so we're not allowed to talk
         * about it right now. Instead, save the signal for later. */
        DEBUG ("%s/%p not losing channel %s just yet: "
               "waiting for %" G_GSIZE_FORMAT " observers, "
               "%" G_GSIZE_FORMAT " approvers",
               self->priv->unique_name, self, object_path,
               self->priv->observers_pending, self->priv->ado_pending);
        g_assert (self->priv->lost_channel == NULL);
        self->priv->lost_channel = g_object_ref (channel);
    }
    else
    {
        gchar *error_name = _mcd_build_error_string (error);

        DEBUG ("%s/%p losing channel %s: %s: %s",
               self->priv->unique_name, self, object_path, error_name,
               error->message);
        tp_svc_channel_dispatch_operation_emit_channel_lost (self, object_path,
                                                             error_name,
                                                             error->message);
        g_free (error_name);
    }

    /* We previously stole this ref from self->priv->channel - drop it */
    g_object_unref (channel);

    /* no channels left, so the CDO finishes (if it hasn't already) */
    _mcd_dispatch_operation_finish (self, error->domain, error->code,
                                    "%s", error->message);
}