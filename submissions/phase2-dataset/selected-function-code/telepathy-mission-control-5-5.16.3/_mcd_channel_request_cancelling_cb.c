static void
_mcd_channel_request_cancelling_cb (McdRequest *request,
                                    McdChannel *self)
{
    McdChannelStatus status = mcd_channel_get_status (self);

    g_object_ref (self);
    DEBUG ("%p in status %u", self, status);

    mcd_channel_take_error (self, g_error_new (TP_ERROR,
                                               TP_ERROR_CANCELLED,
                                               "Cancelled"));

    /* If we're coming from state REQUEST, the call to the CM hasn't
     * happened yet; now that we're in state FAILED, it never will,
     * because mcd_connection_request_channel() now checks that.
     *
     * If we're coming from state REQUESTED, we need to close the channel
     * when the CM tells us where it is, so we can't now.
     *
     * If we're coming from state DISPATCHING, we need to shoot it down
     * now.
     *
     * Anything else is too late.
     */
    if (status == MCD_CHANNEL_STATUS_DISPATCHING)
    {
        _mcd_channel_close (self);
        mcd_mission_abort ((McdMission *) self);
    }

    g_object_unref (self);
}