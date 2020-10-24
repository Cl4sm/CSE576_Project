static void
process_ambient_getlevel_call(DBusMessage *req)
{
  DBusMessage *msg;
  DBusMessageIter args;

  int ret;

  logdebug("Got ambient getLevel call\n");

  if (dbus_message_iter_init(req, &args))
    {
      logdebug("ambient getLevel call with arguments ?!\n");

      return;
    }

  msg = dbus_message_new_method_return(req);

  ret = dbus_message_append_args(msg,
				 DBUS_TYPE_UINT32, &ambient_info.left,
				 DBUS_TYPE_UINT32, &ambient_info.right,
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
      logdebug("Could not send ambient getLevel reply\n");

      dbus_message_unref(msg);

      return;
    }

  dbus_message_unref(msg);
}