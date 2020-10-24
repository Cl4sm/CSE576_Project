void
add_blacklist_ext (const char *ext)
{
  blacklist_ext = realloc (blacklist_ext,
			   (blacklist_next + 1) * sizeof (*blacklist_ext));
  if (blacklist_ext == NULL)
    error (EXIT_FAILURE, errno, "can't create blacklist extension list");
  if (*ext == '*' && strpbrk (ext + 1, "*?[{") == NULL)
    {
      blacklist_ext[blacklist_next].is_glob = 0;
      ext++;
    }
  else
    blacklist_ext[blacklist_next].is_glob = 1;
  blacklist_ext[blacklist_next].ext = strdup (ext);
  if (blacklist_ext[blacklist_next].ext == NULL)
    error (EXIT_FAILURE, errno, "can't create blacklist extension list");
  blacklist_ext[blacklist_next].len = strlen (ext);
  blacklist_next++;
}
