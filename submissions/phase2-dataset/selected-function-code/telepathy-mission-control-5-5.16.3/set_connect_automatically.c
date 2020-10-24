static gboolean
set_connect_automatically (TpSvcDBusProperties *self,
                           const gchar *name,
                           const GValue *value,
                           McdDBusPropSetFlags flags,
                           GError **error)
{
    McdAccount *account = MCD_ACCOUNT (self);
    McdAccountPrivate *priv = account->priv;
    gboolean connect_automatically;

    DEBUG ("called for %s", priv->unique_name);

    if (!G_VALUE_HOLDS_BOOLEAN (value))
    {
        g_set_error (error, TP_ERROR, TP_ERROR_INVALID_ARGUMENT,
                     "Expected boolean for ConnectAutomatically, but got %s",
                     G_VALUE_TYPE_NAME (value));
        return FALSE;
    }

    connect_automatically = g_value_get_boolean (value);

    if (priv->always_on && !connect_automatically)
    {
        g_set_error (error, TP_ERROR, TP_ERROR_PERMISSION_DENIED,
                     "Account %s always connects automatically",
                     priv->unique_name);
        return FALSE;
    }

    if (priv->connect_automatically != connect_automatically)
    {
        const gchar *account_name = mcd_account_get_unique_name (account);

        if (!(flags & MCD_DBUS_PROP_SET_FLAG_ALREADY_IN_STORAGE))
        {
            mcd_storage_set_attribute (priv->storage, account_name,
                                       MC_ACCOUNTS_KEY_CONNECT_AUTOMATICALLY,
                                       value);
            mcd_storage_commit (priv->storage, account_name);
        }

        priv->connect_automatically = connect_automatically;
        mcd_account_changed_property (account, name, value);

        if (connect_automatically)
        {
            _mcd_account_maybe_autoconnect (account);
        }
    }

    return TRUE;
}