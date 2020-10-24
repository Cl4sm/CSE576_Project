krt_get_attr(eattr * a, byte * buf, int buflen UNUSED)
{
  switch (a->id)
  {
  case EA_KRT_SOURCE:
    bsprintf(buf, "source");
    return GA_NAME;

  case EA_KRT_METRIC:
    bsprintf(buf, "metric");
    return GA_NAME;

  case EA_KRT_PREFSRC:
    bsprintf(buf, "prefsrc");
    return GA_NAME;

  case EA_KRT_REALM:
    bsprintf(buf, "realm");
    return GA_NAME;

  default:
    return GA_UNKNOWN;
  }
}
