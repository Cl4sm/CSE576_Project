gather_func (const char *name, const struct stat64 *st, int type,
	     struct FTW *ftwp)
{
  unsigned char e_ident [sizeof (Elf64_Ehdr) + sizeof (Elf64_Phdr)];

#ifndef HAVE_FTW_ACTIONRETVAL
  if (blacklist_dir)
    {
      if (strncmp (name, blacklist_dir, blacklist_dir_len) == 0)
	return FTW_CONTINUE;
      free (blacklist_dir);
      blacklist_dir = NULL;
    }
#endif
  if (type == FTW_F && S_ISREG (st->st_mode) && (st->st_mode & 0111))
    {
      int fd, i;
      DSO *dso;
      struct prelink_entry *ent;
      size_t len = strlen (name);
      const char *base = NULL;

      for (i = 0; i < blacklist_next; ++i)
	if (blacklist_ext[i].is_glob)
	  {
	    if (base == NULL)
	      {
		base = strrchr (name, '/');
		if (base == NULL)
		  base = name;
		else
		  ++base;
	      }
	    if (fnmatch (blacklist_ext[i].ext, base, FNM_PERIOD) == 0)
	      return FTW_CONTINUE;
	  }
	else if (blacklist_ext[i].len <= len
		 && memcmp (name + len - blacklist_ext[i].len,
			    blacklist_ext[i].ext, blacklist_ext[i].len) == 0)
	  return FTW_CONTINUE;

      ent = prelink_find_entry (name, st, 0);
      if (ent != NULL && ent->type != ET_NONE)
	{
	  if (verbose > 5)
	    {
	      if (ent->type == ET_CACHE_EXEC || ent->type == ET_CACHE_DYN)
		printf ("Assuming prelinked %s\n", name);
	      if (ent->type == ET_UNPRELINKABLE)
		printf ("Assuming non-prelinkable %s\n", name);
	    }
	  ent->u.explicit = 1;
	  return FTW_CONTINUE;
	}

      if (st->st_size < sizeof (e_ident))
	return FTW_CONTINUE;

      fd = open (name, O_RDONLY);
      if (fd == -1)
	return FTW_CONTINUE;

      if (read (fd, e_ident, sizeof (e_ident)) != sizeof (e_ident))
	{
close_it:
	  close (fd);
	  return FTW_CONTINUE;
	}

      /* Quickly find ET_EXEC ELF binaries and most of PIE binaries.  */

      if (memcmp (e_ident, ELFMAG, SELFMAG) != 0)
	{
make_unprelinkable:
	  if (! undo)
	    {
	      ent = prelink_find_entry (name, st, 1);
	      if (ent != NULL)
		{
		  assert (ent->type == ET_NONE);
		  ent->type = ET_UNPRELINKABLE;
		}
	    }
	  close (fd);
	  return FTW_CONTINUE;
	}

      switch (e_ident [EI_DATA])
	{
	case ELFDATA2LSB:
	  if (e_ident [EI_NIDENT + 1] != 0)
	    goto make_unprelinkable;
	  if (e_ident [EI_NIDENT] != ET_EXEC)
	    {
	      if (e_ident [EI_NIDENT] != ET_DYN)
		goto make_unprelinkable;
	      else if (e_ident [EI_CLASS] == ELFCLASS32)
		{
		  if (e_ident [offsetof (Elf32_Ehdr, e_phoff)]
		      == sizeof (Elf32_Ehdr)
		      && memcmp (e_ident + offsetof (Elf32_Ehdr, e_phoff) + 1,
				 "\0\0\0", 3) == 0)
		    {
		      Elf32_Half phnum, i;
		      unsigned char *p;
		      phnum = e_ident [offsetof (Elf32_Ehdr, e_phnum)]
			      + (e_ident [offsetof (Elf32_Ehdr, e_phnum) + 1]
				 << 8);
		      p = e_ident + sizeof (Elf32_Ehdr)
			  + offsetof (Elf32_Phdr, p_type);
		      for (i = 0; i < phnum; i++, p += sizeof (Elf32_Phdr))
			{
			  if (p[0] == PT_PHDR
			      && memcmp (p + 1, "\0\0\0", 3) == 0)
			    {
maybe_pie:
			      dso = fdopen_dso (fd, name);
			      if (dso == NULL)
				goto close_it;
			      if (dynamic_info_is_set (dso, DT_DEBUG))
				{
				  close_dso (dso);
				  goto make_unprelinkable;
				}
			      close_dso (dso);
			      goto close_it;
			    }
			  if (p[3] < (PT_LOPROC >> 24)
			      || p[3] > (PT_HIPROC >> 24))
			    break;
			}
		    }
		  goto close_it;
		}
	      else if (e_ident [EI_CLASS] == ELFCLASS64)
		{
		  if (e_ident [offsetof (Elf64_Ehdr, e_phoff)]
		      == sizeof (Elf64_Ehdr)
		      && memcmp (e_ident + offsetof (Elf64_Ehdr, e_phoff) + 1,
				 "\0\0\0\0\0\0\0", 7) == 0)
		    {
		      Elf64_Half phnum, i;
		      unsigned char *p;
		      phnum = e_ident [offsetof (Elf64_Ehdr, e_phnum)]
			      + (e_ident [offsetof (Elf64_Ehdr, e_phnum) + 1]
				 << 8);
		      p = e_ident + sizeof (Elf64_Ehdr)
			  + offsetof (Elf64_Phdr, p_type);
		      for (i = 0; i < phnum; i++, p += sizeof (Elf64_Phdr))
			{
			  if (p[0] == PT_PHDR
			      && memcmp (p + 1, "\0\0\0", 3) == 0)
			    goto maybe_pie;
			  if (p[3] < (PT_LOPROC >> 24)
			      || p[3] > (PT_HIPROC >> 24))
			    break;
			}
		    }
		  goto close_it;
		}
	      else
		goto make_unprelinkable;
	    }
	  break;
	case ELFDATA2MSB:
	  if (e_ident [EI_NIDENT] != 0)
	    goto make_unprelinkable;
	  if (e_ident [EI_NIDENT + 1] != ET_EXEC)
	    {
	      if (e_ident [EI_NIDENT + 1] != ET_DYN)
		goto make_unprelinkable;
	      else if (e_ident [EI_CLASS] == ELFCLASS32)
		{
		  if (e_ident [offsetof (Elf32_Ehdr, e_phoff) + 3]
		      == sizeof (Elf32_Ehdr)
		      && memcmp (e_ident + offsetof (Elf32_Ehdr, e_phoff),
				 "\0\0\0", 3) == 0)
		    {
		      Elf32_Half phnum, i;
		      unsigned char *p;
		      phnum = (e_ident [offsetof (Elf32_Ehdr, e_phnum)] << 8)
			      + e_ident [offsetof (Elf32_Ehdr, e_phnum) + 1];
		      p = e_ident + sizeof (Elf32_Ehdr)
			  + offsetof (Elf32_Phdr, p_type);
		      for (i = 0; i < phnum; i++, p += sizeof (Elf32_Phdr))
			{
			  if (p[3] == PT_PHDR
			      && memcmp (p, "\0\0\0", 3) == 0)
			    goto maybe_pie;
			  if (p[0] < (PT_LOPROC >> 24)
			      || p[0] > (PT_HIPROC >> 24))
			    break;
			}
		    }
		  goto close_it;
		}
	      else if (e_ident [EI_CLASS] == ELFCLASS64)
		{
		  if (e_ident [offsetof (Elf64_Ehdr, e_phoff) + 7]
		      == sizeof (Elf64_Ehdr)
		      && memcmp (e_ident + offsetof (Elf64_Ehdr, e_phoff),
				 "\0\0\0\0\0\0\0", 7) == 0)
		    {
		      Elf64_Half phnum, i;
		      unsigned char *p;
		      phnum = (e_ident [offsetof (Elf64_Ehdr, e_phnum)] << 8)
			      + e_ident [offsetof (Elf64_Ehdr, e_phnum) + 1];
		      p = e_ident + sizeof (Elf64_Ehdr)
			  + offsetof (Elf64_Phdr, p_type);
		      for (i = 0; i < phnum; i++, p += sizeof (Elf64_Phdr))
			{
			  if (p[3] == PT_PHDR
			      && memcmp (p, "\0\0\0", 3) == 0)
			    goto maybe_pie;
			  if (p[0] < (PT_LOPROC >> 24)
			      || p[0] > (PT_HIPROC >> 24))
			    break;
			}
		    }
		  goto close_it;
		}
	      else
		goto make_unprelinkable;
	    }
	  break;
	default:
	  goto make_unprelinkable;
	}

      dso = fdopen_dso (fd, name);
      if (dso == NULL)
	return FTW_CONTINUE;

      gather_exec (dso, st);
    }
  else if (type == FTW_D)
    switch (add_dir_to_dirlist (name, st->st_dev, FTW_CHDIR))
      {
      case 0: return FTW_CONTINUE;
      default: return FTW_STOP;
      case 2:
#ifdef HAVE_FTW_ACTIONRETVAL
	return FTW_SKIP_SUBTREE;
#else
	{
	  blacklist_dir_len = strlen (name) + 1;
	  if (blacklist_dir_len > 1 && name[blacklist_dir_len - 2] == '/')
	    blacklist_dir_len--;
	  blacklist_dir = malloc (blacklist_dir_len + 1);
	  if (blacklist_dir == NULL)
	    {
	      error (0, ENOMEM, "Cannot store blacklisted dir name");
	      return FTW_STOP;
	    }
	  memcpy (blacklist_dir, name, blacklist_dir_len - 1);
	  blacklist_dir[blacklist_dir_len - 1] = '/';
	  blacklist_dir[blacklist_dir_len] = '\0';
	  return FTW_CONTINUE;
	}
#endif
      }

  return FTW_CONTINUE;
}
