static void
krt_read_msg(struct proto *p, struct ks_msg *msg, int scan)
{
  /* p is NULL iff KRT_SHARED_SOCKET and !scan */

  switch (msg->rtm.rtm_type)
  {
    case RTM_GET:
      if(!scan) return;
    case RTM_ADD:
    case RTM_DELETE:
      krt_read_route(msg, (struct krt_proto *)p, scan);
      break;
    case RTM_IFANNOUNCE:
      krt_read_ifannounce(msg);
      break;
    case RTM_IFINFO:
      krt_read_ifinfo(msg, scan);
      break;
    case RTM_NEWADDR:
    case RTM_DELADDR:
      krt_read_addr(msg, scan);
      break;
    default:
      break;
  }
}
