static void
mcd_account_loaded (McdAccount *account)
{
    g_return_if_fail (!account->priv->loaded);
    account->priv->loaded = TRUE;

    /* invoke all the callbacks */
    g_object_ref (account);

    _mcd_object_ready (account, account_ready_quark, NULL);

    if (account->priv->online_requests != NULL)
    {
        /* if we have established that the account is not valid or is
         * disabled, cancel all requests */
        if (!mcd_account_is_valid (account) || !account->priv->enabled)
        {
            /* FIXME: pick better errors and put them in telepathy-spec? */
            GError e = { TP_ERROR, TP_ERROR_NOT_AVAILABLE,
                "account isn't Valid (not enough information to put it "
                    "online)" };
            GList *list;

            if (mcd_account_is_valid (account))
            {
                e.message = "account isn't Enabled";
            }

            list = account->priv->online_requests;
            account->priv->online_requests = NULL;

            for (/* already initialized */ ;
                 list != NULL;
                 list = g_list_delete_link (list, list))
            {
                McdOnlineRequestData *data = list->data;

                data->callback (account, data->user_data, &e);
                g_slice_free (McdOnlineRequestData, data);
            }
        }

        /* otherwise, we want to go online now */
        if (account->priv->conn_status == TP_CONNECTION_STATUS_DISCONNECTED)
        {
            _mcd_account_connect_with_auto_presence (account, TRUE);
        }
    }

    _mcd_account_maybe_autoconnect (account);

    g_object_unref (account);
}