static gboolean
set_nickname (TpSvcDBusProperties *self, const gchar *name,
              const GValue *value,
              McdDBusPropSetFlags flags,
              GError **error)
{
    McdAccount *account = MCD_ACCOUNT (self);
    McdAccountPrivate *priv = account->priv;
    SetResult ret;
    GValue replacement = G_VALUE_INIT;

    DEBUG ("called for %s", priv->unique_name);

    /* If we're asked to set Nickname = "", set it to our identifier
     * (NormalizedName) instead, so that we always have some sort of nickname.
     * This matches what we do when connecting an account.
     *
     * Exception: if we're not fully connected yet (and hence have no
     * self-contact), rely on the corresponding special-case
     * when we do become connected.
     */
    if (G_VALUE_HOLDS_STRING (value) &&
        tp_str_empty (g_value_get_string (value)) &&
        priv->self_contact != NULL)
    {
        g_value_init (&replacement, G_TYPE_STRING);
        g_value_set_string (&replacement,
            tp_contact_get_identifier (priv->self_contact));
        value = &replacement;
    }

    ret = mcd_account_set_string_val (account, MC_ACCOUNTS_KEY_NICKNAME,
                                      value, flags, error);

    if (ret != SET_RESULT_ERROR)
    {
        mcd_account_send_nickname_to_connection (account,
            g_value_get_string (value));
    }

    if (value == &replacement)
        g_value_unset (&replacement);

    return (ret != SET_RESULT_ERROR);
}