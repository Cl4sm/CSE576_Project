char **hfs_glob(hfsvol *vol, int argc, char *argv[], int *nelts)
{
  dlist list;
  int i;

  if (dl_init(&list) == -1)
    return 0;

  for (i = 0; i < argc; ++i)
    {
      if (doglob(vol, &list, "", argv[i]) == -1)
	{
	  dl_free(&list);
	  return 0;
	}
    }

  *nelts = dl_size(&list);

  return dl_array(&list);
}
