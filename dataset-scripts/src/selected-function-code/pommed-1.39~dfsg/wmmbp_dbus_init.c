int
wmmbp_dbus_init(void)
{
  unsigned int signals;

  signals = MBP_DBUS_SIG_LCD | MBP_DBUS_SIG_KBD
            | MBP_DBUS_SIG_VOL | MBP_DBUS_SIG_MUTE
            | MBP_DBUS_SIG_LIGHT | MBP_DBUS_SIG_VIDEO;

  conn = mbp_dbus_init(&dbus_err, signals);

  if (conn == NULL)
    {
      mbpdisplay = DISPLAY_FLAG_UPDATE | DISPLAY_TYPE_DBUS_NOK;

      return -1;
    }
  else
    wmmbp_get_values();

  return 0;
}