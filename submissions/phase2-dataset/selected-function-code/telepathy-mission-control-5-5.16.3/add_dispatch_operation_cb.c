static void
add_dispatch_operation_cb (TpClient *proxy,
                           const GError *error,
                           gpointer user_data,
                           GObject *weak_object)
{
    McdDispatchOperation *self = user_data;

    if (error)
    {
        DEBUG ("AddDispatchOperation %s (%p) on approver %s failed: "
               "%s",
               _mcd_dispatch_operation_get_path (self), self,
               tp_proxy_get_object_path (proxy), error->message);
    }
    else
    {
        DEBUG ("Approver %s accepted AddDispatchOperation %s (%p)",
               tp_proxy_get_object_path (proxy),
               _mcd_dispatch_operation_get_path (self), self);

        if (!self->priv->accepted_by_an_approver)
        {
            self->priv->accepted_by_an_approver = TRUE;
        }
    }

    /* If all approvers fail to add the DO, then we behave as if no
     * approver was registered: i.e., we continue dispatching. If at least
     * one approver accepted it, then we can still continue dispatching,
     * since it will be stalled until an approval is received. */
    _mcd_dispatch_operation_dec_ado_pending (self);
}