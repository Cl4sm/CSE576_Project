static void
_mcd_dispatch_operation_dec_ado_pending (McdDispatchOperation *self)
{
    DEBUG ("%" G_GSIZE_FORMAT " -> %" G_GSIZE_FORMAT,
           self->priv->ado_pending,
           self->priv->ado_pending - 1);
    g_return_if_fail (self->priv->ado_pending > 0);
    self->priv->ado_pending--;

    _mcd_dispatch_operation_check_finished (self);

    if (self->priv->ado_pending == 0 && !self->priv->accepted_by_an_approver)
    {
        DEBUG ("No approver accepted the channel; considering it to be "
               "approved");
        g_queue_push_tail (self->priv->approvals,
                           approval_new (APPROVAL_TYPE_NO_APPROVERS));
    }

    _mcd_dispatch_operation_check_client_locks (self);

    g_object_unref (self);
}