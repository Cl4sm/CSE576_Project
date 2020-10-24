static SetResult
mcd_account_set_string_val (McdAccount *account,
                            const gchar *key,
                            const GValue *value,
                            McdDBusPropSetFlags flags,
                            GError **error)
{
    McdAccountPrivate *priv = account->priv;
    McdStorage *storage = priv->storage;
    const gchar *name = mcd_account_get_unique_name (account);
    const gchar *new_string;

    if (!G_VALUE_HOLDS_STRING (value))
    {
        g_set_error (error, TP_ERROR, TP_ERROR_INVALID_ARGUMENT,
                     "Expected string for %s, but got %s", key,
                     G_VALUE_TYPE_NAME (value));
        return SET_RESULT_ERROR;
    }

    new_string = g_value_get_string (value);

    if (tp_str_empty (new_string)) {
        new_string = NULL;
    }

    if (flags & MCD_DBUS_PROP_SET_FLAG_ALREADY_IN_STORAGE)
    {
        mcd_account_changed_property (account, key, value);
        return SET_RESULT_CHANGED;
    }
    else if (mcd_storage_set_string (storage, name, key, new_string))
    {
        mcd_storage_commit (storage, name);
        mcd_account_changed_property (account, key, value);
        return SET_RESULT_CHANGED;
    }
    else
    {
        return SET_RESULT_UNCHANGED;
    }
}