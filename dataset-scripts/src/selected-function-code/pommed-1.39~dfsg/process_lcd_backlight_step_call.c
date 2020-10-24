static void
process_lcd_backlight_step_call(DBusMessage *req, int dir)
{
  DBusMessage *msg;

  int ret;

  logdebug("Got lcdBacklight levelUp/levelDown call\n");

  mops->lcd_backlight_step(dir);

  msg = dbus_message_new_method_return(req);

  ret = dbus_connection_send(conn, msg, NULL);
  if (ret == FALSE)
    {
      logdebug("Could not send lcdBacklight levelUp/levelDown reply\n");

      dbus_message_unref(msg);

      return;
    }

  dbus_message_unref(msg);
}