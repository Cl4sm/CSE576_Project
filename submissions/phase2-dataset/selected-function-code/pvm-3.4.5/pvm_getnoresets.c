pvm_getnoresets( tids, ntids )
	int **tids;
	int *ntids;
{
	static int *tidlist = (int *) NULL;
	static int num = 0;

	int index;
	int done;
	int save;
	int mid;
	int cnt;

	/* Allocate Initial Tids List */

	if ( !tidlist )
	{
		num = 16;
		tidlist = TALLOC( num, int, "int" );
	}

	/* Save Receive Buffer */

	save = pvm_setrbuf( 0 );

	/* Search for No Resets */

	index = 0;
	done = 0;
	cnt = 0;

	while ( !done )
	{
		if ( pvm_recvinfo( PVMNORESETCLASS, index, PvmMboxFirstAvail )
				<= 0 )
		{
			done++;
		}

		else
		{
			if ( cnt >= num )
			{
				num *= 2;

				tidlist = TREALLOC( tidlist, num, int );
			}

			pvm_upkint( &(tidlist[cnt++]), 1, 1 );

			index++;
		}
	}

	/* Restore Receive Buffer */

	pvm_setrbuf( save );

	/* Set Return Values */

	if ( tids != NULL )
		*tids = tidlist;

	if ( ntids != NULL )
		*ntids = cnt;

	return( PvmOk );
}
