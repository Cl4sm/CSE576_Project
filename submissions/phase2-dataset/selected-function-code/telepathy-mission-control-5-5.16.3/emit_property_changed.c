static gboolean
emit_property_changed (gpointer userdata)
{
    McdAccount *account = MCD_ACCOUNT (userdata);
    McdAccountPrivate *priv = account->priv;

    DEBUG ("called");

    if (g_hash_table_size (priv->changed_properties) > 0)
    {
        tp_svc_account_emit_account_property_changed (account,
            priv->changed_properties);
        g_hash_table_remove_all (priv->changed_properties);
    }

    if (priv->properties_source != 0)
    {
      g_source_remove (priv->properties_source);
      priv->properties_source = 0;
    }
    return FALSE;
}