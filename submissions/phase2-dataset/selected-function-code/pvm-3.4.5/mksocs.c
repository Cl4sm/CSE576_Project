mksocs()
{
	struct hostd *hp = hosts->ht_hosts[hosts->ht_local];
	struct hostd *hp0 = hosts->ht_hosts[0];
	struct sockaddr_in sin;
	char buf[128];
	char *sfn;
#ifndef WIN32
	int d;
#else
	HANDLE d;
	int e=0;
#endif
#ifndef NOSOCKOPT
	int bsz;
#endif
	char *p;
#ifdef SOCKLENISUINT
#if defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2) \
		|| defined(IMA_AIX56K64) || defined(IMA_LINUXALPHA)
	unsigned int oslen;
#else
	size_t oslen;
#endif
#else
	int oslen;
#endif
	int cc;
#ifndef NOUNIXDOM
	char spath[PVMTMPNAMLEN];	/* local socket path */
	struct sockaddr_un uns;
#endif
#ifdef IMA_BEOLIN
	struct hostent *hostaddr;
#endif
	short tmpp;

	/*
	* make pvmd-pvmd socket
	*/

	if ((netsock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		pvmlogperror("mksocs() socket netsock");
		return 1;
	}

	p = getenv("PVMNETSOCKPORT");

	tmpp = (p) ? atoi(p) : 0;

	hp->hd_sad.sin_port = htons(tmpp);

	do
	{
		oslen = sizeof(hp->hd_sad);
		if ((cc=bind(netsock, (struct sockaddr*)&hp->hd_sad, oslen))
				== -1)
		{
			if (p) {
				tmpp++;
				hp->hd_sad.sin_port = htons(tmpp);
			} else {
				pvmlogperror("mksocs() bind netsock");
				return 1;
			}
		}
	}
	while ( cc == -1 && p && hp->hd_sad.sin_port < 65535 );

	if ( cc == -1 ) {
		pvmlogperror("mksocs() bind netsock");
		return 1;
	}

	oslen = sizeof(hp->hd_sad);
	if (getsockname(netsock, (struct sockaddr*)&hp->hd_sad, &oslen) == -1) {
		pvmlogperror("mksocs() getsockname netsock");
		return 1;
	}

	/*
	* make pvmd-pvmd' socket
	*/

	if ((ppnetsock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		pvmlogperror("mksocs() socket ppnetsock");
		return 1;
	}

	hp0->hd_sad.sin_port = 0;
	oslen = sizeof(hp0->hd_sad);
	if (bind(ppnetsock, (struct sockaddr*)&hp0->hd_sad, oslen) == -1) {
		pvmlogperror("mksocs() bind ppnetsock");
		return 1;
	}
	oslen = sizeof(hp0->hd_sad);
	if (getsockname(ppnetsock, (struct sockaddr*)&hp0->hd_sad, &oslen) == -1) {
		pvmlogperror("mksocs() getsockname ppnetsock");
		return 1;
	}

	/*
	* make pvmd-local task socket
	*/

#ifdef NOUNIXDOM
	if ((loclsock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		pvmlogperror("mksocs() socket loclsock");
		return 1;
	}

	/*
	* first try localhost address (loopback) then regular address
	* XXX 127.0.0.1 is a hack, we should really gethostbyaddr()
	*/

	BZERO((char*)&sin, sizeof(sin));
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	sin = hp->hd_sad;		/* allow task to connect from a node */
#else

#ifdef IMA_BEOLIN
	/* allow connection from another node */
	sin.sin_family = AF_INET;
	if (gethostname(buf, sizeof(buf)-1) == -1) {
		pvmlogerror("mksocs() can't gethostname()\n");
		return 1;
	} else {
		/* got name, now get addr */
		if (!(hostaddr = gethostbyname( buf ))) {
			pvmlogprintf( "mksocs() can't gethostbyname() for %s\n", buf );
			return 1;
		} else {
			/* got addr, now save it */
			BCOPY( hostaddr->h_addr_list[0], (char*)&sin.sin_addr,
					sizeof(struct in_addr));
		}
	}
	sin.sin_port = 0;
#else
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(0x7f000001);
	sin.sin_port = 0;
#endif

#endif

	oslen = sizeof(sin);
	if (bind(loclsock, (struct sockaddr*)&sin, oslen) == -1) {
		sin = hp->hd_sad;
		oslen = sizeof(sin);
		if (bind(loclsock, (struct sockaddr*)&sin, oslen) == -1) {
			pvmlogperror("mksocs() bind loclsock");
			return 1;
		}
	}
	oslen = sizeof(sin);
	if (getsockname(loclsock, (struct sockaddr*)&sin, &oslen) == -1) {
		pvmlogperror("mksocs() getsockname loclsock");
		return 1;
	}

	if (listen(loclsock, SOMAXCONN) == -1) {
		pvmlogperror("mksocs() listen loclsock");
		return 1;
	}

#ifndef NOSOCKOPT
	bsz = pvmudpmtu * 2;
	if (setsockopt(netsock, SOL_SOCKET, SO_SNDBUF,
			(char*)&bsz, sizeof(bsz)) == -1
	|| setsockopt(netsock, SOL_SOCKET, SO_RCVBUF,
			(char*)&bsz, sizeof(bsz)) == -1
	|| setsockopt(ppnetsock, SOL_SOCKET, SO_SNDBUF,
			(char*)&bsz, sizeof(bsz)) == -1
	|| setsockopt(ppnetsock, SOL_SOCKET, SO_RCVBUF,
			(char*)&bsz, sizeof(bsz)) == -1) {
		pvmlogperror("mksocs() setsockopt");
		return 1;
	}
#endif /*NOSOCKOPT*/

	p = inadport_hex(&sin);

#else /*NOUNIXDOM*/
	if ((loclsock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		pvmlogperror("mksocs() socket loclsock");
		return 1;
	}

	BZERO((char*)&uns, sizeof(uns));
	uns.sun_family = AF_UNIX;
	spath[0] = 0;
	(void)PVMTMPNAMFUN(spath);
	strcpy(uns.sun_path, spath);
/*
XXX len?
*/

	oslen = sizeof(uns);
	if (bind(loclsock, (struct sockaddr*)&uns, oslen) == -1) {
		pvmlogperror("mksocs() bind loclsock");
		return 1;
	}

	if (listen(loclsock, SOMAXCONN) == -1) {
		pvmlogperror("mksocs() listen loclsock");
		return 1;
	}

	loclspath = STRALLOC(spath);
	p = spath;

#endif /*NOUNIXDOM*/

	/*
	* make pvmd-local task socket address file
	*/

	if (!(sfn = pvmdsockfile())) {
		pvmlogerror("mksocs() pvmdsockfile() failed\n");
		pvmbailout(0);
	}

#ifndef WIN32
	if ((d = open(sfn, O_CREAT|O_EXCL|O_WRONLY|O_TRUNC, 0600)) == -1) {
		if (errno == EEXIST) {
#else
	d = win32_create_file(sfn, CREATE_NEW);
	if ((int) d == -2){
		/* this code is for WIN95 */
		system_loser_win = TRUE;
		e = _open(sfn,O_CREAT|O_EXCL|O_WRONLY|O_TRUNC, 0600);
	}
	if (((int)d == -1) || (e ==-1)) {
		if (1) {  /* errno == EEXIST */
#endif
#ifndef	OVERLOADHOST
#ifdef WIN32
			if (d == INVALID_HANDLE_VALUE) {
				(void) pvmlogprintf("mksocs() %s failed. You are required to run on NTFS\n", sfn);
				return 1; /* same as write address */
			} else
#endif
			(void)pvmlogprintf(
					"mksocs() %s exists.  pvmd already running?\n", sfn);
			return 2;
#endif

		} else {
			pvmlogperror(sfn);
			pvmlogerror("mksocs() can't write address file\n");
			return 1;
		}
	} else {
#ifndef WIN32
		cc = write(d, p, strlen(p));
#else
		CloseHandle(d);
		if (!system_loser_win) {
			if ((d = (HANDLE) win32_open_file(sfn)) == (HANDLE) -1) {
				pvmlogprintf("could not open file %s: %d \n",
						sfn,GetLastError());
			return 2;
			}
			cc = win32_write_file(d,p,strlen(p));
		} else
			/* the win95 case */
			cc = write(e,p,strlen(p));
#endif
		if (cc != strlen(p)) {
			if (cc == -1) {
				pvmlogperror(sfn);
				pvmlogerror("mksocs() can't write address file\n");

			} else {
				(void)pvmlogprintf(
						"mksocs() aargh, short write on %s: %d\n",
						sfn, cc);
				pvmlogerror("mksocs() is the partition full?\n");
			}
#ifndef WIN32
			(void)close(d);
#else
			if (d)
				CloseHandle(d);
			if (e)
				close(e);
#endif
			(void)unlink(sfn);
			return 1;
		}
		loclsnam = STRALLOC(sfn);
#ifndef WIN32
		(void)close(d);
#else
		if (d)
			win32_close_file(d);
		if (e)
			close(e);
#endif
	}

	/* set PVMSOCK envar */

	sprintf(buf, "PVMSOCK=%s", p);
	p = STRALLOC(buf);
	pvmputenv(p);

	return 0;
}
