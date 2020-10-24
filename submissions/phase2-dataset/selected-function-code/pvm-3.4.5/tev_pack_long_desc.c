tev_pack_long_desc( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int type;
	int cc;

	if ( (cc = PACK_INT( &did, 1, 1 )) )
		return( cc );

	type = TEV_DATA_LONG | array;
	if ( (cc = PACK_INT( &type, 1, 1 )) )
		return( cc );

	if ( array == TEV_DATA_ARRAY )
	{
		if ( (cc = PACK_INT( &cnt, 1, 1 )) )
			return( cc );
	}

	return( PACK_LONG( datap, cnt, std ) );
}
