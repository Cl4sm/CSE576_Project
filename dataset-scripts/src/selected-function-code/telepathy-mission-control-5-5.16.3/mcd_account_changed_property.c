static void
mcd_account_changed_property (McdAccount *account, const gchar *key,
			      const GValue *value)
{
    McdAccountPrivate *priv = account->priv;

    DEBUG ("called: %s", key);
    if (priv->changed_properties &&
	g_hash_table_lookup (priv->changed_properties, key))
    {
	/* the changed property was also changed before; then let's force the
	 * emission of the signal now, so that the property will appear in two
	 * separate signals */
        DEBUG ("Forcibly emit PropertiesChanged now");
	emit_property_changed (account);
    }

    if (priv->properties_source == 0)
    {
        DEBUG ("First changed property");
        priv->properties_source = g_timeout_add_full (G_PRIORITY_DEFAULT, 10,
                                                      emit_property_changed,
                                                      g_object_ref (account),
                                                      g_object_unref);
    }
    g_hash_table_insert (priv->changed_properties, (gpointer) key,
                         tp_g_value_slice_dup (value));
}