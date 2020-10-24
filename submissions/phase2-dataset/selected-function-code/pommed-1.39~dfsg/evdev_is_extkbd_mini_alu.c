static int
evdev_is_extkbd_mini_alu(unsigned short *id)
{
  unsigned short product = id[ID_PRODUCT];

  if (id[ID_BUS] != BUS_USB)
    return 0;

  if (id[ID_VENDOR] != USB_VENDOR_ID_APPLE)
    return 0;

  if ((product == USB_PRODUCT_ID_APPLE_EXTKBD_MINI_ALU_ANSI)
      || (product == USB_PRODUCT_ID_APPLE_EXTKBD_MINI_ALU_ISO)
      || (product == USB_PRODUCT_ID_APPLE_EXTKBD_MINI_ALU_JIS))
    {
      logdebug(" -> External Apple USB mini keyboard (aluminium)\n");

      kbd_set_fnmode();

      return 1;
    }

  return 0;
}