int packet_go_on_the_link(unsigned char *pkt, int nr)
{
	int c, fd;
	struct sockaddr_ll sa;
	struct ifreq ifr;
        char buff[100];
	
	/* do we have the rights to do that? */
	if (getuid() && geteuid()) {
		//printf("Sorry but need the su rights!\n");
		error("Sorry but need the su rights!");
		return -2;
	}
	
	/* open socket in raw mode */
	fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (fd == -1) {
		//printf("Error: Could not open socket!\n");
		error("Error: Could not open socket!");
		return -2;
	}

	/* which interface would you like to use? */
	memset(&ifr, 0, sizeof(ifr));
	strncpy (ifr.ifr_name, iftext, sizeof(ifr.ifr_name) - 1);
	ifr.ifr_name[sizeof(ifr.ifr_name)-1] = '\0';

	if (ioctl(fd, SIOCGIFINDEX, &ifr) == -1) {
		//printf("No such interface: %s\n", iftext);
		snprintf(buff, 100, "No such interface: %s", iftext);
		error(buff);
		close(fd);
		return -2;
	}	

	/* is the interface up? */
        ioctl(fd, SIOCGIFFLAGS, &ifr);
	if ( (ifr.ifr_flags & IFF_UP) == 0) {
                //printf("Interface %s is down\n", iftext);
		snprintf(buff, 100, "Interface %s is down", iftext);
               	error(buff);
                close(fd);
                return -2;
        }

	/* just write in the structure again */
        ioctl(fd, SIOCGIFINDEX, &ifr);
	
	/* well we need this to work */
	memset(&sa, 0, sizeof (sa));
	sa.sll_family    = AF_PACKET;
	sa.sll_ifindex   = ifr.ifr_ifindex;
	sa.sll_protocol  = htons(ETH_P_ALL);

	c = sendto(fd, pkt, nr, 0, (struct sockaddr *)&sa, sizeof (sa));

	//printf("There were %d bytes sent on the wire (in case of an error we get -1)\n", c);

	if (close(fd) == 0) {
        	return (c);
	}
	else {
		//printf("Warning! close(fd) returned -1!\n");
		error("Warning! close(fd) returned -1!");
        	return (c);
	}

}