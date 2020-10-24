static void
process_audio_getvolume_call(DBusMessage *req)
{
  DBusMessage *msg;
  DBusMessageIter args;

  int ret;

  logdebug("Got audio getVolume call\n");

  if (dbus_message_iter_init(req, &args))
    {
      logdebug("audio getVolume call with arguments ?!\n");

      return;
    }

  msg = dbus_message_new_method_return(req);

  ret = dbus_message_append_args(msg,
				 DBUS_TYPE_UINT32, &audio_info.level,
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
      logdebug("Could not send audio getVolume reply\n");

      dbus_message_unref(msg);

      return;
    }

  dbus_message_unref(msg);
}