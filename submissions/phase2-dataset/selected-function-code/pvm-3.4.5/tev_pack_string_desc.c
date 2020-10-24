static int
tev_pack_string_desc( did, array, datap, cnt, std )
int did;
int array;
void *datap;
int cnt;
int std;
{
	int type;
	int num;
	int cc;
	int i;

	if ( (cc = PACK_INT( &did, 1, 1 )) )
		return( cc );

	type = TEV_DATA_STRING | array;
	if ( (cc = PACK_INT( &type, 1, 1 )) )
		return( cc );

	if ( array == TEV_DATA_ARRAY )
	{
		if ( std <= 0 )
			return( PvmBadParam );

		num = ( cnt + ( std - 1 ) ) / std;
		if ( (cc = PACK_INT( &num, 1, 1 )) )
			return( cc );

		for ( i=0 ; i < cnt ; i += std )
			if ( (cc = PACK_STRING( ((char **) datap)[i] )) )
				return( cc );

		return( 0 );
	}

	else
		return( PACK_STRING( (char *) datap ) );
}
