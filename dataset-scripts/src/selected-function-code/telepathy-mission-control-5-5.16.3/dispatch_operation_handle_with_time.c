static void
dispatch_operation_handle_with_time (TpSvcChannelDispatchOperation *cdo,
    const gchar *handler_name,
    gint64 user_action_timestamp,
    DBusGMethodInvocation *context)
{
    GError *error = NULL;
    McdDispatchOperation *self = MCD_DISPATCH_OPERATION (cdo);

    DEBUG ("%s/%p", self->priv->unique_name, self);

    if (!mcd_dispatch_operation_check_handle_with (self, handler_name, &error))
    {
        dbus_g_method_return_error (context, error);
        g_error_free (error);
        return;
    }

    self->priv->handle_with_time = user_action_timestamp;

    g_queue_push_tail (self->priv->approvals,
                       approval_new_handle_with (handler_name, context));
    _mcd_dispatch_operation_check_client_locks (self);
}