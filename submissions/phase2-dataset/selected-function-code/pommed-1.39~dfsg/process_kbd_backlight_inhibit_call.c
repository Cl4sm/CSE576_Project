static void
process_kbd_backlight_inhibit_call(DBusMessage *req, int inhibit)
{
  DBusMessage *msg;

  int ret;

  logdebug("Got kbdBacklight inhibit call\n");

  if (inhibit)
    kbd_backlight_inhibit_set(KBD_INHIBIT_USER);
  else
    kbd_backlight_inhibit_clear(KBD_INHIBIT_USER);

  msg = dbus_message_new_method_return(req);

  ret = dbus_connection_send(conn, msg, NULL);
  if (ret == FALSE)
    {
      logdebug("Could not send kbdBacklight inhibit reply\n");

      dbus_message_unref(msg);

      return;
    }

  dbus_message_unref(msg);
}