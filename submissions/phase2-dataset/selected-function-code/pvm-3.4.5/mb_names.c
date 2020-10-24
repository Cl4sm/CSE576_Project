int
mb_names(tid, pattern, mp)
	int tid;
	char *pattern;
	struct pmsg *mp;
{
	struct pvmmclass *np;
	struct pvmmentry *ep;

#ifdef USE_GNU_REGEX
	void *pattbuff;
	int pattglob = 0;
#endif

	int cnt;

#ifdef USE_GNU_REGEX

	/* Check for "*" Global Match Pattern */

	if ( !strcmp( pattern, "*" ) )
		pattglob = 1;

	/* Compile Regular Expression */

	if ( !pattglob )
		pattbuff = pvmcompileregex( pattern );

#endif

	/* Count # of Classes */

	cnt = 0;

	for ( np = pvmmboxclasses->mc_link; np != pvmmboxclasses;
			np = np->mc_link )
	{
#ifdef USE_GNU_REGEX
		if ( pattglob
				|| ( ( pattbuff ) ?
					( pvmmatchregex( pattbuff, np->mc_name ) )
					: ( !strcmp( pattern, np->mc_name ) ) ) )
#else
		if ( pvmmatchstring( np->mc_name, pattern ) )
#endif
		{
			cnt++;
		}
	}

	pkint( mp, cnt );

	/* Pack Class Info */

	for ( np = pvmmboxclasses->mc_link; np != pvmmboxclasses;
			np = np->mc_link )
	{
#ifdef USE_GNU_REGEX
		if ( pattglob
				|| ( ( pattbuff ) ?
					( pvmmatchregex( pattbuff, np->mc_name ) )
					: ( !strcmp( pattern, np->mc_name ) ) ) )
#else
		if ( pvmmatchstring( np->mc_name, pattern ) )
#endif
		{
			pkstr( mp, np->mc_name );

			/* Count # of Entries */

			cnt = 0;

			for ( ep = np->mc_ent->me_link; ep != np->mc_ent;
					ep = ep->me_link )
				cnt++;

			pkint( mp, cnt );

			/* Pack Entry Info */

			for ( ep = np->mc_ent->me_link; ep != np->mc_ent;
					ep = ep->me_link )
			{
				pkint( mp, ep->me_ind );
				pkint( mp, ep->me_tid );
				pkint( mp, ep->me_flags );
			}
		}
	}

#ifdef USE_GNU_REGEX
	if ( !pattglob && pattbuff )
		pvmfreeregex( &pattbuff );
#endif

	return 0;
}
