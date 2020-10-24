find_address_conf(const char *host, const char *user, struct irc_ssaddr *ip,
                  int aftype, const char *password)
{
  struct MaskItem *authcnf = NULL, *killcnf = NULL;

  /* Find the best auth{} block... If none, return NULL -A1kmm */
  if ((authcnf = find_conf_by_address(host, ip, CONF_CLIENT, aftype, user,
                                      password, 1)) == NULL)
    return NULL;

  /* If they are exempt from K-lines, return the best auth{} block. -A1kmm */
  if (IsConfExemptKline(authcnf))
    return authcnf;

  /* Find the best K-line... -A1kmm */
  killcnf = find_conf_by_address(host, ip, CONF_KLINE, aftype, user, NULL, 1);

  /*
   * If they are K-lined, return the K-line. Otherwise, return the
   * auth{} block. -A1kmm
   */
  if (killcnf)
    return killcnf;

  if (IsConfExemptGline(authcnf))
    return authcnf;

  killcnf = find_conf_by_address(host, ip, CONF_GLINE, aftype, user, NULL, 1);
  if (killcnf)
    return killcnf;

  return authcnf;
}
