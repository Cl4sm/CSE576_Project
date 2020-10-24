static void
get_storage_identifier (TpSvcDBusProperties *self,
    const gchar *name, GValue *value)
{

  McdAccount *account = MCD_ACCOUNT (self);
  McpAccountStorage *storage_plugin = get_storage_plugin (account);
  GValue identifier = G_VALUE_INIT;

  g_value_init (value, G_TYPE_VALUE);

  if (storage_plugin != NULL)
    {
      mcp_account_storage_get_identifier (
          storage_plugin, account->priv->unique_name, &identifier);
    }
  else
    {
      g_value_init (&identifier, G_TYPE_UINT);

      g_value_set_uint (&identifier, 0);
    }

  g_value_set_boxed (value, &identifier);

  g_value_unset (&identifier);
}