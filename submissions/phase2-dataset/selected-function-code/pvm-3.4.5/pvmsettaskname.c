int
pvmsettaskname( name )
	char *name;
{
	if ( name == NULL )
		return lpvmerr( "pvmsettaskname", PvmBadParam );

	if ( pvmmytid != -1 )
		return lpvmerr( "pvmsettaskname", PvmExists );

	if ( pvmmytaskname != NULL )
	{
		PVM_FREE( pvmmytaskname );
		pvmmytaskname = (char *) NULL;
	}

	pvmmytaskname = STRALLOC( name );

	return 0;
}
