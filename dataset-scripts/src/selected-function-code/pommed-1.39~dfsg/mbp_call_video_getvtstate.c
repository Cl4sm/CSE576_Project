int
mbp_call_video_getvtstate(int vtnum, DBusPendingCallNotifyFunction cb, void *userdata)
{
  DBusMessage *msg;
  DBusPendingCall *pending;

  int ret;

  msg = dbus_message_new_method_call("org.pommed", "/org/pommed/video",
				     "org.pommed.video", "getVTState");

  if (msg == NULL)
    {
      printf("Failed to create method call message\n");

      return -1;
    }

  ret = dbus_message_append_args(msg,
				 DBUS_TYPE_UINT32, &vtnum,
				 DBUS_TYPE_INVALID);
  if (ret == FALSE)
    {
      printf("Failed to add arguments\n");

      dbus_message_unref(msg);

      return -1;
    }

  ret = dbus_connection_send_with_reply(conn, msg, &pending, 250);
  if (ret == FALSE)
    {
      printf("Could not send method call\n");

      dbus_message_unref(msg);

      return -1;
    }

  dbus_connection_flush(conn);

  dbus_message_unref(msg);

  dbus_pending_call_block(pending);

  cb(pending, userdata);

  return 0;
}