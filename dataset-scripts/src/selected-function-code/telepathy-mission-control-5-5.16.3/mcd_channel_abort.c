static void
mcd_channel_abort (McdMission *mission)
{
    McdChannel *channel = MCD_CHANNEL (mission);
    McdChannelPrivate *priv = channel->priv;

    DEBUG ("%p", mission);
    if (priv->is_aborted)
    {
        DEBUG ("Already aborted");
        return;
    }
    priv->is_aborted = TRUE;
    /* If this is still a channel request, signal the failure */
    if (priv->status == MCD_CHANNEL_STATUS_REQUEST ||
        priv->status == MCD_CHANNEL_STATUS_REQUESTED ||
        priv->status == MCD_CHANNEL_STATUS_DISPATCHING ||
        priv->status == MCD_CHANNEL_STATUS_HANDLER_INVOKED)
    {
        /* this code-path can only happen if the connection is aborted, as in
         * the other cases we handle the error in McdChannel; for this reason,
         * we use the DISCONNECTED error code */
        GError *error = g_error_new (TP_ERROR, TP_ERROR_DISCONNECTED,
                                     "Channel aborted");
        mcd_channel_take_error (channel, error);
    }

    _mcd_channel_set_status (channel, MCD_CHANNEL_STATUS_ABORTED);

    /* chain up with the parent */
    MCD_MISSION_CLASS (mcd_channel_parent_class)->abort (mission);
}