void
new_free(iptr)
	void	*iptr;
{
	void	**ptr = (void **) iptr;
#ifdef ALLOC_DEBUG
	FILE	*fp;
	int	i;
#endif /* ALLOC_DEBUG */
#ifdef DO_USER2
	int	oldmask;
#endif /* DO_USER2 */

	/* cheap hack. */
	if (*ptr == empty_string || *ptr == zero || *ptr == one)
		*ptr = (void *) 0;
	else if (*ptr)
	{
#ifdef DO_USER2
		oldmask = sigblock(sigmask(SIGUSR2));
#endif /* DO_USER2 */
#ifdef FREE_DEBUG
		if (free(*ptr) < 0)
			put_it("*debug* free failed '%s'", (u_char *) ptr);
#else
		free(*ptr);
#endif /* FREE_DEBUG */
#ifdef DO_USER2
		sigblock(oldmask);
#endif /* DO_USER2 */
#ifdef ALLOC_DEBUG
		for (i = 0; i < ALLOC_LIST; i++)
		{
			if ((void *) MemList[i] == *ptr)
				break;
		}
		if (i == ALLOC_LIST)
		{
			fprintf(stderr,
				"Memory freed that was never allocated\n");
			fp=fopen("debug.log", "w");
			fprintf(fp, "failed by freeing %08lx\n", (long) *ptr);
			fprintf(fp, "List is as follows:\n");
			for (i = 0; i < ALLOC_LIST; i++)
				if (MemList[i])
					fprintf(fp, "    %08lx  %08lx\n",
						(long) MemList[i], MemSize[i]);
			fclose(fp);
			abort();
		}
		MemList[i] = (void *) 0;
		MemSize[i] = 0L;
#endif /* ALLOC_DEBUG */
		*ptr = (void *) 0;
	}
}
