gboolean
mcd_account_get_parameter_of_known_type (McdAccount *account,
                                         const gchar *name,
                                         GType type,
                                         GValue *parameter,
                                         GError **error)
{
    const gchar *account_name = mcd_account_get_unique_name (account);
    McdStorage *storage = account->priv->storage;
    GValue tmp = G_VALUE_INIT;

    g_value_init (&tmp, type);

    if (mcd_storage_get_parameter (storage, account_name, name, &tmp, error))
    {
        if (parameter != NULL)
        {
            memcpy (parameter, &tmp, sizeof (tmp));
        }

        return TRUE;
    }

    return FALSE;
}