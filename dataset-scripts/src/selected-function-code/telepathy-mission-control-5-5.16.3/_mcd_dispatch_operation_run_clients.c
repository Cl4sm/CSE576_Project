void
_mcd_dispatch_operation_run_clients (McdDispatchOperation *self)
{
    g_object_ref (self);
    DEBUG ("%s %p", self->priv->unique_name, self);

    if (self->priv->channel != NULL)
    {
        const GList *mini_plugins;

        if (_mcd_dispatch_operation_handlers_can_bypass_observers (self))
        {
            DEBUG ("Bypassing observers");
        }
        else
        {
            DEBUG ("Running observers");
            _mcd_dispatch_operation_run_observers (self);
        }

        for (mini_plugins = mcp_list_objects ();
             mini_plugins != NULL;
             mini_plugins = mini_plugins->next)
        {
            if (MCP_IS_DISPATCH_OPERATION_POLICY (mini_plugins->data))
            {
                mcp_dispatch_operation_policy_check (mini_plugins->data,
                    MCP_DISPATCH_OPERATION (self->priv->plugin_api));
            }
        }
    }

    DEBUG ("All necessary observers invoked");
    self->priv->invoked_observers_if_needed = TRUE;

    DEBUG ("Checking finished/locks");
    _mcd_dispatch_operation_check_finished (self);
    _mcd_dispatch_operation_check_client_locks (self);

    g_object_unref (self);
}