get_channel_access(const struct Client *source_p,
                   const struct Membership *member)
{
  /* Let hacked servers in for now... */
  if (!MyClient(source_p))
    return CHACCESS_REMOTE;

  if (!member)
    return CHACCESS_NOTONCHAN;

  /* Just to be sure.. */
  assert(source_p == member->client_p);

  if (has_member_flags(member, CHFL_CHANOP))
    return CHACCESS_CHANOP;

  if (has_member_flags(member, CHFL_HALFOP))
    return CHACCESS_HALFOP;

  return CHACCESS_PEON;
}
