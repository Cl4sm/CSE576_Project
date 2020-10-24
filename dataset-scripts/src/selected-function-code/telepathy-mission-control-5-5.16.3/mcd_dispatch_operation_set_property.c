static void
mcd_dispatch_operation_set_property (GObject *obj, guint prop_id,
                                     const GValue *val, GParamSpec *pspec)
{
    McdDispatchOperation *operation = MCD_DISPATCH_OPERATION (obj);
    McdDispatchOperationPrivate *priv = operation->priv;

    switch (prop_id)
    {
    case PROP_CLIENT_REGISTRY:
        g_assert (priv->client_registry == NULL); /* construct-only */
        priv->client_registry = MCD_CLIENT_REGISTRY (g_value_dup_object (val));
        break;

    case PROP_HANDLER_MAP:
        g_assert (priv->handler_map == NULL); /* construct-only */
        priv->handler_map = MCD_HANDLER_MAP (g_value_dup_object (val));
        break;

    case PROP_CHANNEL:
        /* because this is construct-only, we can assert that: */
        g_assert (priv->channel == NULL);
        g_assert (g_queue_is_empty (priv->approvals));

        priv->channel = g_value_dup_object (val);

        if (G_LIKELY (priv->channel))
        {
            const gchar *preferred_handler;

            priv->connection = (McdConnection *)
                mcd_mission_get_parent (MCD_MISSION (priv->channel));

            if (G_LIKELY (priv->connection))
            {
                g_object_ref (priv->connection);
            }
            else
            {
                /* shouldn't happen? */
                g_warning ("Channel has no Connection?!");
            }

            /* if the channel is actually a channel request, get the
             * preferred handler from it */
            preferred_handler =
                _mcd_channel_get_request_preferred_handler (priv->channel);

            if (preferred_handler != NULL &&
                g_str_has_prefix (preferred_handler, TP_CLIENT_BUS_NAME_BASE) &&
                tp_dbus_check_valid_bus_name (preferred_handler,
                                              TP_DBUS_NAME_TYPE_WELL_KNOWN,
                                              NULL))
            {
                DEBUG ("Extracted preferred handler: %s",
                       preferred_handler);
                g_queue_push_tail (priv->approvals,
                                   approval_new_requested (preferred_handler));
            }

            priv->account = mcd_channel_get_account (priv->channel);

            if (G_LIKELY (priv->account != NULL))
            {
                g_object_ref (priv->account);
            }
            else
            {
                /* shouldn't happen? */
                g_warning ("Channel given to McdDispatchOperation has no "
                           "Account?!");
            }

            /* connect to its signals */
            g_signal_connect_after (priv->channel, "abort",
                G_CALLBACK (mcd_dispatch_operation_channel_aborted_cb),
                operation);
        }
        break;

    case PROP_POSSIBLE_HANDLERS:
        g_assert (priv->possible_handlers == NULL);
        priv->possible_handlers = g_value_dup_boxed (val);
        break;

    case PROP_NEEDS_APPROVAL:
        priv->needs_approval = g_value_get_boolean (val);
        break;

    case PROP_OBSERVE_ONLY:
        priv->observe_only = g_value_get_boolean (val);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, prop_id, pspec);
        break;
    }
}