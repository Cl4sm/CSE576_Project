void
mbpdbus_send_video_switch(void)
{
  DBusMessage *msg;

  int ret;

  if (conn == NULL)
    return;

  logdebug("DBus video switch\n");

  msg = dbus_message_new_signal("/org/pommed/notify/videoSwitch",
				"org.pommed.signal.videoSwitch",
				"videoSwitch");
  if (msg == NULL)
    {
      logdebug("Failed to create DBus message\n");

      return;
    }

  ret = dbus_connection_send(conn, msg, NULL);
  if (ret == FALSE)
    {
      logdebug("Could not send videoSwitch signal\n");

      dbus_message_unref(msg);

      return;
    }

  dbus_connection_flush(conn);

  dbus_message_unref(msg);
}