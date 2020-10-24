static int
evdev_is_wellspring4a(unsigned short *id)
{
  unsigned short product = id[ID_PRODUCT];

  if (id[ID_BUS] != BUS_USB)
    return 0;

  if (id[ID_VENDOR] != USB_VENDOR_ID_APPLE)
    return 0;

  if ((product == USB_PRODUCT_ID_WELLSPRING4A_ANSI)
      || (product == USB_PRODUCT_ID_WELLSPRING4A_ISO)
      || (product == USB_PRODUCT_ID_WELLSPRING4A_JIS))
    {
      logdebug(" -> WellSpring IVa USB assembly\n");

      kbd_set_fnmode();

      return 1;
    }

  return 0;
}