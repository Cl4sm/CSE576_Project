{
	if (orig == newfile)
		return 1;
	if (!orig)
		return 0;
	if (orig->size == newfile->size && (orig->path || orig->uncompressed))
	{
		map_entry(orig);
		map_entry(newfile);
		if (!memcmp(orig->uncompressed, newfile->uncompressed, orig->size))
		{
			newfile->same = orig;
			unmap_entry(newfile);
			unmap_entry(orig);
			return 1;
		}
		unmap_entry(newfile);
		unmap_entry(orig);
	}
	return (find_identical_file(orig->child, newfile) ||
		find_identical_file(orig->next, newfile));
}
