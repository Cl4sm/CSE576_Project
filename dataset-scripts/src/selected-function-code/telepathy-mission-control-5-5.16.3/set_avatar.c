static gboolean
set_avatar (TpSvcDBusProperties *self, const gchar *name, const GValue *value,
            McdDBusPropSetFlags flags,
            GError **error)
{
    McdAccount *account = MCD_ACCOUNT (self);
    McdAccountPrivate *priv = account->priv;
    const gchar *mime_type;
    const GArray *avatar;
    GValueArray *va;

    DEBUG ("called for %s", priv->unique_name);

    if (!G_VALUE_HOLDS (value, TP_STRUCT_TYPE_AVATAR))
    {
        g_set_error (error, TP_ERROR, TP_ERROR_INVALID_ARGUMENT,
                     "Unexpected type for Avatar: wanted (ay,s), got %s",
                     G_VALUE_TYPE_NAME (value));
        return FALSE;
    }

    va = g_value_get_boxed (value);
    avatar = g_value_get_boxed (va->values);
    mime_type = g_value_get_string (va->values + 1);

    if (!_mcd_account_set_avatar (account, avatar, mime_type, NULL, error))
    {
        return FALSE;
    }

    tp_svc_account_interface_avatar_emit_avatar_changed (account);
    return TRUE;
}