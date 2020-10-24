void
mbpdbus_send_kbd_backlight(int cur, int prev, int who)
{
  DBusMessage *msg;

  int ret;

  if (conn == NULL)
    return;

  logdebug("DBus kbdBacklight: %d %d\n", cur, prev);

  msg = dbus_message_new_signal("/org/pommed/notify/kbdBacklight",
				"org.pommed.signal.kbdBacklight",
				"kbdBacklight");
  if (msg == NULL)
    {
      logdebug("Failed to create DBus message\n");

      return;
    }

  ret = dbus_message_append_args(msg,
				 DBUS_TYPE_UINT32, &cur,
				 DBUS_TYPE_UINT32, &prev,
				 DBUS_TYPE_UINT32, &kbd_bck_info.max,
				 DBUS_TYPE_UINT32, &who,
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