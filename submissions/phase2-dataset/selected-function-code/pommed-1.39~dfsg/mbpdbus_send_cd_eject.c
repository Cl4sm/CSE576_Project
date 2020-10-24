void
mbpdbus_send_cd_eject(void)
{
  DBusMessage *msg;

  int ret;

  if (conn == NULL)
    return;

  logdebug("DBus CD eject\n");

  msg = dbus_message_new_signal("/org/pommed/notify/cdEject",
				"org.pommed.signal.cdEject",
				"cdEject");
  if (msg == NULL)
    {
      logdebug("Failed to create DBus message\n");

      return;
    }

  ret = dbus_connection_send(conn, msg, NULL);
  if (ret == FALSE)
    {
      logdebug("Could not send cdEject signal\n");

      dbus_message_unref(msg);

      return;
    }

  dbus_connection_flush(conn);

  dbus_message_unref(msg);
}