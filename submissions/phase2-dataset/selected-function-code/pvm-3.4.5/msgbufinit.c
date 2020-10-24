msgbufinit(buf)
	char *buf;
{
	char *p, *end;

	end = buf + shmbufsiz;
#ifndef IMA_KSR1
#if defined(IMA_SUNMP) && defined(PVMUSEMUTEX)
	cond_init(&((struct msgboxhdr *)buf)->mb_cond, USYNC_PROCESS, 0);
#endif
#if defined(IMA_RS6KMP) || defined(IMA_AIX4MP) || defined(IMA_AIX5MP)
	pthread_cond_init(&((struct msgboxhdr *)buf)->mb_cond, NULL);
#endif
	PAGEINITLOCK(&((struct msgboxhdr *)buf)->mb_lock);
	((struct msgboxhdr *)buf)->mb_sleep = 0;
#endif
	((struct msgboxhdr *)buf)->mb_read = -1;
	((struct msgboxhdr *)buf)->mb_last = -1;

	for (p = outmsgbuf; p < end; p += pvmpgsz) {
#if !defined(TEST_ADD) && !defined(IMA_KSR1)
		PAGEINITLOCK(&((struct shmpghdr *)p)->pg_lock);
#endif
		((struct shmpghdr *)p)->pg_priv = 0;
		((struct shmpghdr *)p)->pg_ref = 0;
	}

	if (pvmdebmask & PDMNODE) {
		sprintf(pvmtxt, "msgbufinit() outmsgbuf=0x%x\n", outmsgbuf);
		pvmlogerror(pvmtxt);
		sprintf(pvmtxt, "msgbufinit() last buffer=0x%x\n", p - pvmpgsz);
		pvmlogerror(pvmtxt);
		sprintf(pvmtxt, "msgbufinit() pvmpgsz=%d\n", pvmpgsz);
		pvmlogerror(pvmtxt);
	}
	return 0;
}
