static void
mcd_dispatch_operation_channel_aborted_cb (McdChannel *channel,
                                           McdDispatchOperation *self)
{
    const GError *error;

    g_object_ref (self);    /* FIXME: use a GObject closure or something */

    DEBUG ("Channel %p aborted while in a dispatch operation", channel);

    /* if it was a channel request, and it was cancelled, then the whole
     * dispatch operation should be aborted, closing any related channels */
    error = mcd_channel_get_error (channel);
    if (error && error->code == TP_ERROR_CANCELLED)
        self->priv->cancelled = TRUE;

    _mcd_dispatch_operation_lose_channel (self, channel);

    if (self->priv->channel == NULL)
    {
        DEBUG ("Nothing left in this context");
    }

    g_object_unref (self);
}