static gboolean
set_automatic_presence (TpSvcDBusProperties *self,
                        const gchar *name,
                        const GValue *value,
                        McdDBusPropSetFlags flags,
                        GError **error)
{
    McdAccount *account = MCD_ACCOUNT (self);
    McdAccountPrivate *priv = account->priv;
    const gchar *status, *message;
    TpConnectionPresenceType type;
    gboolean changed = FALSE;
    GValueArray *va;
    const gchar *account_name = mcd_account_get_unique_name (account);

    DEBUG ("called for %s", account_name);

    if (!G_VALUE_HOLDS (value, TP_STRUCT_TYPE_SIMPLE_PRESENCE))
    {
        g_set_error (error, TP_ERROR, TP_ERROR_INVALID_ARGUMENT,
                     "Unexpected type for AutomaticPresence: wanted (u,s,s), "
                     "got %s", G_VALUE_TYPE_NAME (value));
        return FALSE;
    }

    va = g_value_get_boxed (value);
    type = g_value_get_uint (va->values);
    status = g_value_get_string (va->values + 1);
    message = g_value_get_string (va->values + 2);

    if (!_presence_type_is_online (type))
    {
        g_set_error (error, TP_ERROR, TP_ERROR_INVALID_ARGUMENT,
                     "AutomaticPresence must be an online presence, not %d",
                     type);
        return FALSE;
    }

    DEBUG ("setting automatic presence: %d, %s, %s", type, status, message);

    if (priv->auto_presence_type != type)
    {
        priv->auto_presence_type = type;
        changed = TRUE;
    }

    if (tp_strdiff (priv->auto_presence_status, status))
    {
        g_free (priv->auto_presence_status);
        priv->auto_presence_status = g_strdup (status);
        changed = TRUE;
    }

    if (tp_strdiff (priv->auto_presence_message, message))
    {
        g_free (priv->auto_presence_message);
        priv->auto_presence_message = g_strdup (message);
        changed = TRUE;
    }

    if (changed)
    {
        mcd_storage_set_attribute (priv->storage, account_name,
                                   MC_ACCOUNTS_KEY_AUTOMATIC_PRESENCE,
                                   value);
        mcd_storage_commit (priv->storage, account_name);
        mcd_account_changed_property (account, name, value);
    }

    return TRUE;
}