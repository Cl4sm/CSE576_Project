static void *alignedrealloc(void *old, size_t oldsize, size_t size, int *res)
{
#if _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
	void *new;
	/* Shortcut resizing */
	if (size < oldsize)
		return old;
	*res = posix_memalign(&new, 4096, size);
	if (*res != 0) {
		fprintf(stderr, "Error aligning IO buffer: %d\n", *res);
		
		return NULL;
	}
	assert(oldsize<size);
	memcpy(new,old,oldsize);
	free(old);
	return new;
#else
	/* These no-ops are to stop the compiler whinging about unused
	 * parameters */
	(void)oldsize;
	(void)res;
	return realloc(old,size);
#endif
}