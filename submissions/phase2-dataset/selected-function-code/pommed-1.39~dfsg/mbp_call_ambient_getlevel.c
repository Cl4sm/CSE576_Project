int
mbp_call_ambient_getlevel(DBusPendingCallNotifyFunction cb, void *userdata)
{
  DBusMessage *msg;
  DBusPendingCall *pending;

  int ret;

  msg = dbus_message_new_method_call("org.pommed", "/org/pommed/ambient",
				     "org.pommed.ambient", "getLevel");

  if (msg == NULL)
    {
      printf("Failed to create method call message\n");

      return -1;
    }

  ret = dbus_connection_send_with_reply(conn, msg, &pending, 250);
  if (ret == FALSE)
    {
      printf("Could not send method call\n");

      dbus_message_unref(msg);

      return -1;
    }

#if 0 /* Needs more work, using dispatch & stuff */
  ret = dbus_pending_call_set_notify(pending, cb, NULL, NULL);
  if (!ret)
    {
      printf("Failed to set callback\n");

      dbus_pending_call_unref(pending);

      return -1;
    }
#endif /* 0 */

  dbus_connection_flush(conn);

  dbus_message_unref(msg);

  dbus_pending_call_block(pending);

  cb(pending, userdata);

  return 0;
}