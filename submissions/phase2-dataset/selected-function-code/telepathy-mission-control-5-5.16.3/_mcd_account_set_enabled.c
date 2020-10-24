gboolean
_mcd_account_set_enabled (McdAccount *account,
                          gboolean enabled,
                          gboolean write_out,
                          McdDBusPropSetFlags flags,
                          GError **error)
{
    McdAccountPrivate *priv = account->priv;

    if (priv->always_on && !enabled)
    {
        g_set_error (error, TP_ERROR, TP_ERROR_PERMISSION_DENIED,
                     "Account %s cannot be disabled",
                     priv->unique_name);
        return FALSE;
    }

    if (priv->enabled != enabled)
    {
        GValue value = G_VALUE_INIT;
        const gchar *name = mcd_account_get_unique_name (account);

        if (!enabled && priv->connection != NULL)
            _mcd_connection_request_presence (priv->connection,
                                              TP_CONNECTION_PRESENCE_TYPE_OFFLINE,
                                              "offline",
                                              NULL);

        priv->enabled = enabled;

        g_value_init (&value, G_TYPE_BOOLEAN);
        g_value_set_boolean (&value, enabled);

        if (!(flags & MCD_DBUS_PROP_SET_FLAG_ALREADY_IN_STORAGE))
        {
            mcd_storage_set_attribute (priv->storage, name,
                                       MC_ACCOUNTS_KEY_ENABLED, &value);

            if (write_out)
                mcd_storage_commit (priv->storage, name);
        }

        mcd_account_changed_property (account, "Enabled", &value);

        g_value_unset (&value);

        if (enabled)
        {
            mcd_account_rerequest_presence (account, TRUE);
            _mcd_account_maybe_autoconnect (account);
        }
    }

    return TRUE;
}