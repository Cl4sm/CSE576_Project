static gboolean
set_supersedes (TpSvcDBusProperties *svc,
    const gchar *name,
    const GValue *value,
    McdDBusPropSetFlags flags,
    GError **error)
{
  McdAccount *self = MCD_ACCOUNT (svc);

  if (!G_VALUE_HOLDS (value, TP_ARRAY_TYPE_OBJECT_PATH_LIST))
    {
      g_set_error (error, TP_ERROR, TP_ERROR_INVALID_ARGUMENT,
          "Unexpected type for Supersedes: wanted 'ao', got %s",
          G_VALUE_TYPE_NAME (value));
      return FALSE;
    }

  if (self->priv->supersedes != NULL)
    g_ptr_array_unref (self->priv->supersedes);

  self->priv->supersedes = g_value_dup_boxed (value);
  mcd_account_changed_property (self, name, value);

  mcd_storage_set_attribute (self->priv->storage, self->priv->unique_name,
      MC_ACCOUNTS_KEY_SUPERSEDES, value);
  mcd_storage_commit (self->priv->storage, self->priv->unique_name);

  return TRUE;
}