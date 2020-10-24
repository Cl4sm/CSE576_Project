static DBusHandlerResult
mcd_client_registry_name_owner_filter (DBusConnection *conn,
    DBusMessage *msg,
    gpointer data)
{
  McdClientRegistry *self = MCD_CLIENT_REGISTRY (data);

  /* make sure this is the right kind of signal: */
  if (dbus_message_is_signal (msg, DBUS_INTERFACE_DBUS, "NameOwnerChanged"))
    {
      const gchar *dbus_name = NULL;
      const gchar *old_owner = NULL;
      const gchar *new_owner = NULL;
      gboolean ok = dbus_message_get_args (msg, NULL,
          DBUS_TYPE_STRING, &dbus_name,
          DBUS_TYPE_STRING, &old_owner,
          DBUS_TYPE_STRING, &new_owner,
          DBUS_TYPE_INVALID);

      /* could not unpack args -> invalid -> stop processing right here */
      if (!ok)
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;

      if (tp_str_empty (old_owner) && !tp_str_empty (new_owner))
        _mcd_client_registry_found_name (self, dbus_name, new_owner, FALSE);
    }

  /* in case somebody else is also interested */
  return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}