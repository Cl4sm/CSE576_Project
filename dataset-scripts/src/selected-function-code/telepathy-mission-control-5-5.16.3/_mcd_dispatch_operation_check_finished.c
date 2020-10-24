static void
_mcd_dispatch_operation_check_finished (McdDispatchOperation *self)
{
    if (mcd_dispatch_operation_may_signal_finished (self))
    {
        McdChannel *lost_channel = self->priv->lost_channel;

        /* steal it */
        self->priv->lost_channel = NULL;

        if (lost_channel != NULL)
        {
            const gchar *object_path = mcd_channel_get_object_path (
                lost_channel);

            if (object_path == NULL)
            {
                /* This shouldn't happen, but McdChannel is twisty enough
                 * that I can't be sure */
                g_critical ("McdChannel has already lost its TpChannel: %p",
                    lost_channel);
            }
            else
            {
                const GError *error = mcd_channel_get_error (lost_channel);
                gchar *error_name = _mcd_build_error_string (error);

                DEBUG ("%s/%p losing channel %s: %s: %s",
                       self->priv->unique_name, self, object_path, error_name,
                       error->message);
                tp_svc_channel_dispatch_operation_emit_channel_lost (self,
                    object_path, error_name, error->message);
                g_free (error_name);
            }

            g_object_unref (lost_channel);
        }

        if (self->priv->result != NULL)
        {
            DEBUG ("%s/%p finished", self->priv->unique_name, self);
            mcd_dispatch_operation_actually_finish (self);
        }
    }
    else if (self->priv->result != NULL)
    {
        DEBUG ("%s/%p still unable to finish: "
               "waiting for %" G_GSIZE_FORMAT " observers, "
               "%" G_GSIZE_FORMAT " approvers",
               self->priv->unique_name, self,
               self->priv->observers_pending, self->priv->ado_pending);
    }
}