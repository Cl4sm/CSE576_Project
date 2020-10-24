static int
close_dso_1 (DSO *dso)
{
  if (dso_is_rdwr (dso))
    {
      int i;

      for (i = 1; i < dso->ehdr.e_shnum; ++i)
	{
	  Elf_Scn *scn = dso->scn[i];
	  Elf_Data *data = NULL;

	  while ((data = elf_getdata (scn, data)) != NULL)
	    {
	      free (data->d_buf);
	      data->d_buf = NULL;
	    }
	}
    }

  elf_end (dso->elf);
  if (dso_is_rdwr (dso))
    fsync (dso->fd);
  close (dso->fd);
  if (dso->elfro)
    {
      elf_end (dso->elfro);
      close (dso->fdro);
    }
  if (dso->filename != dso->soname)
    free ((char *) dso->soname);
  free ((char *) dso->filename);
  free ((char *) dso->temp_filename);
  free (dso->move);
  free (dso->adjust);
  free (dso->undo.d_buf);
  free (dso);
  return 0;
}
