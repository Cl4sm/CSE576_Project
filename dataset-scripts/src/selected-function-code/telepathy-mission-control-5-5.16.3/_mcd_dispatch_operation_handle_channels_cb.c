static void
_mcd_dispatch_operation_handle_channels_cb (TpClient *client,
                                            const GError *error,
                                            gpointer user_data,
                                            GObject *weak G_GNUC_UNUSED)
{
    McdDispatchOperation *self = user_data;

    if (error)
    {
        DEBUG ("error: %s", error->message);

        _mcd_dispatch_operation_set_handler_failed (self,
            tp_proxy_get_bus_name (client), error);
    }
    else
    {
        /* FIXME: can channel ever be NULL here? */
        if (self->priv->channel != NULL)
        {
            McdChannel *channel = MCD_CHANNEL (self->priv->channel);
            const gchar *unique_name;

            unique_name = _mcd_client_proxy_get_unique_name (MCD_CLIENT_PROXY (client));

            /* This should always be false in practice - either we already know
             * the handler's unique name (because active handlers' unique names
             * are discovered before their handler filters), or the handler
             * is activatable and was not running, the handler filter came
             * from a .client file, and the bus daemon activated the handler
             * as a side-effect of HandleChannels (in which case
             * NameOwnerChanged should have already been emitted by the time
             * we got a reply to HandleChannels).
             *
             * We recover by whining to stderr and closing the channels, in the
             * interests of at least failing visibly.
             *
             * If dbus-glib exposed more of the details of the D-Bus message
             * passing system, then we could just look at the sender of the
             * reply and bypass this rubbish...
             */
            if (G_UNLIKELY (unique_name == NULL || unique_name[0] == '\0'))
            {
                g_warning ("Client %s returned successfully but doesn't "
                           "exist? dbus-daemon bug suspected",
                           tp_proxy_get_bus_name (client));
                g_warning ("Closing channel %s as a result",
                           mcd_channel_get_object_path (channel));
                _mcd_channel_undispatchable (channel);
            }
            else
            {
                mcd_dispatch_operation_set_channel_handled_by (self, channel,
                    unique_name, tp_proxy_get_bus_name (client));
            }
        }

        /* emit Finished, if we haven't already; but first make a note of the
         * handler we used, so we can reply to all the HandleWith calls with
         * success or failure */
        self->priv->successful_handler = g_object_ref (client);
        _mcd_dispatch_operation_finish (self, TP_ERROR, TP_ERROR_NOT_YOURS,
                                        "Channel successfully handled by %s",
                                        tp_proxy_get_bus_name (client));
    }

    tp_clear_object (&self->priv->trying_handler);
    _mcd_dispatch_operation_check_client_locks (self);
}