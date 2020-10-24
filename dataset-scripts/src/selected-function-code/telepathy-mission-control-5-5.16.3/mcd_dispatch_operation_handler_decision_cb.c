static void
mcd_dispatch_operation_handler_decision_cb (GObject *source,
                                            GAsyncResult *res,
                                            gpointer user_data)
{
    McdDispatchOperation *self = user_data;
    GError *error = NULL;

    if (!mcp_dispatch_operation_policy_handler_is_suitable_finish (
            MCP_DISPATCH_OPERATION_POLICY (source), res, &error))
    {
        /* ignore any errors after the first */
        if (self->priv->handler_unsuitable == NULL)
            g_propagate_error (&self->priv->handler_unsuitable, error);
        else
            g_error_free (error);
    }

    if (--self->priv->handler_suitable_pending == 0)
    {
        mcd_dispatch_operation_handle_channels (self);
    }

    g_object_unref (self);
}