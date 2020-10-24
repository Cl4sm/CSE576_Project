static void
process_cd_eject_call(DBusMessage *req)
{
  DBusMessage *msg;

  int ret;

  logdebug("Got cd eject call\n");

  cd_eject();

  msg = dbus_message_new_method_return(req);

  ret = dbus_connection_send(conn, msg, NULL);
  if (ret == FALSE)
    {
      logdebug("Could not send cd eject reply\n");

      dbus_message_unref(msg);

      return;
    }

  dbus_message_unref(msg);
}