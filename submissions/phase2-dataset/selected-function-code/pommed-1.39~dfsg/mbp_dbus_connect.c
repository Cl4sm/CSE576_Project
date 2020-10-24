static int
mbp_dbus_connect(void)
{
  unsigned int signals;

  DBusMessage *msg;

  int ret;

  signals = MBP_DBUS_SIG_LCD | MBP_DBUS_SIG_KBD
    | MBP_DBUS_SIG_VOL | MBP_DBUS_SIG_MUTE
    | MBP_DBUS_SIG_EJECT | MBP_DBUS_SIG_VIDEO;

  conn = mbp_dbus_init(&dbus_err, signals);

  if (conn == NULL)
    return -1;

  dbus_connection_setup_with_g_main(conn, NULL);

  dbus_connection_add_filter(conn, mbp_dbus_listen, NULL, NULL);

  /* Get the mute state */
  msg = dbus_message_new_method_call("org.pommed", "/org/pommed/audio",
				     "org.pommed.audio", "getMute");

  if (msg == NULL)
    {
      printf("Failed to create method call message for audio getMute\n");

      return 0;
    }

  ret = dbus_connection_send(conn, msg, &mute_serial);
  if (ret == FALSE)
    {
      printf("Could not send method call for audio getMute\n");

      dbus_message_unref(msg);

      return 0;
    }

  dbus_connection_flush(conn);

  dbus_message_unref(msg);

  return 0;
}