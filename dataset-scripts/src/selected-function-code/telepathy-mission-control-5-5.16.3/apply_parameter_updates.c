static void
apply_parameter_updates (McdAccount *account,
                         GHashTable *params,
                         const gchar **unset,
                         GHashTable *dbus_properties)
{
    McdAccountPrivate *priv = account->priv;
    GHashTableIter iter;
    gpointer name, value;
    const gchar **unset_iter;

    g_hash_table_iter_init (&iter, params);
    while (g_hash_table_iter_next (&iter, &name, &value))
    {
        _mcd_account_set_parameter (account, name, value);
    }

    for (unset_iter = unset;
         unset_iter != NULL && *unset_iter != NULL;
         unset_iter++)
    {
        _mcd_account_set_parameter (account, *unset_iter, NULL);
    }

    if (mcd_account_get_connection_status (account) ==
        TP_CONNECTION_STATUS_CONNECTED)
    {
        g_hash_table_iter_init (&iter, dbus_properties);
        while (g_hash_table_iter_next (&iter, &name, &value))
        {
            DEBUG ("updating parameter %s", (const gchar *) name);
            _mcd_connection_update_property (priv->connection, name, value);
        }
    }

    mcd_account_check_validity (account,
                                set_parameters_maybe_autoconnect_cb, NULL);
}