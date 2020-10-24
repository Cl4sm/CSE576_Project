static void
mcd_dispatch_operation_try_handler (McdDispatchOperation *self,
                                    McdClientProxy *handler)
{
    TpClient *handler_client = (TpClient *) handler;
    const GList *p;
    McpDispatchOperation *plugin_api = MCP_DISPATCH_OPERATION (
        self->priv->plugin_api);

    g_assert (self->priv->trying_handler == NULL);
    self->priv->trying_handler = g_object_ref (handler);

    self->priv->handler_suitable_pending = 0;

    DEBUG ("%s: channel ACL verification", self->priv->unique_name);

    for (p = mcp_list_objects (); p != NULL; p = g_list_next (p))
    {
        if (MCP_IS_DISPATCH_OPERATION_POLICY (p->data))
        {
            McpDispatchOperationPolicy *plugin = p->data;

            DEBUG ("%s: checking policy for %s",
                G_OBJECT_TYPE_NAME (plugin),
                tp_proxy_get_object_path (handler));

            self->priv->handler_suitable_pending++;
            mcp_dispatch_operation_policy_handler_is_suitable_async (plugin,
                    handler_client,
                    _mcd_client_proxy_get_unique_name (handler),
                    plugin_api,
                    mcd_dispatch_operation_handler_decision_cb,
                    g_object_ref (self));
        }
    }

    if (self->priv->handler_suitable_pending == 0)
    {
        mcd_dispatch_operation_handle_channels (self);
    }
}