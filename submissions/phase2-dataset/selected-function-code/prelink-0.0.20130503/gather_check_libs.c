gather_check_libs (void)
{
  struct prelink_dir *dir;
  void *f;

  htab_traverse (prelink_filename_htab, gather_check_lib, NULL);

  dir = dirs;
  while (dir != NULL)
    {
      f = dir;
      dir = dir->next;
      free (f);
    }

  dir = blacklist;
  while (dir != NULL)
    {
      f = dir;
      dir = dir->next;
      free (f);
    }

  dirs = NULL;
  blacklist = NULL;
  return 0;
}
