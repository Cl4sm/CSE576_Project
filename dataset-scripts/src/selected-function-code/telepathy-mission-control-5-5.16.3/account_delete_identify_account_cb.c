static void
account_delete_identify_account_cb (TpProxy *protocol,
    const char *account_id,
    const GError *in_error,
    gpointer user_data,
    GObject *self)
{
  McdAccount *account = MCD_ACCOUNT (self);
  TpConnectionManager *cm = mcd_account_get_cm (account);

  if (in_error != NULL)
    {
      DEBUG ("Error identifying account: %s", in_error->message);
    }
  else
    {
      DEBUG ("Identified account as %s", account_id);

      mc_cli_connection_manager_interface_account_storage_call_remove_account (
          cm, -1, account_id,
          NULL, NULL, NULL, NULL);
    }

  g_object_unref (account);
}