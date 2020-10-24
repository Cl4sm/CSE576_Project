void
mbpdbus_send_ambient_light(int l, int l_prev, int r, int r_prev)
{
  DBusMessage *msg;

  int ret;

  if (conn == NULL)
    return;

  logdebug("DBus ambientLight: %d %d %d %d\n", l, l_prev, r, r_prev);

  msg = dbus_message_new_signal("/org/pommed/notify/ambientLight",
				"org.pommed.signal.ambientLight",
				"ambientLight");
  if (msg == NULL)
    {
      logdebug("Failed to create DBus message\n");

      return;
    }

  ret = dbus_message_append_args(msg,
				 DBUS_TYPE_UINT32, &l,
				 DBUS_TYPE_UINT32, &l_prev,
				 DBUS_TYPE_UINT32, &r,
				 DBUS_TYPE_UINT32, &r_prev,
				 DBUS_TYPE_UINT32, &ambient_info.max,
				 DBUS_TYPE_INVALID);
  if (ret == FALSE)
    {
      logdebug("Failed to add arguments\n");

      dbus_message_unref(msg);

      return;
    }

  ret = dbus_connection_send(conn, msg, NULL);
  if (ret == FALSE)
    {
      logdebug("Could not send kbdBacklight signal\n");

      dbus_message_unref(msg);

      return;
    }

  dbus_connection_flush(conn);

  dbus_message_unref(msg);
}