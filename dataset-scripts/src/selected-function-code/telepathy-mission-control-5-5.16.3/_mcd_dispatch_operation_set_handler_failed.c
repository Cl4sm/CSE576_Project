static void
_mcd_dispatch_operation_set_handler_failed (McdDispatchOperation *self,
                                            const gchar *bus_name,
                                            const GError *error)
{
    GList *iter, *next;
    gchar **handler;

    if (self->priv->failed_handlers == NULL)
    {
        self->priv->failed_handlers = g_hash_table_new_full (g_str_hash,
                                                             g_str_equal,
                                                             g_free, NULL);
    }

    /* the value is an arbitrary non-NULL pointer - the hash table itself
     * will do nicely */
    g_hash_table_insert (self->priv->failed_handlers, g_strdup (bus_name),
                         self->priv->failed_handlers);

    for (iter = g_queue_peek_head_link (self->priv->approvals);
         iter != NULL;
         iter = next)
    {
        Approval *approval = iter->data;

        /* do this before we potentially free the list element */
        next = iter->next;

        /* If this approval wanted the same handler that just failed, then
         * we can assume that's not going to happen. */
        if (approval->type == APPROVAL_TYPE_HANDLE_WITH &&
            !tp_strdiff (approval->client_bus_name, bus_name))
        {
            dbus_g_method_return_error (approval->context, (GError *) error);
            approval->context = NULL;
            approval_free (approval);
            g_queue_delete_link (self->priv->approvals, iter);
        }
    }

    for (handler = self->priv->possible_handlers;
         handler != NULL && *handler != NULL;
         handler++)
    {
        if (g_hash_table_lookup (self->priv->failed_handlers, *handler)
            == NULL)
        {
            /* we'll try this one soon */
            return;
        }
    }

    DEBUG ("All possible handlers failed: failing with the last error");
    _mcd_dispatch_operation_close_as_undispatchable (self, error);
}