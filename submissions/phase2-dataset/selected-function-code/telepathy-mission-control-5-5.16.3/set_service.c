static gboolean
set_service (TpSvcDBusProperties *self, const gchar *name,
             const GValue *value,
             McdDBusPropSetFlags flags,
             GError **error)
{
    McdAccount *account = MCD_ACCOUNT (self);
    SetResult ret = SET_RESULT_ERROR;
    gboolean proceed = TRUE;
    static GRegex *rule = NULL;
    static gsize service_re_init = 0;

    if (g_once_init_enter (&service_re_init))
    {
        GError *regex_error = NULL;
        rule = g_regex_new ("^(?:[a-z][a-z0-9_-]*)?$",
                            G_REGEX_CASELESS|G_REGEX_DOLLAR_ENDONLY,
                            0, &regex_error);
        g_assert_no_error (regex_error);
        g_once_init_leave (&service_re_init, 1);
    }

    if (G_VALUE_HOLDS_STRING (value))
      proceed = g_regex_match (rule, g_value_get_string (value), 0, NULL);

    /* if value is not a string, mcd_account_set_string_val will set *
     * the appropriate error for us: don't duplicate that logic here */
    if (proceed)
    {
        ret = mcd_account_set_string_val (account, MC_ACCOUNTS_KEY_SERVICE,
                                          value, flags, error);
    }
    else
    {
        g_set_error (error, TP_ERROR, TP_ERROR_INVALID_ARGUMENT,
                     "Invalid service '%s': Must consist of ASCII alphanumeric "
                     "characters, underscores (_) and hyphens (-) only, and "
                     "start with a letter",
                     g_value_get_string (value));
    }

    return (ret != SET_RESULT_ERROR);
}