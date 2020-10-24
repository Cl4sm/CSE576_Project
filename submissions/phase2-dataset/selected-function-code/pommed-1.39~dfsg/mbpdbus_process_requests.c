static DBusHandlerResult
mbpdbus_process_requests(DBusConnection *lconn, DBusMessage *msg, void *data)
{
  // Get methods
  if (dbus_message_is_method_call(msg, "org.pommed.lcdBacklight", "getLevel"))
    process_lcd_getlevel_call(msg);
  else if (dbus_message_is_method_call(msg, "org.pommed.kbdBacklight", "getLevel"))
    process_kbd_getlevel_call(msg);
  else if (dbus_message_is_method_call(msg, "org.pommed.ambient", "getLevel"))
    process_ambient_getlevel_call(msg);
  else if (dbus_message_is_method_call(msg, "org.pommed.audio", "getVolume"))
    process_audio_getvolume_call(msg);
  else if (dbus_message_is_method_call(msg, "org.pommed.audio", "getMute"))
    process_audio_getmute_call(msg);
  else if (dbus_message_is_method_call(msg, "org.pommed.video", "getVTState"))
    process_video_getvtstate_call(msg);
  // Set methods
  else if (dbus_message_is_method_call(msg, "org.pommed.lcdBacklight", "levelUp"))
    process_lcd_backlight_step_call(msg, STEP_UP);
  else if (dbus_message_is_method_call(msg, "org.pommed.lcdBacklight", "levelDown"))
    process_lcd_backlight_step_call(msg, STEP_DOWN);
  else if (dbus_message_is_method_call(msg, "org.pommed.kbdBacklight", "inhibit"))
    process_kbd_backlight_inhibit_call(msg, 1);
  else if (dbus_message_is_method_call(msg, "org.pommed.kbdBacklight", "disinhibit"))
    process_kbd_backlight_inhibit_call(msg, 0);
  else if (dbus_message_is_method_call(msg, "org.pommed.audio", "volumeUp"))
    process_audio_volume_step_call(msg, STEP_UP);
  else if (dbus_message_is_method_call(msg, "org.pommed.audio", "volumeDown"))
    process_audio_volume_step_call(msg, STEP_DOWN);
  else if (dbus_message_is_method_call(msg, "org.pommed.audio", "toggleMute"))
    process_audio_toggle_mute_call(msg);
  else if (dbus_message_is_method_call(msg, "org.pommed.cd", "eject"))
    process_cd_eject_call(msg);
  else if (dbus_message_is_signal(msg, DBUS_INTERFACE_LOCAL, "Disconnected"))
    {
      logmsg(LOG_INFO, "DBus disconnected");

      mbpdbus_cleanup();

      dbus_timer = evloop_add_timer(DBUS_TIMEOUT, mbpdbus_reconnect);
      if (dbus_timer < 0)
	logmsg(LOG_WARNING, "Could not set up timer for DBus reconnection");
    }
  else
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;

  return DBUS_HANDLER_RESULT_HANDLED;
}