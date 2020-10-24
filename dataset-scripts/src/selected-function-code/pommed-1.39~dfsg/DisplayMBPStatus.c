void
DisplayMBPStatus(void)
{
  switch (DISPLAY_TYPE(mbpdisplay))
    {
      case DISPLAY_TYPE_MACBOOK:
	BlitString("MacBook", 4, 4);
	DrawGreenDot();

	BlitString("LCD level", 4, 18);
	DrawBar(((float)mbp.lcd_lvl / (float)mbp.lcd_max) * 100.0, 4, 27);

	BlitString("KBD level", 4, 32);
	DrawGreenBar(((float)mbp.kbd_lvl / (float)mbp.kbd_max) * 100.0, 4, 41);

	if (mbp.snd_mute)
	  BlitString("Audio OFF", 4, 46);
	else
	  BlitString("Audio    ", 4, 46);
	DrawGreenBar(((float)mbp.snd_lvl / (float)mbp.snd_max) * 100.0, 4, 55);
	break;

      case DISPLAY_TYPE_AMBIENT:
	BlitString("Ambient", 4, 4);
	DrawYellowDot();

	BlitString("Left     ", 4, 18);
	DrawBar(((float)mbp.ambient_l / (float)mbp.ambient_max) * 100.0, 4, 27);

	BlitString("Right    ", 4, 32);
	DrawBar(((float)mbp.ambient_r / (float)mbp.ambient_max) * 100.0, 4, 41);

	BlitString("KBD level", 4, 46);
	DrawGreenBar(((float)mbp.kbd_lvl / (float)mbp.kbd_max) * 100.0, 4, 55);
	break;

      case DISPLAY_TYPE_DBUS_NOK:
	BlitString(" Error ", 4, 4);
	DrawRedDot();

	BlitString("DBus     ", 4, 18);
	DrawBar(0.0, 4, 27);

	BlitString("Connect  ", 4, 32);
	DrawGreenBar(0.0, 4, 41);

	BlitString("Failed   ", 4, 46);
	DrawGreenBar(0.0, 4, 55);
	break;

      case DISPLAY_TYPE_NO_DATA:
	BlitString("No Data", 4, 4);
	DrawRedDot();

	BlitString("Server   ", 4, 18);
	DrawBar(0.0, 4, 27);

	BlitString("Not      ", 4, 32);
	DrawGreenBar(0.0, 4, 41);

	BlitString("Running ?", 4, 46);
	DrawGreenBar(0.0, 4, 55);
	break;
    }

  mbpdisplay = DISPLAY_TYPE(mbpdisplay);
}