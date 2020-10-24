static gboolean
set_enabled (TpSvcDBusProperties *self,
             const gchar *name,
             const GValue *value,
             McdDBusPropSetFlags flags,
             GError **error)
{
    McdAccount *account = MCD_ACCOUNT (self);
    McdAccountPrivate *priv = account->priv;
    gboolean enabled;

    DEBUG ("called for %s", priv->unique_name);

    if (!G_VALUE_HOLDS_BOOLEAN (value))
    {
        g_set_error (error, TP_ERROR, TP_ERROR_INVALID_ARGUMENT,
                     "Expected boolean for Enabled, but got %s",
                     G_VALUE_TYPE_NAME (value));
        return FALSE;
    }

    enabled = g_value_get_boolean (value);

    return _mcd_account_set_enabled (account, enabled, TRUE, flags, error);
}