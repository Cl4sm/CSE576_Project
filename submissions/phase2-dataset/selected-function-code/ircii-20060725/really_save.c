static	void
really_save(file, line)
	u_char	*file;
	u_char	*line;
{
	FILE	*fp;

	if (*line != 'y' && *line != 'Y')
		return;
	if ((fp = fopen(CP(file), "w")) != NULL)
	{
		if (save_which & SFLAG_BIND)
			save_bindings(fp, save_do_all);
		if (save_which & SFLAG_ON)
			save_hooks(fp, save_do_all);
		if (save_which & SFLAG_NOTIFY)
			save_notify(fp);
		if (save_which & SFLAG_SET)
			save_variables(fp, save_do_all);
		if (save_which & SFLAG_ALIAS)
			save_aliases(fp, save_do_all);
		if (save_which & SFLAG_DIGRAPH)
			save_digraphs(fp);
		fclose(fp);
		say("IRCII settings saved to %s", file);
	}
	else
		say("Error opening %s: %s", file, strerror(errno));
}
