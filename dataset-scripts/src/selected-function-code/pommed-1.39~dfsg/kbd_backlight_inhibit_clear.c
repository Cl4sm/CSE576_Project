void
kbd_backlight_inhibit_clear(int mask)
{
  int flag;

  flag = kbd_bck_info.inhibit & mask;

  kbd_bck_info.inhibit &= ~mask;

  logdebug("KBD: inhibit clear 0x%02x -> 0x%02x\n", mask, kbd_bck_info.inhibit);

  if (kbd_bck_info.inhibit || !flag)
    return;

  kbd_backlight_set(kbd_bck_info.inhibit_lvl,
		    (mask & KBD_MASK_AUTO) ? (KBD_AUTO) : (KBD_USER));

  if (kbd_bck_info.auto_on)
    {
      kbd_bck_info.auto_on = 0;
      kbd_bck_info.inhibit_lvl = 0;
    }
}