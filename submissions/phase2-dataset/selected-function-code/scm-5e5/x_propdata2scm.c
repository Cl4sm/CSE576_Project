SCM x_propdata2scm(type, format, nitems, data)
     Atom type;
     int format;
     unsigned long nitems;
     unsigned char* data;
{
  SCM datum = EOL;
  SCM lst = EOL;
  int cnt;
  for (cnt = nitems; cnt--;) {
    switch (type) {
    case XA_ATOM:
    case XA_VISUALID:
    case XA_CARDINAL:
      switch (format) {
      case 8: datum = MAKINUM(((unsigned char *)data)[cnt]); break;
      case 16: datum = MAKINUM(((unsigned short *)data)[cnt]); break;
      case 32: datum = ulong2num(((unsigned long *)data)[cnt]); break;
      default: return MAKINUM(format);
      } break;
    case XA_INTEGER:
      switch (format) {
      case 8: datum = MAKINUM(((char *)data)[cnt]); break;
      case 16: datum = MAKINUM(((short *)data)[cnt]); break;
      case 32: datum = long2num(((long *)data)[cnt]); break;
      default: return MAKINUM(format);
      } break;
    case XA_STRING:
      switch (format) {
      case 8: return makfrom0str(data);
      default: return MAKINUM(format);
      } break;
    case XA_ARC:
    case XA_BITMAP:
    case XA_COLORMAP:
    case XA_CURSOR:
    case XA_DRAWABLE:
    case XA_FONT:
    case XA_PIXMAP:
    case XA_POINT:
    case XA_RECTANGLE:
    case XA_RGB_COLOR_MAP:
    case XA_WINDOW:
    case XA_WM_HINTS:
    case XA_WM_SIZE_HINTS:
    default:
      /* datum = BOOL_F; */
      return MAKINUM(-type);
    }
    lst = cons(datum, lst);
  }
  return lst;
}
