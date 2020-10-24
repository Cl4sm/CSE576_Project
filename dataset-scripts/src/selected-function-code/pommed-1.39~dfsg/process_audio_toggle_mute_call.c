static void
process_audio_toggle_mute_call(DBusMessage *req)
{
  DBusMessage *msg;

  int ret;

  logdebug("Got audio toggleMute call\n");

  audio_toggle_mute();

  msg = dbus_message_new_method_return(req);

  ret = dbus_connection_send(conn, msg, NULL);
  if (ret == FALSE)
    {
      logdebug("Could not send audio toggleMute reply\n");

      dbus_message_unref(msg);

      return;
    }

  dbus_message_unref(msg);
}