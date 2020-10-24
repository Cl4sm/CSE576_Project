static void
report_and_set_user_flags(struct Client *source_p, const struct MaskItem *conf)
{
  /* If this user is being spoofed, tell them so */
  if (IsConfDoSpoofIp(conf))
    sendto_one_notice(source_p, &me, ":*** Spoofing your IP. Congrats.");

  /* If this user is in the exception class, Set it "E lined" */
  if (IsConfExemptKline(conf))
  {
    SetExemptKline(source_p);
    sendto_one_notice(source_p, &me, ":*** You are exempt from K/D/G lines. Congrats.");
  }

  /*
   * The else here is to make sure that G line exempt users
   * do not get noticed twice.
   */
  else if (IsConfExemptGline(conf))
  {
    SetExemptGline(source_p);
    sendto_one_notice(source_p, &me, ":*** You are exempt from G lines. Congrats.");
  }

  if (IsConfExemptResv(conf))
  {
    SetExemptResv(source_p);
    sendto_one_notice(source_p, &me, ":*** You are exempt from resvs. Congrats.");
  }

  /* If this user is exempt from user limits set it "F lined" */
  if (IsConfExemptLimits(conf))
  {
    SetExemptLimits(source_p);
    sendto_one_notice(source_p, &me, ":*** You are exempt from user limits. Congrats.");
  }

  if (IsConfCanFlood(conf))
  {
    SetCanFlood(source_p);
    sendto_one_notice(source_p, &me, ":*** You are exempt from flood "
                      "protection, aren't you fearsome.");
  }
}
