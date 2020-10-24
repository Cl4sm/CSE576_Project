int
pvmmatchstring( str, pattern )
char *str;
char *pattern;
{
	char *start;
	char *save;
	char *x;
	char *y;

	/* Find Start of Search String (Ignore Preceding '*'s) */

	start = pattern;

	while ( *start == '*' && *start != '\0' )
		start++;

	/* Null Search - Always Matches */

	if ( *start == '\0' )
		return( 1 );

	/* Initialize Pointers */

	x = str;

	y = start;

	/* Search Line for Start of Search String */

	while ( *x != '\0' )
	{
		/* Starting Match Found...  Check it Out */

		if ( *x == *y )
		{
			/* Save Continuation Pointer */

			save = x + 1;

			/* Traverse Line Until No Longer Matches */

			while ( *x != '\0' && *y != '\0' && *x == *y )
			{
				x++;
				y++;

				/* Nested '*' Search Expansion */

				if ( *y == '*' )
				{
					if ( pvmmatchstring( x, y + 1 ) )
						return( 1 );

					else
						break;
				}

				/* Skip Over '\' for Escaped '*'s */

				else if ( *y == '\\' && *(y+1) == '*' )
					y++;
			}

			/* It's a Match! */

			if ( *y == '\0' )
				return( 1 );

			/* Reset and Continue */

			x = save;

			y = start;
		}

		else
			x++;
	}

	return( 0 );
}
