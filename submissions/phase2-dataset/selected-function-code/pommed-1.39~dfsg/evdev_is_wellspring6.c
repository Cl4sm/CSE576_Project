static int
evdev_is_wellspring6(unsigned short *id)
{
  unsigned short product = id[ID_PRODUCT];

  if (id[ID_BUS] != BUS_USB)
    return 0;

  if (id[ID_VENDOR] != USB_VENDOR_ID_APPLE)
    return 0;

  if ((product == USB_PRODUCT_ID_WELLSPRING6_ANSI)
      || (product == USB_PRODUCT_ID_WELLSPRING6_ISO)
      || (product == USB_PRODUCT_ID_WELLSPRING6_JIS))
    {
      logdebug(" -> WellSpring VI USB assembly\n");

      kbd_set_fnmode();

      return 1;
    }

  return 0;
}