void
mbpdbus_send_audio_volume(int cur, int prev)
{
  DBusMessage *msg;

  int ret;

  if (conn == NULL)
    return;

  logdebug("DBus audioVolume: %d %d\n", cur, prev);

  msg = dbus_message_new_signal("/org/pommed/notify/audioVolume",
				"org.pommed.signal.audioVolume",
				"audioVolume");
  if (msg == NULL)
    {
      logdebug("Failed to create DBus message\n");

      return;
    }

  ret = dbus_message_append_args(msg,
				 DBUS_TYPE_UINT32, &cur,
				 DBUS_TYPE_UINT32, &prev,
				 DBUS_TYPE_UINT32, &audio_info.max,
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
      logdebug("Could not send audioVolume signal\n");

      dbus_message_unref(msg);

      return;
    }

  dbus_connection_flush(conn);

  dbus_message_unref(msg);
}