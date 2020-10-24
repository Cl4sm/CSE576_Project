int
sleep_dammit( msecs )
int msecs;
{
#if defined(IMA_SUNMP) || defined(IMA_RS6KMP) || defined(IMA_AIX4MP) \
		defined(IMA_AIX5MP)
	return( usleep( msecs ) );
#else
	struct timeval biteme;

	biteme.tv_sec = 0;
	biteme.tv_usec = msecs;

	while ( biteme.tv_usec > 1000000 )
	{
		(biteme.tv_sec)++;
		biteme.tv_usec -= 1000000;
	}

	return( select( 0,
#ifdef	FDSETISINT
				(int *) NULL, (int *) NULL, (int *) NULL,
#else
				(fd_set *) NULL, (fd_set *) NULL, (fd_set *) NULL,
#endif
				&biteme ) );
#endif
}
