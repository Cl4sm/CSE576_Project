tev_init()
{
	int i;

	if ( pvmtrc.trctid > 0 && pvmmytid != pvmtrc.trctid )
	{
		/* Reset Trace Descriptor Counts */

		for ( i=TEV_FIRST ; i <= TEV_MAX ; i++ )
		{
			pvmtevinfo[i].desc_status = 0;

			pvmtevinfo[i].mark.tv_sec =
				pvmtevinfo[i].mark.tv_usec = 0;

			pvmtevinfo[i].total.tv_sec =
				pvmtevinfo[i].total.tv_usec = 0;

			pvmtevinfo[i].count = 0;
		}
	}
}
