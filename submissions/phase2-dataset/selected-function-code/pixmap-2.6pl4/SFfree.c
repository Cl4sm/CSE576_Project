static
SFfree(i)
	int	i;
{
	register SFDir	*dir;
	register int	j;

	dir = &(SFdirs[i]);

	for (j = dir->nEntries - 1; j >= 0; j--) {
		if (dir->entries[j].shown != dir->entries[j].real) {
			XtFree(dir->entries[j].shown);
		}
		XtFree(dir->entries[j].real);
	}

	XtFree((char *) dir->entries);

	XtFree(dir->dir);

	dir->dir = NULL;
}
