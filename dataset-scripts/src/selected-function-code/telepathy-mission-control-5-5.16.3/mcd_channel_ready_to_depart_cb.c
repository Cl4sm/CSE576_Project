static void
mcd_channel_ready_to_depart_cb (GObject *source_object,
                                GAsyncResult *result,
                                gpointer data)
{
    TpChannel *channel = TP_CHANNEL (source_object);
    DepartData *d = data;
    GError *error = NULL;

    if (!tp_proxy_prepare_finish (channel, result, &error))
    {
        DEBUG ("%s %d: %s", g_quark_to_string (error->domain), error->code,
               error->message);
        g_free (d->message);
        g_slice_free (DepartData, d);
        g_clear_error (&error);
        return;
    }

    /* If it's a Group, this will leave gracefully.
     * If not, it will just close it. Either's good. */
    tp_channel_leave_async (channel, d->reason, d->message,
                            mcd_channel_depart_cb, NULL);
}