int
mbpdbus_init(void)
{
  int ret;

  watches = NULL;
  dbus_timer = -1;

  dbus_error_init(&err);

  conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
  if (dbus_error_is_set(&err))
    {
      logmsg(LOG_ERR, "DBus system bus connection failed: %s", err.message);

      dbus_error_free(&err);

      conn = NULL;

      goto init_reconnect;
    }

  dbus_connection_set_exit_on_disconnect(conn, FALSE);

  ret = dbus_bus_request_name(conn, "org.pommed", 0, &err);

  if (dbus_error_is_set(&err))
    {
      logmsg(LOG_ERR, "Failed to request DBus name: %s", err.message);

      mbpdbus_cleanup();

      goto init_reconnect;
    }

  if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)
    {
      logmsg(LOG_ERR, "Not primary DBus name owner");

      mbpdbus_cleanup();

      goto init_reconnect;
    }

  ret = dbus_connection_set_watch_functions(conn, mbpdbus_add_watch, mbpdbus_remove_watch,
					    mbpdbus_toggle_watch, NULL, mbpdbus_data_free);
  if (!ret)
    {
      mbpdbus_cleanup();

      goto init_reconnect;
    }

  dbus_connection_add_filter(conn, mbpdbus_process_requests, NULL, NULL);

  return 0;

 init_reconnect:
  if (dbus_timer > 0) /* Trying to reconnect already, called from mbpdbus_reconnect() */
      return -1;

  dbus_timer = evloop_add_timer(DBUS_TIMEOUT, mbpdbus_reconnect);
  if (dbus_timer < 0)
    {
      logmsg(LOG_ERR, "Could not set up timer for DBus reconnection");

      return -1;
    }

  return 0;
}