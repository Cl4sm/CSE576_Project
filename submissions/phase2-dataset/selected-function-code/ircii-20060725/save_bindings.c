void
save_bindings(fp, do_all)
	FILE	*fp;
	int	do_all;
{
	int	i;
	int	charsize = charset_size();

	for (i = 0; i < charsize; i++)
		write_binding((u_char) i, 0, fp, do_all);
	for (i = 0; i < charsize; i++)
		write_binding((u_char) i, 1, fp, do_all);
	for (i = 0; i < charsize; i++)
		write_binding((u_char) i, 2, fp, do_all);
	for (i = 0; i < charsize; i++)
		write_binding((u_char) i, 3, fp, do_all);
	for (i = 0; i < charsize; i++)
		write_binding((u_char) i, 4, fp, do_all);
	for (i = 0; i < charsize; i++)
		write_binding((u_char) i, 5, fp, do_all);
	for (i = 0; i < charsize; i++)
		write_binding((u_char) i, 6, fp, do_all);
	for (i = 0; i < charsize; i++)
		write_binding((u_char) i, 7, fp, do_all);
	for (i = 0; i < charsize; i++)
		write_binding((u_char) i, 8, fp, do_all);
}
