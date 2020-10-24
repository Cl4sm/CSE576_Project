int
scandir(Name, dirlist, Selector, Sorter)
#if defined(__linux__) || defined(__sgi)
	const char	*Name;
#else
	char		*Name;
#endif /* __linux__ || __sgi */
	struct dirent	***dirlist;
	int		(*Selector)();
	int		(*Sorter)();
{
	static struct dirent *E;
	struct dirent	**names;
	DIR		*Dp;
	int		i;
	int		size = INITIAL_SIZE;

	if (!(names = (struct dirent **) new_malloc(size * sizeof names[0])) ||
	     access(Name, R_OK | X_OK) || !(Dp = opendir(Name)))
		return -1;

	/* Read entries in the directory. */

	for (i = 0; (E = readdir(Dp)); )
		if (Selector == NULL || (*Selector)(E))
	{
		/* User wants them all, or he wants this one. */
		if (++i >= size)
		{
			size <<= 1;
			names = (struct dirent **) new_realloc(UP(names),
						size * sizeof names[0]);
			if (names == NULL)
			{
				closedir(Dp);
				new_free(&names);
				return(-1);
			}
		}

		/* Copy the entry. */
		names[i - 1] = (struct dirent *) new_malloc(DIRSIZ(E));
		if (names[i - 1] == NULL)
		{ 
			closedir(Dp);
			new_free(&names);
			return(-1);
		}
#ifndef __QNX__
		names[i - 1]->d_ino = E->d_ino;
		names[i - 1]->d_reclen = E->d_reclen;
#endif /* !__QNX__ */
		my_strcpy(names[i - 1]->d_name, E->d_name);
	}

	/* Close things off. */
	names = (struct dirent **) new_realloc(UP(names),
					(i + 1) * sizeof names[0]);
	names[i] = 0;
	*dirlist = names;
	closedir(Dp);

	/* Sort? */
	if (i && Sorter)
		qsort((char *)names, i, sizeof names[0], Sorter);

	return i;
}
