static gboolean
set_requested_presence (TpSvcDBusProperties *self,
                        const gchar *name,
                        const GValue *value,
                        McdDBusPropSetFlags flags,
                        GError **error)
{
    McdAccount *account = MCD_ACCOUNT (self);
    McdAccountPrivate *priv = account->priv;
    const gchar *status, *message;
    gint type;
    GValueArray *va;

    DEBUG ("called for %s", priv->unique_name);

    if (!G_VALUE_HOLDS (value, TP_STRUCT_TYPE_SIMPLE_PRESENCE))
    {
        g_set_error (error, TP_ERROR, TP_ERROR_INVALID_ARGUMENT,
                     "Unexpected type for RequestedPresence: wanted (u,s,s), "
                     "got %s", G_VALUE_TYPE_NAME (value));
        return FALSE;
    }

    va = g_value_get_boxed (value);
    type = (gint)g_value_get_uint (va->values);
    status = g_value_get_string (va->values + 1);
    message = g_value_get_string (va->values + 2);

    if (priv->always_on && !_presence_type_is_online (type))
    {
        g_set_error (error, TP_ERROR, TP_ERROR_PERMISSION_DENIED,
                     "Account %s cannot be taken offline", priv->unique_name);
        return FALSE;
    }

    if (!_mcd_account_presence_type_is_settable (type))
    {
        g_set_error (error, TP_ERROR, TP_ERROR_INVALID_ARGUMENT,
                     "RequestedPresence %d cannot be set on yourself", type);
        return FALSE;
    }

    DEBUG ("setting requested presence: %d, %s, %s", type, status, message);

    mcd_account_request_presence_int (account, type, status, message, TRUE);
    return TRUE;
}