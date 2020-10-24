static gboolean
_mcd_dispatch_operation_try_next_handler (McdDispatchOperation *self)
{
    gchar **iter;
    gboolean is_approved = _mcd_dispatch_operation_is_approved (self);
    Approval *approval = g_queue_peek_head (self->priv->approvals);

    /* If there is a preferred Handler chosen by the first Approver or
     * request, it's the first one we'll consider. We'll even consider
     * it even if its filter doesn't match.
     *
     * In the case of an Approver calling HandleWith, we'll also try again
     * even if it already failed - perhaps the Approver is feeling lucky. */
    if (approval != NULL && approval->client_bus_name != NULL)
    {
        McdClientProxy *handler = _mcd_client_registry_lookup (
            self->priv->client_registry, approval->client_bus_name);
        gboolean failed = _mcd_dispatch_operation_get_handler_failed (self,
            approval->client_bus_name);

        DEBUG ("Approved handler is %s (still exists: %c, "
               "already failed: %c)", approval->client_bus_name,
               handler != NULL ? 'Y' : 'N',
               failed ? 'Y' : 'N');

        /* Maybe the handler has exited since we chose it, or maybe we
         * already tried it? Otherwise, it's the right choice. */
        if (handler != NULL &&
            (approval->type == APPROVAL_TYPE_HANDLE_WITH || !failed))
        {
            mcd_dispatch_operation_try_handler (self, handler);
            return TRUE;
        }

        /* If the Handler has disappeared, a HandleWith call should fail,
         * but a request (for which the client_bus_name is merely advisory)
         * can legitimately try more handlers. */
        if (approval->type == APPROVAL_TYPE_HANDLE_WITH)
        {
            GError gone = { TP_ERROR,
                TP_ERROR_NOT_IMPLEMENTED,
                "The requested Handler does not exist" };

            g_queue_pop_head (self->priv->approvals);

            dbus_g_method_return_error (approval->context, &gone);

            approval->context = NULL;
            approval_free (approval);
            return TRUE;
        }
    }

    for (iter = self->priv->possible_handlers;
         iter != NULL && *iter != NULL;
         iter++)
    {
        McdClientProxy *handler = _mcd_client_registry_lookup (
            self->priv->client_registry, *iter);
        gboolean failed = _mcd_dispatch_operation_get_handler_failed
            (self, *iter);

        DEBUG ("Possible handler: %s (still exists: %c, already failed: %c)",
               *iter, handler != NULL ? 'Y' : 'N', failed ? 'Y' : 'N');

        if (handler != NULL && !failed &&
            (is_approved || _mcd_client_proxy_get_bypass_approval (handler)))
        {
            mcd_dispatch_operation_try_handler (self, handler);
            return TRUE;
        }
    }

    return FALSE;
}