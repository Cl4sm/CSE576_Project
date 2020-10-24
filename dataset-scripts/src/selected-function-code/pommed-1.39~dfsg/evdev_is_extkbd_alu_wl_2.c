static int
evdev_is_extkbd_alu_wl_2(unsigned short *id)
{
  unsigned short product = id[ID_PRODUCT];

  if (id[ID_BUS] != BUS_BLUETOOTH)
    return 0;

  if (id[ID_VENDOR] != USB_VENDOR_ID_APPLE)
    return 0;

  if ((product == USB_PRODUCT_ID_APPLE_EXTKBD_ALU_WL_2_ANSI)
      || (product == USB_PRODUCT_ID_APPLE_EXTKBD_ALU_WL_2_ISO)
      || (product == USB_PRODUCT_ID_APPLE_EXTKBD_ALU_WL_2_JIS))
    {
      logdebug(" -> External Apple wireless keyboard 2 (aluminium)\n");

      kbd_set_fnmode();

      return 1;
    }

  return 0;
}