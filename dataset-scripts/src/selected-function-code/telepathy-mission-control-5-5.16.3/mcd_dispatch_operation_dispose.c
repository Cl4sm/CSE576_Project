static void
mcd_dispatch_operation_dispose (GObject *object)
{
    McdDispatchOperationPrivate *priv = MCD_DISPATCH_OPERATION_PRIV (object);

    tp_clear_object (&priv->plugin_api);
    tp_clear_object (&priv->successful_handler);

    if (priv->channel != NULL)
    {
        g_signal_handlers_disconnect_by_func (priv->channel,
            mcd_dispatch_operation_channel_aborted_cb, object);
    }

    tp_clear_object (&priv->channel);
    tp_clear_object (&priv->lost_channel);
    tp_clear_object (&priv->account);
    tp_clear_object (&priv->handler_map);
    tp_clear_object (&priv->client_registry);

    if (priv->approvals != NULL)
    {
        g_queue_foreach (priv->approvals, (GFunc) approval_free, NULL);
        tp_clear_pointer (&priv->approvals, g_queue_free);
    }

    G_OBJECT_CLASS (_mcd_dispatch_operation_parent_class)->dispose (object);
}