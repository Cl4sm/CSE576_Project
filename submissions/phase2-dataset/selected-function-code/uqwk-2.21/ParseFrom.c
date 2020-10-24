char *c;
/*
 *  Extract the email address from a From: line
 */
{
	int type, n, i, where = 0;

	/*
	 *  Addresses come in three flavors:
	 *
	 *      1: seb3@gte.com
	 *      2: seb3@gte.com (steve belczyk)
	 *      3: steve belczyk <seb3@gte.com>
	 */

	/* Assume type 1 */
	type = 1;

	/* Look through address */
	n = strlen (c);
	for (i=0; i<n; i++) {
	        /* Change close-angle-bracket to null so we can
	           sscanf the address later */
	        if (c[i] == '>') c[i] = 0;

	        /* If we find an open-angle-bracket, assume type 3 */
	        if (c[i] == '<') {
	                type = 3;
	                where = i+1;
	        }
	}

	/* Now extract the address */
	if (type == 1) {
	        /* This works for type 2 addresses too */
	        sscanf (c, "%s", f);
	} else {    /* type == 3 */
	        /* Check we don't fly off the end of c.  This should
	           never happen, but I've been wrong before. */
	        if (where > n) {
	                strcpy (f, "unknown");
	        } else {
	                /* Get address */
	                sscanf (&c[where], "%s", f);
	        }
	}

	/* Done! */
	return (&f[0]);
}
