void
mcd_account_delete (McdAccount *account,
                     McdAccountDeleteCb callback,
                     gpointer user_data)
{
    McdAccountPrivate *priv = account->priv;
    gchar *data_dir_str;
    GError *error = NULL;
    const gchar *name = mcd_account_get_unique_name (account);
    TpConnectionManager *cm = mcd_account_get_cm (account);

    /* if the CM implements CM.I.AccountStorage, we need to tell the CM
     * to forget any account credentials it knows */
    if (tp_proxy_has_interface_by_id (cm,
            MC_IFACE_QUARK_CONNECTION_MANAGER_INTERFACE_ACCOUNT_STORAGE))
    {
        TpProtocol *protocol;
        GHashTable *params;

        /* identify the account */
        protocol = tp_connection_manager_get_protocol_object (cm,
            account->priv->protocol_name);
        params = _mcd_account_dup_parameters (account);

        tp_cli_protocol_call_identify_account (protocol, -1, params,
            account_delete_identify_account_cb,
            NULL, NULL, g_object_ref (account));

        g_hash_table_unref (params);
    }

    /* got to turn the account off before removing it, otherwise we can *
     * end up with an orphaned CM holding the account online            */
    if (!_mcd_account_set_enabled (account, FALSE, FALSE,
                                   MCD_DBUS_PROP_SET_FLAG_NONE, &error))
    {
        g_warning ("could not disable account %s (%s)", name, error->message);
        callback (account, error, user_data);
        g_error_free (error);
        return;
    }

    mcd_storage_delete_account (priv->storage, name);

    data_dir_str = get_old_account_data_path (priv);

    if (data_dir_str != NULL)
    {
        GDir *data_dir = g_dir_open (data_dir_str, 0, NULL);

        if (data_dir)
        {
            const gchar *filename;

            while ((filename = g_dir_read_name (data_dir)) != NULL)
            {
                gchar *path = g_build_filename (data_dir_str, filename, NULL);

                g_remove (path);
                g_free (path);
            }

            g_dir_close (data_dir);
            g_rmdir (data_dir_str);
        }

        g_free (data_dir_str);
    }

    mcd_storage_commit (priv->storage, name);

    /* The callback may drop the latest ref on @account so make sure it stays
     * alive while we still need it. */
    g_object_ref (account);

    if (callback != NULL)
        callback (account, NULL, user_data);

    /* If the account was not removed via the DBus Account interface code     *
     * path and something is holding a ref to it so it does not get disposed, *
     * then this signal may not get fired, so we make sure it _does_ here     */
    if (!priv->removed)
    {
        DEBUG ("Forcing Account.Removed for %s", name);
        priv->removed = TRUE;
        tp_svc_account_emit_removed (account);
    }

    unregister_dbus_service (account);
    g_object_unref (account);
}