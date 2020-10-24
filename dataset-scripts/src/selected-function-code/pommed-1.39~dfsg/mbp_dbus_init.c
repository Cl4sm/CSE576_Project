DBusConnection *
mbp_dbus_init(DBusError *error, unsigned int signals)
{
  err = error;

  dbus_error_init(err);

  conn = dbus_bus_get(DBUS_BUS_SYSTEM, err);
  if (dbus_error_is_set(err))
    {
      printf("DBus system bus connection failed: %s\n", err->message);

      dbus_error_free(err);

      conn = NULL;

      return NULL;
    }

  dbus_connection_set_exit_on_disconnect(conn, FALSE);

  if ((signals & MBP_DBUS_SIG_LCD)
      && (bus_add_match(conn, "type='signal',path='/org/pommed/notify/lcdBacklight',interface='org.pommed.signal.lcdBacklight'") < 0))
    {
      mbp_dbus_cleanup();
      return NULL;
    }

  if ((signals & MBP_DBUS_SIG_KBD)
      && (bus_add_match(conn, "type='signal',path='/org/pommed/notify/kbdBacklight',interface='org.pommed.signal.kbdBacklight'") < 0))
    {
      mbp_dbus_cleanup();
      return NULL;
    }

  if ((signals & MBP_DBUS_SIG_VOL)
      && (bus_add_match(conn, "type='signal',path='/org/pommed/notify/audioVolume',interface='org.pommed.signal.audioVolume'") < 0))
    {
      mbp_dbus_cleanup();
      return NULL;
    }

  if ((signals & MBP_DBUS_SIG_MUTE)
      && (bus_add_match(conn, "type='signal',path='/org/pommed/notify/audioMute',interface='org.pommed.signal.audioMute'") < 0))
    {
      mbp_dbus_cleanup();
      return NULL;
    }

  if ((signals & MBP_DBUS_SIG_LIGHT)
      && (bus_add_match(conn, "type='signal',path='/org/pommed/notify/ambientLight',interface='org.pommed.signal.ambientLight'") < 0))
    {
      mbp_dbus_cleanup();
      return NULL;
    }

  if ((signals & MBP_DBUS_SIG_EJECT)
      && (bus_add_match(conn, "type='signal',path='/org/pommed/notify/cdEject',interface='org.pommed.signal.cdEject'") < 0))
    {
      mbp_dbus_cleanup();
      return NULL;
    }

  if ((signals & MBP_DBUS_SIG_VIDEO)
      && (bus_add_match(conn, "type='signal',path='/org/pommed/notify/videoSwitch',interface='org.pommed.signal.videoSwitch'") < 0))
    {
      mbp_dbus_cleanup();
      return NULL;
    }

  return conn;
}