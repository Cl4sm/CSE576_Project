static int
evdev_is_wellspring5a(unsigned short *id)
{
  unsigned short product = id[ID_PRODUCT];

  if (id[ID_BUS] != BUS_USB)
    return 0;

  if (id[ID_VENDOR] != USB_VENDOR_ID_APPLE)
    return 0;

  if ((product == USB_PRODUCT_ID_WELLSPRING5A_ANSI)
      || (product == USB_PRODUCT_ID_WELLSPRING5A_ISO)
      || (product == USB_PRODUCT_ID_WELLSPRING5A_JIS))
    {
      logdebug(" -> WellSpring VA USB assembly\n");

      kbd_set_fnmode();

      return 1;
    }

  return 0;
}