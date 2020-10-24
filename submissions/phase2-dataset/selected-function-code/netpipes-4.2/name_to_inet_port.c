int name_to_inet_port(portname)
char *portname;
/* This procedure converts a character string to a port number.  It looks
   up the service by name and if there is none, then it converts the string
   to a number with sscanf. Return value in host byte order! */
{
  struct servent	*p;

  if (portname==NULL)
    return 0;

  p = getservbyname(portname,"tcp");
  if (p!=NULL)
    {
      return p->s_port;
    }
  else
    {
      int	port;
      if (sscanf(portname,"%i",&port)!=1)
	{
	  return 0;
	}
      else
	return port;
    }
}