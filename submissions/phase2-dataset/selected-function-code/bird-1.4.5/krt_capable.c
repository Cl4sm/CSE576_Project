krt_capable(rte *e)
{
  rta *a = e->attrs;

  if (a->cast != RTC_UNICAST)
    return 0;

  switch (a->dest)
    {
    case RTD_ROUTER:
    case RTD_DEVICE:
      if (a->iface == NULL)
	return 0;
    case RTD_BLACKHOLE:
    case RTD_UNREACHABLE:
    case RTD_PROHIBIT:
    case RTD_MULTIPATH:
      break;
    default:
      return 0;
    }
  return 1;
}
