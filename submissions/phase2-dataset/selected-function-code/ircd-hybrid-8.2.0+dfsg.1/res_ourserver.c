res_ourserver(const struct irc_ssaddr *inp)
{
  const struct sockaddr_in6 *v6in = (const struct sockaddr_in6 *)inp;
  const struct sockaddr_in *v4in = (const struct sockaddr_in *)inp;

  for (unsigned int i = 0; i < irc_nscount; ++i)
  {
    const struct irc_ssaddr *srv = &irc_nsaddr_list[i];
    const struct sockaddr_in6 *v6 = (const struct sockaddr_in6 *)srv;
    const struct sockaddr_in *v4 = (const struct sockaddr_in *)srv;

    /*
     * Could probably just memcmp(srv, inp, srv.ss_len) here
     * but we'll air on the side of caution - stu
     */
    switch (srv->ss.ss_family)
    {
      case AF_INET6:
        if (srv->ss.ss_family == inp->ss.ss_family)
          if (v6->sin6_port == v6in->sin6_port)
            if (!memcmp(&v6->sin6_addr.s6_addr, &v6in->sin6_addr.s6_addr,
                        sizeof(struct in6_addr)))
              return 1;
        break;
      case AF_INET:
        if (srv->ss.ss_family == inp->ss.ss_family)
          if (v4->sin_port == v4in->sin_port)
            if (v4->sin_addr.s_addr == v4in->sin_addr.s_addr)
              return 1;
        break;
      default:
        break;
    }
  }

  return 0;
}
