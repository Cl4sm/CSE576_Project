static void on_manager_ready (McdManager *manager, const GError *error,
                              gpointer user_data)
{
    McdAccount *account = MCD_ACCOUNT (user_data);

    if (error)
    {
        DEBUG ("got error: %s", error->message);
        mcd_account_loaded (account);
    }
    else
    {
        TpConnectionManager *cm = mcd_manager_get_tp_proxy (manager);

        mcd_account_check_parameters (account, manager_ready_check_params_cb,
                                      NULL);

        /* determine if we support Acct.I.ExternalPasswordStorage */
        if (tp_proxy_has_interface_by_id (cm,
                MC_IFACE_QUARK_CONNECTION_MANAGER_INTERFACE_ACCOUNT_STORAGE))
        {
            TpProtocol *protocol = tp_connection_manager_get_protocol_object (
                cm, account->priv->protocol_name);
            GHashTable *params;

            DEBUG ("CM %s has CM.I.AccountStorage iface",
                   mcd_manager_get_name (manager));

            mcd_dbus_activate_optional_interface (
                TP_SVC_DBUS_PROPERTIES (account),
                MC_TYPE_SVC_ACCOUNT_INTERFACE_EXTERNAL_PASSWORD_STORAGE);

            /* look up account identity so we can look up our value in
             * the Accounts map */
            params = _mcd_account_dup_parameters (account);
            tp_cli_protocol_call_identify_account (protocol, -1, params,
                account_setup_identify_account_cb,
                NULL, NULL, G_OBJECT (account));

            tp_cli_dbus_properties_connect_to_properties_changed (cm,
                account_external_password_storage_properties_changed_cb,
                NULL, NULL, G_OBJECT (account), NULL);

            g_hash_table_unref (params);
        }
    }
}