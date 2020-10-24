char *
tildepath(path)
char *path;		/* Path starting with ~ */
{
    /*
     * Local Variables
     */

    char *username;	/* Points to the username */
    char *rest;		/* Rest of input path */
    char *result;	/* Used to construct the result */
    struct passwd *pwent; /* Password entry for user */
    int size;		/* Length of new path */

    /*
     * Functions
     */

    /*    char *strdup();	*/	/* Make a copy of a path */

    /*
     * If the path doesn't start with ~ quit right now
     */

    if (path[0] != '~')
	return(NULL);

    /*
     * Find the name delimited by / or the end of the string
     */

    if ((username = strdup(++path)) == NULL)
	return(NULL);
    if ((rest = strchr(username, '/'))) {
	*rest = '\0';
	rest++;
    }

    /*
     * Get the user's password entry
     */

    if (username[0])
	pwent = getpwnam(username);
    else
	pwent = getpwuid(getuid());

    /*
     * Check to see if a password entry was found
     */

    if (pwent == NULL) {
	free(username);	
	return(NULL);
    }

    /*
     * Determine the size of the new path and allocate space for it
     */

    size = strlen(pwent->pw_dir) + 1;
    if (rest && (rest[0] != '\0'))
	size += strlen(rest) + 1;
    if ((result = calloc(size, sizeof(char))) == NULL) {
	free(username);
	return(NULL);
    }

    /*
     * Copy over the new path
     */

    strcpy(result, pwent->pw_dir);
    if (rest && (rest[0] != '\0')) {
	strcat(result, "/");
	strcat(result, rest);
    }

    /*
     * Done
     */

    free(username);
    return(result);
}
