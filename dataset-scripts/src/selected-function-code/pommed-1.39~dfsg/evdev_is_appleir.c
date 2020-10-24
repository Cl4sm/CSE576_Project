static int
evdev_is_appleir(unsigned short *id)
{
  unsigned short product = id[ID_PRODUCT];

  if (id[ID_BUS] != BUS_USB)
    return 0;

  if (id[ID_VENDOR] != USB_VENDOR_ID_APPLE)
    return 0;

  if ((product == USB_PRODUCT_ID_APPLEIR)
      || (product == USB_PRODUCT_ID_APPLEIR_2))
    {
      logdebug(" -> Apple IR receiver\n");

      return 1;
    }

  return 0;
}