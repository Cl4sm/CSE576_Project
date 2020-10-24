static gboolean
mcd_dispatch_operation_check_handle_with (McdDispatchOperation *self,
                                          const gchar *handler_name,
                                          GError **error)
{
    g_return_val_if_fail (MCD_IS_DISPATCH_OPERATION (self), FALSE);

    if (self->priv->result != NULL)
    {
        DEBUG ("already finished, %s", self->priv->result->message);
        if (error != NULL)
            *error = g_error_copy (self->priv->result);
        return FALSE;
    }

    if (!g_queue_is_empty (self->priv->approvals))
    {
        DEBUG ("NotYours: already finished or approved");
        g_set_error (error, TP_ERROR, TP_ERROR_NOT_YOURS,
                     "CDO already finished or approved");
        return FALSE;
    }

    if (handler_name == NULL || handler_name[0] == '\0')
    {
        /* no handler name given */
        return TRUE;
    }

    if (!g_str_has_prefix (handler_name, TP_CLIENT_BUS_NAME_BASE) ||
        !tp_dbus_check_valid_bus_name (handler_name,
                                       TP_DBUS_NAME_TYPE_WELL_KNOWN, NULL))
    {
        DEBUG ("InvalidArgument: handler name %s is bad", handler_name);
        g_set_error (error, TP_ERROR, TP_ERROR_INVALID_ARGUMENT,
                     "Invalid handler name");
        return FALSE;
    }

    return TRUE;
}