static void
process_video_getvtstate_call(DBusMessage *req)
{
  DBusMessage *msg;

  int vtnum;
  int vtstate;
  int ret;

  logdebug("Got video getVTState call\n");

  ret = dbus_message_get_args(req, &err, DBUS_TYPE_UINT32, &vtnum, DBUS_TYPE_INVALID);
  if (ret == FALSE)
    {
      logdebug("video getVTState call with no/inappropriate arguments ?!\n");

      return;
    }

  /* Check VT state */
  vtstate = video_vt_active(vtnum);

  msg = dbus_message_new_method_return(req);

  ret = dbus_message_append_args(msg,
				 DBUS_TYPE_BOOLEAN, &vtstate,
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
      logdebug("Could not send video getVTState reply\n");

      dbus_message_unref(msg);

      return;
    }

  dbus_message_unref(msg);
}