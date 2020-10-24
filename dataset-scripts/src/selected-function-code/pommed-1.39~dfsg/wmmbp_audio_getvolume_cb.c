void
wmmbp_audio_getvolume_cb(DBusPendingCall *pending, void *status)
{
  DBusMessage *msg;

  msg = dbus_pending_call_steal_reply(pending);

  if (msg == NULL)
    {
      fprintf(stderr, "Could not steal reply\n");

      dbus_pending_call_unref(pending);

      return;
    }

  dbus_pending_call_unref(pending);

  if (!mbp_dbus_check_error(msg))
    {
      dbus_message_get_args(msg, &dbus_err,
			    DBUS_TYPE_UINT32, &mbp.snd_lvl,
			    DBUS_TYPE_UINT32, &mbp.snd_max,
			    DBUS_TYPE_INVALID);
    }
  else
    *(int *)status = -1;

  dbus_message_unref(msg);
}