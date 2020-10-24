static gboolean
set_storage_provider (TpSvcDBusProperties *self,
    const gchar *name,
    const GValue *value,
    McdDBusPropSetFlags flags,
    GError **error)
{
  McdAccount *account = MCD_ACCOUNT (self);
  McpAccountStorage *storage_plugin = get_storage_plugin (account);
  const gchar *current_provider = mcp_account_storage_provider (storage_plugin);

  if (!G_VALUE_HOLDS_STRING (value) ||
      tp_strdiff (g_value_get_string (value), current_provider))
    {
      g_set_error (error, TP_ERROR, TP_ERROR_INVALID_ARGUMENT,
          "Cannot change provider, it is defined at account creation only");
      return FALSE;
    }

  return TRUE;
}