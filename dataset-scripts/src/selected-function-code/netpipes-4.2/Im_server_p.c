static int Im_server_p(sock_fd)
     int	sock_fd;
{
    struct sockaddr_storage	me, him;
    socklen_t len;
    int i;

    len = sizeof(me);
    getsockname(sock_fd, (struct sockaddr*)&me, &len);

    len = sizeof(him);
    getpeername(sock_fd, (struct sockaddr*)&him, &len);

    if(me.ss_family != him.ss_family)
	return (me.ss_family == AF_INET) ? 1 : 0;

    if(me.ss_family == AF_INET)
	i = memcmp(&(((struct sockaddr_in *) &me)->sin_addr),
			&(((struct sockaddr_in *) &me)->sin_addr),
			sizeof(struct in_addr));
    else
	i = memcmp(&((struct sockaddr_in6 *) &me)->sin6_addr,
			&((struct sockaddr_in6 *) &me)->sin6_addr,
			sizeof(struct in6_addr));
    if (i<0)
	return 1;
    else if (i>0)
	return 0;


    if (get_port(&me) < get_port(&him))
	return 1;
    else
	return 0;
}