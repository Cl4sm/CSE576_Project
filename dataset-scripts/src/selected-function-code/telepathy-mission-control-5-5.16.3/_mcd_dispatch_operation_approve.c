void
_mcd_dispatch_operation_approve (McdDispatchOperation *self,
                                 const gchar *preferred_handler)
{
    g_return_if_fail (MCD_IS_DISPATCH_OPERATION (self));

    /* NULL-safety: treat both NULL and "" as "unspecified" */
    if (preferred_handler == NULL)
        preferred_handler = "";

    DEBUG ("%s/%p (preferred handler: '%s')", self->priv->unique_name, self,
           preferred_handler);

    if (!g_str_has_prefix (preferred_handler, TP_CLIENT_BUS_NAME_BASE) ||
        !tp_dbus_check_valid_bus_name (preferred_handler,
                                       TP_DBUS_NAME_TYPE_WELL_KNOWN, NULL))
    {
        DEBUG ("preferred handler name '%s' is bad, treating as unspecified",
               preferred_handler);
        preferred_handler = "";
    }

    g_queue_push_tail (self->priv->approvals,
                       approval_new_requested (preferred_handler));

    _mcd_dispatch_operation_check_client_locks (self);
}