void
wmmbp_get_values(void)
{
  int ret;
  int cbret;

  ret = mbp_call_lcd_getlevel(wmmbp_lcd_getlevel_cb, &cbret);
  if ((ret < 0) || (cbret < 0))
    {
      fprintf(stderr, "lcdBacklight getLevel call failed !\n");
      goto mcall_error;
    }

  ret = mbp_call_kbd_getlevel(wmmbp_kbd_getlevel_cb, &cbret);
  if ((ret < 0) || (cbret < 0))
    {
      fprintf(stderr, "kbdBacklight getLevel call failed !\n");
      goto mcall_error;
    }

  ret = mbp_call_ambient_getlevel(wmmbp_ambient_getlevel_cb, &cbret);
  if ((ret < 0) || (cbret < 0))
    {
      fprintf(stderr, "ambient getLevel call failed !\n");
      goto mcall_error;
    }

  ret = mbp_call_audio_getvolume(wmmbp_audio_getvolume_cb, &cbret);
  if ((ret < 0) || (cbret < 0))
    {
      fprintf(stderr, "audio getVolume call failed !\n");
      goto mcall_error;
    }

  ret = mbp_call_audio_getmute(wmmbp_audio_getmute_cb, &cbret);
  if ((ret < 0) || (cbret < 0))
    {
      fprintf(stderr, "audio getMute call failed !\n");
      goto mcall_error;
    }

  if (DISPLAY_TYPE(mbpdisplay) <= DISPLAY_TYPE_NO_DATA)
    mbpdisplay = DISPLAY_TYPE_MACBOOK;

  mbpdisplay |= DISPLAY_FLAG_UPDATE;

  return;

 mcall_error:
  mbpdisplay = DISPLAY_FLAG_UPDATE | DISPLAY_TYPE_NO_DATA;
}