int
mbp_get_x_vtnum(Display *dpy)
{
  Atom prop;
  Atom actualtype;
  int actualformat;
  unsigned long nitems;
  unsigned long bytes_after;
  unsigned char *buf;
  int num;

  prop = XInternAtom (dpy, "XFree86_VT", False);
  if (prop == None)
      return -1;

  if (XGetWindowProperty (dpy, DefaultRootWindow (dpy), prop, 0, 1,
			  False, AnyPropertyType, &actualtype, &actualformat,
			  &nitems, &bytes_after, &buf))
    {
      return -1;
    }

  if (nitems != 1)
    {
      XFree (buf);
      return -1;
    }

  switch (actualtype)
    {
      case XA_CARDINAL:
      case XA_INTEGER:
      case XA_WINDOW:
	switch (actualformat)
	  {
	    case 8:
	      num = (*(uint8_t  *)(void *)buf);
	      break;
	    case 16:
	      num = (*(uint16_t *)(void *)buf);
	      break;
	    case 32:
	      num = (*(uint32_t *)(void *)buf);
	      break;
	    default:
	      XFree (buf);
	      return -1;
	  }
	break;
      default:
	XFree (buf);
	return -1;
    }

  XFree (buf);

  return num;
}