reopen_dso (DSO *dso, struct section_move *move, const char *temp_base)
{
  char filename[strlen (temp_base ? temp_base : dso->filename)
		+ sizeof ("/dev/shm/.#prelink#.XXXXXX")];
  int adddel = 0;
  int free_move = 0;
  Elf *elf = NULL;
  GElf_Ehdr ehdr;
  char *e_ident;
  int fd, i, j;

  if (move == NULL)
    {
      move = init_section_move (dso);
      if (move == NULL)
	return 1;
      free_move = 1;
    }
  else
    assert (dso->ehdr.e_shnum == move->old_shnum);

  if (temp_base == NULL)
    temp_base = dso->filename;
  sprintf (filename, "%s.#prelink#.XXXXXX", temp_base);

  fd = mkstemp (filename);
  if (fd == -1)
    {
      strcpy (filename, "/tmp/#prelink#.XXXXXX");
      fd = mkstemp (filename);
      if (fd == -1)
	{
	  strcpy (filename, "/dev/shm/#prelink#.XXXXXX");
	  fd = mkstemp (filename);
	}
      if (fd == -1)
	{
	  error (0, errno, "Could not create temporary file %s", filename);
	  goto error_out;
	}
    }

  elf = elf_begin (fd, ELF_C_WRITE, NULL);
  if (elf == NULL)
    {
      error (0, 0, "cannot open ELF file: %s", elf_errmsg (-1));
      goto error_out;

    }

  /* Some gelf_newehdr implementations don't return the resulting
     ElfNN_Ehdr, so we have to do it the hard way instead of:
     e_ident = (char *) gelf_newehdr (elf, gelf_getclass (dso->elf));  */
  switch (gelf_getclass (dso->elf))
    {
    case ELFCLASS32:
      e_ident = (char *) elf32_newehdr (elf);
      break;
    case ELFCLASS64:
      e_ident = (char *) elf64_newehdr (elf);
      break;
    default:
      e_ident = NULL;
      break;
    }

  if (e_ident == NULL
      /* This is here just for the gelfx wrapper, so that gelf_update_ehdr
	 already has the correct ELF class.  */
      || memcpy (e_ident, dso->ehdr.e_ident, EI_NIDENT) == NULL
      || gelf_update_ehdr (elf, &dso->ehdr) == 0
      || gelf_newphdr (elf, dso->ehdr.e_phnum) == 0)
    {
      error (0, 0, "Could not create new ELF headers");
      goto error_out;
    }
  ehdr = dso->ehdr;
  elf_flagelf (elf, ELF_C_SET, ELF_F_LAYOUT | ELF_F_PERMISSIVE);
  for (i = 0; i < ehdr.e_phnum; ++i)
    gelf_update_phdr (elf, i, dso->phdr + i);

  for (i = 1; i < move->new_shnum; ++i)
    {
      Elf_Scn *scn;
      Elf_Data data, *data1, *data2;

      if (move->new_to_old[i] == -1)
	{
	  scn = elf_newscn (elf);
	  elf_newdata (scn);
	}
      else
	{
	  j = move->new_to_old[i];
	  scn = elf_newscn (elf);
	  gelfx_update_shdr (elf, scn, &dso->shdr[j]);
	  if (dso->shdr[j].sh_type == SHT_NOBITS)
	    {
	       data1 = elf_getdata (dso->scn[j], NULL);
	       data2 = elf_newdata (scn);
	       memcpy (data2, data1, sizeof (*data1));
	    }
	  else
	    {
	      memset (&data, 0, sizeof data);
	      data.d_type = ELF_T_NUM;
	      data1 = NULL;
	      while ((data1 = elf_getdata (dso->scn[j], data1))
		     != NULL)
		{
		  if (data.d_type == ELF_T_NUM)
		    data = *data1;
		  else if (data.d_type != data1->d_type
			   || data.d_version != data1->d_version)
		    abort ();
		  else
		    {
		      if (data1->d_off < data.d_off)
			{
			  data.d_size += data.d_off - data1->d_off;
			  data.d_off = data1->d_off;
			}
		      if (data1->d_off + data1->d_size
			  > data.d_off + data.d_size)
			data.d_size = data1->d_off + data1->d_size
				      - data.d_off;
		      if (data1->d_align > data.d_align)
			data.d_align = data1->d_align;
		    }
		}
	      if (data.d_type == ELF_T_NUM)
		{
		  assert (dso->shdr[j].sh_size == 0);
		  continue;
		}
	      if (data.d_size != 0)
		{
		  data.d_buf = calloc (1, data.d_size);
		  if (data.d_buf == NULL)
		    {
		      error (0, ENOMEM, "%s: Could not copy section",
			     dso->filename);
		      goto error_out;
		    }
		}
	      else
		data.d_buf = NULL;
	      data1 = NULL;
	      while ((data1 = elf_getdata (dso->scn[j], data1))
		     != NULL)
		memcpy (data.d_buf + data1->d_off - data.d_off, data1->d_buf,
			data1->d_size);
	      data2 = elf_newdata (scn);
	      memcpy (data2, &data, sizeof (data));
	    }
	}
    }

  ehdr.e_shnum = move->new_shnum;
  dso->temp_filename = strdup (filename);
  if (dso->temp_filename == NULL)
    {
      error (0, ENOMEM, "%s: Could not save temporary filename", dso->filename);
      goto error_out;
    }
  dso->elfro = dso->elf;
  dso->elf = elf;
  dso->fdro = dso->fd;
  dso->fd = fd;
  dso->ehdr = ehdr;
  dso->lastscn = 0;
  elf = NULL;
  fd = -1;
  for (i = 1; i < move->old_shnum; i++)
    if (move->old_to_new[i] != i)
      {
	adddel = 1;
	break;
      }
  if (! adddel)
    for (i = 1; i < move->new_shnum; i++)
      if (move->new_to_old[i] != i)
	{
	  adddel = 1;
	  break;
	}

  for (i = 1; i < move->new_shnum; i++)
    {
      dso->scn[i] = elf_getscn (dso->elf, i);
      gelfx_getshdr (dso->elf, dso->scn[i], dso->shdr + i);
      if (move->new_to_old[i] == -1)
	continue;
      if (dso->move
	  && (dso->shdr[i].sh_type == SHT_SYMTAB
	      || dso->shdr[i].sh_type == SHT_DYNSYM))
	{
	  if (adjust_symtab_section_indices (dso, i, dso->move->old_shnum,
					     dso->move->old_to_new))
	    goto error_out;
	}
      if (adddel)
	{
	  if (dso->shdr[i].sh_link)
	    {
	      if (dso->shdr[i].sh_link >= move->old_shnum)
		{
		  error (0, 0, "%s: bogus sh_link value %d", dso->filename,
			 dso->shdr[i].sh_link);
		  goto error_out;
		}
	      if (move->old_to_new[dso->shdr[i].sh_link] == -1)
		{
		  error (0, 0, "Section sh_link points to has been removed");
		  goto error_out;
		}
	      dso->shdr[i].sh_link = move->old_to_new[dso->shdr[i].sh_link];
	    }
	  /* Only some section types use sh_info for section index.  */
	  if (dso->shdr[i].sh_info
	      && (dso->shdr[i].sh_type == SHT_REL
		  || dso->shdr[i].sh_type == SHT_RELA
		  || (dso->shdr[i].sh_flags & SHF_INFO_LINK)))
	    {
	      if (dso->shdr[i].sh_info >= move->old_shnum)
		{
		  error (0, 0, "%s: bogus sh_info value %d", dso->filename,
			 dso->shdr[i].sh_info);
		  goto error_out;
		}
	      if (move->old_to_new[dso->shdr[i].sh_info] == -1)
		{
		  error (0, 0, "Section sh_info points to has been removed");
		  goto error_out;
		}
	      dso->shdr[i].sh_info = move->old_to_new[dso->shdr[i].sh_info];
	    }
	  if (dso->shdr[i].sh_type == SHT_SYMTAB
	      || dso->shdr[i].sh_type == SHT_DYNSYM)
	    {
	      if (adjust_symtab_section_indices (dso, i, move->old_shnum,
						 move->old_to_new))
		goto error_out;
	    }
	}
    }

  free (dso->move);
  dso->move = NULL;

  dso->ehdr.e_shstrndx = move->old_to_new[dso->ehdr.e_shstrndx];
  gelf_update_ehdr (dso->elf, &dso->ehdr);

  read_dynamic (dso);

  /* If shoff does not point after last section, we need to adjust the sections
     after it if we added or removed some sections.  */
  if (move->old_shnum != move->new_shnum
      && adjust_dso_nonalloc (dso, 0, dso->ehdr.e_shoff + 1,
			      ((long) move->new_shnum - (long) move->old_shnum)
			      * gelf_fsize (dso->elf, ELF_T_SHDR, 1,
					    EV_CURRENT)))
    goto error_out;

  if (free_move)
    free (move);
  return 0;

error_out:
  if (free_move)
    free (move);
  if (elf)
    elf_end (elf);
  if (fd != -1)
    {
      unlink (filename);
      close (fd);
    }
  return 1;
}
