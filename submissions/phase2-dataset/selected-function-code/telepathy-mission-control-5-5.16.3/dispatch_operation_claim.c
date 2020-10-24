static void
dispatch_operation_claim (TpSvcChannelDispatchOperation *cdo,
                          DBusGMethodInvocation *context)
{
    McdDispatchOperation *self = MCD_DISPATCH_OPERATION (cdo);
    ClaimAttempt *claim_attempt;
    gchar *sender = dbus_g_method_get_sender (context);
    McpDispatchOperation *plugin_api = MCP_DISPATCH_OPERATION (
        self->priv->plugin_api);
    const GList *p;

    if (self->priv->result != NULL)
    {

        DEBUG ("Giving error to %s: %s", sender, self->priv->result->message);
        dbus_g_method_return_error (context, self->priv->result);
        goto finally;
    }

    claim_attempt = g_slice_new0 (ClaimAttempt);
    claim_attempt->self = g_object_ref (self);
    claim_attempt->context = context;
    claim_attempt->handler_suitable_pending = 0;

    for (p = mcp_list_objects (); p != NULL; p = g_list_next (p))
    {
        if (MCP_IS_DISPATCH_OPERATION_POLICY (p->data))
        {
            McpDispatchOperationPolicy *plugin = p->data;

            DEBUG ("%s: checking policy for %s",
                G_OBJECT_TYPE_NAME (plugin), sender);

            claim_attempt->handler_suitable_pending++;
            mcp_dispatch_operation_policy_handler_is_suitable_async (plugin,
                    NULL, sender, plugin_api,
                    claim_attempt_suitability_cb,
                    claim_attempt);
        }
    }

    if (claim_attempt->handler_suitable_pending == 0)
        claim_attempt_resolve (claim_attempt);

finally:
    g_free (sender);
}