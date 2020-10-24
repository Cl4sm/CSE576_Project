static DBusHandlerResult
mbp_dbus_listen(DBusConnection *lconn, DBusMessage *msg, gpointer userdata)
{
  int scratch;
  int cur;
  int max;
  int who;
  double ratio;

  Display *dpy;

  if (dbus_message_is_signal(msg, "org.pommed.signal.lcdBacklight", "lcdBacklight"))
    {
      dbus_message_get_args(msg, &dbus_err,
			    DBUS_TYPE_UINT32, &cur,
			    DBUS_TYPE_UINT32, &scratch, /* previous */
			    DBUS_TYPE_UINT32, &max,
			    DBUS_TYPE_UINT32, &who,
			    DBUS_TYPE_INVALID);

      if (who == LCD_USER)
	{
	  ratio = (double)cur / (double)max;

	  show_window(IMG_LCD_BCK, _("LCD backlight level"), ratio);
	}
    }
  else if (dbus_message_is_signal(msg, "org.pommed.signal.kbdBacklight", "kbdBacklight"))
    {
      dbus_message_get_args(msg, &dbus_err,
			    DBUS_TYPE_UINT32, &cur,
			    DBUS_TYPE_UINT32, &scratch, /* previous */
			    DBUS_TYPE_UINT32, &max,
			    DBUS_TYPE_UINT32, &who,
			    DBUS_TYPE_INVALID);

      if (who == KBD_USER)
	{
	  ratio = (double)cur / (double)max;

	  show_window(IMG_KBD_BCK, _("Keyboard backlight level"), ratio);
	}
    }
  else if (dbus_message_is_signal(msg, "org.pommed.signal.audioVolume", "audioVolume"))
    {
      dbus_message_get_args(msg, &dbus_err,
			    DBUS_TYPE_UINT32, &cur,
			    DBUS_TYPE_UINT32, &scratch, /* previous */
			    DBUS_TYPE_UINT32, &max,
			    DBUS_TYPE_INVALID);

      ratio = (double)cur / (double)max;

      if (!mbp.muted)
	show_window(IMG_AUDIO_VOL_ON, _("Sound volume"), ratio);
      else
	show_window(IMG_AUDIO_VOL_OFF, _("Sound volume (muted)"), ratio);
    }
  else if (dbus_message_is_signal(msg, "org.pommed.signal.audioMute", "audioMute"))
    {
      dbus_message_get_args(msg, &dbus_err,
			    DBUS_TYPE_BOOLEAN, &mbp.muted,
			    DBUS_TYPE_INVALID);

      if (mbp.muted)
	show_window(IMG_AUDIO_MUTE, _("Sound muted"), -1.0);
      else
	show_window(IMG_AUDIO_MUTE, _("Sound unmuted"), -1.0);
    }
  else if (dbus_message_is_signal(msg, "org.pommed.signal.cdEject", "cdEject"))
    {
      show_window(IMG_CD_EJECT, _("Eject"), -1.0);
    }
  else if (dbus_message_is_signal(msg, "org.pommed.signal.videoSwitch", "videoSwitch"))
    {
      int vtnum;
      int vtstate;
      int ret;

      dpy = GDK_WINDOW_XDISPLAY(GTK_WIDGET(mbp_w.window)->window);

      vtnum = mbp_get_x_vtnum(dpy);

      ret = mbp_call_video_getvtstate(vtnum, mbp_video_getvtstate_cb, &vtstate);
      if ((ret < 0) || (vtstate < 0))
	fprintf(stderr, "video getVTState call failed !\n");
      else if (vtstate == 1)
	mbp_video_switch();
    }
  else if (dbus_message_is_signal(msg, DBUS_INTERFACE_LOCAL, "Disconnected"))
    {
      printf("DBus disconnected\n");

      mbp_dbus_cleanup();

      g_timeout_add(200, mbp_dbus_reconnect, NULL);
    }
  else
    {
      if ((dbus_message_get_type(msg) == DBUS_MESSAGE_TYPE_METHOD_RETURN)
	  && (dbus_message_get_reply_serial(msg) == mute_serial))
	{
	  dbus_message_get_args(msg, &dbus_err,
				DBUS_TYPE_BOOLEAN, &mbp.muted,
				DBUS_TYPE_INVALID);
	}
      else
	{
	  return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}
    }

  return DBUS_HANDLER_RESULT_HANDLED;
}