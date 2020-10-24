static void
process_audio_volume_step_call(DBusMessage *req, int dir)
{
  DBusMessage *msg;

  int ret;

  logdebug("Got audio volumeUp/volumeDown call\n");

  audio_step(dir);

  msg = dbus_message_new_method_return(req);

  ret = dbus_connection_send(conn, msg, NULL);
  if (ret == FALSE)
    {
      logdebug("Could not send audio volumeUp/volumeDown reply\n");

      dbus_message_unref(msg);

      return;
    }

  dbus_message_unref(msg);
}