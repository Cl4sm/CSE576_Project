static void
_mcd_dispatch_operation_close_as_undispatchable (McdDispatchOperation *self,
                                                 const GError *error)
{
    /* All of the usable handlers vanished while we were thinking about it
     * (this can only happen if non-activatable handlers exit after we
     * include them in the list of possible handlers, but before we .
     * We should recover in some better way, perhaps by asking all the
     * approvers again (?), but for now we'll just close all the channels. */

    DEBUG ("%s", error->message);
    _mcd_dispatch_operation_finish (self, error->domain, error->code,
                                    "%s", error->message);

    if (self->priv->channel != NULL)
    {
        McdChannel *channel = MCD_CHANNEL (self->priv->channel);
        GError e = { TP_ERROR, TP_ERROR_NOT_AVAILABLE,
            "Handler no longer available" };

        g_object_ref (channel);
        mcd_channel_take_error (channel, g_error_copy (&e));
        _mcd_channel_undispatchable (channel);
        g_object_unref (channel);
    }
}