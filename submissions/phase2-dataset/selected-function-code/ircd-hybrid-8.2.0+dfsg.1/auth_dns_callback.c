static void
auth_dns_callback(void *vptr, const struct irc_ssaddr *addr, const char *name, size_t namelength)
{
  struct AuthRequest *auth = vptr;

  ClearDNSPending(auth);

  if (!EmptyString(name))
  {
    const struct sockaddr_in *v4, *v4dns;
    const struct sockaddr_in6 *v6, *v6dns;

    if (auth->client->localClient->ip.ss.ss_family == AF_INET6)
    {
      v6 = (const struct sockaddr_in6 *)&auth->client->localClient->ip;
      v6dns = (const struct sockaddr_in6 *)addr;

      if (memcmp(&v6->sin6_addr, &v6dns->sin6_addr, sizeof(struct in6_addr)) != 0)
      {
        sendheader(auth->client, REPORT_IP_MISMATCH);
        release_auth_client(auth);
        return;
      }
    }
    else
    {
      v4 = (const struct sockaddr_in *)&auth->client->localClient->ip;
      v4dns = (const struct sockaddr_in *)addr;

      if (v4->sin_addr.s_addr != v4dns->sin_addr.s_addr)
      {
        sendheader(auth->client, REPORT_IP_MISMATCH);
        release_auth_client(auth);
        return;
      }
    }

    if (namelength > HOSTLEN)
      sendheader(auth->client, REPORT_HOST_TOOLONG);
    else
    {
      strlcpy(auth->client->host, name, sizeof(auth->client->host));
      sendheader(auth->client, REPORT_FIN_DNS);
    }
  }
  else
    sendheader(auth->client, REPORT_FAIL_DNS);

  release_auth_client(auth);
}
