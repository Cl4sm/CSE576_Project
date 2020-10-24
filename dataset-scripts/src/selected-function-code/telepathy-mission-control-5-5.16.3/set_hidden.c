static gboolean
set_hidden (TpSvcDBusProperties *self,
    const gchar *name,
    const GValue *value,
    McdDBusPropSetFlags flags,
    GError **error)
{
  McdAccount *account = MCD_ACCOUNT (self);
  McdAccountPrivate *priv = account->priv;
  const gchar *account_name = mcd_account_get_unique_name (account);

  if (!G_VALUE_HOLDS_BOOLEAN (value))
    {
      g_set_error (error, TP_ERROR, TP_ERROR_INVALID_ARGUMENT,
          "Hidden must be set to a boolean, not a %s",
          G_VALUE_TYPE_NAME (value));
      return FALSE;
    }

  /* Technically this property is immutable after the account's been created,
   * but currently it's not easy for this code to tell whether or not this is
   * a create-time property. It would probably be better if the create-time
   * properties were passed into us as a construct-time GObject property. But
   * that's a job for another month.
   *
   * So for now we check whether the value has changed, and violate the spec
   * by making this property mutable (at least with the keyfile backend).
   */
  if (mcd_storage_set_attribute (priv->storage, account_name,
          MC_ACCOUNTS_KEY_HIDDEN, value))
    {
      mcd_storage_commit (priv->storage, account_name);
      mcd_account_changed_property (account, MC_ACCOUNTS_KEY_HIDDEN, value);
      g_object_set_property (G_OBJECT (self), "hidden", value);
    }

  return TRUE;
}