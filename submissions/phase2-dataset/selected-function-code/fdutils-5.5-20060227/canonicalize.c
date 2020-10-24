canonicalize (const char *path)
{
    char *canonical = xmalloc (PATH_MAX + 1);
    char *p = canonical;
  
    if (path == NULL)
	return NULL;

#if 0
    if (!strcmp (path, "none"))
	{
	    strncpy (canonical, path, sizeof(canonical)-1);
	    canonical[sizeof(canonical)-1] = '\0';
	    return canonical;
	}
    if (strchr (path, ':') != NULL)
	{
	    strncpy(canonical, path, sizeof(canonical)-1);
	    canonical[sizeof(canonical)-1] = '\0';
	    return canonical;
	}
#endif

    if (*path == '/')
	{
	    /* We've already got an absolute path in PATH, but we need at
	       least one char in canonical[] on entry to the loop below.  */
	    *p = *path++;
	}
    else
	{
	    getcwd (canonical, PATH_MAX);
	    p = canonical + strlen (canonical) - 1;
	    if (*p != '/')
		*++p = '/';
	}
  
    /* There is at least one character in canonical[],
       and the last char in canonical[], *p, is '/'.  */
    while ((*path != '\0') && (p < canonical + PATH_MAX))
	if (*p != '/')
	    {
		*++p = *path++;
	    }
	else
	    {
		if (path[0] == '/')
		    {
			path++;		/* eliminate duplicate slashes (``//'') */
		    }
		else if ((path[0] == '.') && ((path[1] == '\0') || (path[1] == '/')))
		    {
			path++;		/* eliminate ``/.'' */
		    }
		else if ((path[0] == '.') && (path[1] == '.')
			 && ((path[2] == '\0') || (path[2] == '/')))
		    {
			while ((p > canonical) && (*--p != '/'))
			    /* ascend on ``/../'' */
			    ;
			path += 2;
		    }
		else
		    {
			*++p = *path++;	/* not a special case, just copy char */
		    }
	    }
    if (p >= (canonical + PATH_MAX))
	die ("path too long");

    if (*p == '/')
	--p;			/* eliminate trailing slash */

    *++p = '\0';
  
    return canonical;
}
