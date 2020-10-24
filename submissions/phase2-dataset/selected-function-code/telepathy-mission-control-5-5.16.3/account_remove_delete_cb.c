static void
account_remove_delete_cb (McdAccount *account, const GError *error,
                          gpointer user_data)
{
    RemoveMethodData *data = (RemoveMethodData *) user_data;

    if (error != NULL)
    {
        dbus_g_method_return_error (data->context, (GError *) error);
        return;
    }

    if (!data->self->priv->removed)
    {
        data->self->priv->removed = TRUE;
        tp_svc_account_emit_removed (data->self);
    }

    tp_svc_account_return_from_remove (data->context);

    g_slice_free (RemoveMethodData, data);
}