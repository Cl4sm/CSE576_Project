void
internal_function
_nl_load_domain (domain_file, domainbinding)
     struct loaded_l10nfile *domain_file;
     struct binding *domainbinding;
{
  int fd;
  size_t size;
#ifdef _LIBC
  struct stat64 st;
#else
  struct stat st;
#endif
  struct mo_file_header *data = (struct mo_file_header *) -1;
  int use_mmap = 0;
  struct loaded_domain *domain;
  int revision;
  const char *nullentry;

  domain_file->decided = 1;
  domain_file->data = NULL;

  /* Note that it would be useless to store domainbinding in domain_file
     because domainbinding might be == NULL now but != NULL later (after
     a call to bind_textdomain_codeset).  */

  /* If the record does not represent a valid locale the FILENAME
     might be NULL.  This can happen when according to the given
     specification the locale file name is different for XPG and CEN
     syntax.  */
  if (domain_file->filename == NULL)
    return;

  /* Try to open the addressed file.  */
  fd = open (domain_file->filename, O_RDONLY | O_BINARY);
  if (fd == -1)
    return;

  /* We must know about the size of the file.  */
  if (
#ifdef _LIBC
      __builtin_expect (fstat64 (fd, &st) != 0, 0)
#else
      __builtin_expect (fstat (fd, &st) != 0, 0)
#endif
      || __builtin_expect ((size = (size_t) st.st_size) != st.st_size, 0)
      || __builtin_expect (size < sizeof (struct mo_file_header), 0))
    {
      /* Something went wrong.  */
      close (fd);
      return;
    }

#ifdef HAVE_MMAP
  /* Now we are ready to load the file.  If mmap() is available we try
     this first.  If not available or it failed we try to load it.  */
  data = (struct mo_file_header *) mmap (NULL, size, PROT_READ,
					 MAP_PRIVATE, fd, 0);

  if (__builtin_expect (data != (struct mo_file_header *) -1, 1))
    {
      /* mmap() call was successful.  */
      close (fd);
      use_mmap = 1;
    }
#endif

  /* If the data is not yet available (i.e. mmap'ed) we try to load
     it manually.  */
  if (data == (struct mo_file_header *) -1)
    {
      size_t to_read;
      char *read_ptr;

      data = (struct mo_file_header *) malloc (size);
      if (data == NULL)
	return;

      to_read = size;
      read_ptr = (char *) data;
      do
	{
	  long int nb = (long int) read (fd, read_ptr, to_read);
	  if (nb <= 0)
	    {
#ifdef EINTR
	      if (nb == -1 && errno == EINTR)
		continue;
#endif
	      close (fd);
	      return;
	    }
	  read_ptr += nb;
	  to_read -= nb;
	}
      while (to_read > 0);

      close (fd);
    }

  /* Using the magic number we can test whether it really is a message
     catalog file.  */
  if (__builtin_expect (data->magic != _MAGIC && data->magic != _MAGIC_SWAPPED,
			0))
    {
      /* The magic number is wrong: not a message catalog file.  */
#ifdef HAVE_MMAP
      if (use_mmap)
	munmap ((caddr_t) data, size);
      else
#endif
	free (data);
      return;
    }

  domain = (struct loaded_domain *) malloc (sizeof (struct loaded_domain));
  if (domain == NULL)
    return;
  domain_file->data = domain;

  domain->data = (char *) data;
  domain->use_mmap = use_mmap;
  domain->mmap_size = size;
  domain->must_swap = data->magic != _MAGIC;
  domain->malloced = NULL;

  /* Fill in the information about the available tables.  */
  revision = W (domain->must_swap, data->revision);
  /* We support only the major revision 0.  */
  switch (revision >> 16)
    {
    case 0:
      domain->nstrings = W (domain->must_swap, data->nstrings);
      domain->orig_tab = (const struct string_desc *)
	((char *) data + W (domain->must_swap, data->orig_tab_offset));
      domain->trans_tab = (const struct string_desc *)
	((char *) data + W (domain->must_swap, data->trans_tab_offset));
      domain->hash_size = W (domain->must_swap, data->hash_tab_size);
      domain->hash_tab =
	(domain->hash_size > 2
	 ? (const nls_uint32 *)
	   ((char *) data + W (domain->must_swap, data->hash_tab_offset))
	 : NULL);
      domain->must_swap_hash_tab = domain->must_swap;

      /* Now dispatch on the minor revision.  */
      switch (revision & 0xffff)
	{
	case 0:
	  domain->n_sysdep_strings = 0;
	  domain->orig_sysdep_tab = NULL;
	  domain->trans_sysdep_tab = NULL;
	  break;
	case 1:
	default:
	  {
	    nls_uint32 n_sysdep_strings;

	    if (domain->hash_tab == NULL)
	      /* This is invalid.  These minor revisions need a hash table.  */
	      goto invalid;

	    n_sysdep_strings =
	      W (domain->must_swap, data->n_sysdep_strings);
	    if (n_sysdep_strings > 0)
	      {
		nls_uint32 n_sysdep_segments;
		const struct sysdep_segment *sysdep_segments;
		const char **sysdep_segment_values;
		const nls_uint32 *orig_sysdep_tab;
		const nls_uint32 *trans_sysdep_tab;
		size_t memneed;
		char *mem;
		struct sysdep_string_desc *inmem_orig_sysdep_tab;
		struct sysdep_string_desc *inmem_trans_sysdep_tab;
		nls_uint32 *inmem_hash_tab;
		unsigned int i;

		/* Get the values of the system dependent segments.  */
		n_sysdep_segments =
		  W (domain->must_swap, data->n_sysdep_segments);
		sysdep_segments = (const struct sysdep_segment *)
		  ((char *) data
		   + W (domain->must_swap, data->sysdep_segments_offset));
		sysdep_segment_values =
		  alloca (n_sysdep_segments * sizeof (const char *));
		for (i = 0; i < n_sysdep_segments; i++)
		  {
		    const char *name =
		      (char *) data
		      + W (domain->must_swap, sysdep_segments[i].offset);
		    nls_uint32 namelen =
		      W (domain->must_swap, sysdep_segments[i].length);

		    if (!(namelen > 0 && name[namelen - 1] == '\0'))
		      {
			freea (sysdep_segment_values);
			goto invalid;
		      }

		    sysdep_segment_values[i] = get_sysdep_segment_value (name);
		  }

		orig_sysdep_tab = (const nls_uint32 *)
		  ((char *) data
		   + W (domain->must_swap, data->orig_sysdep_tab_offset));
		trans_sysdep_tab = (const nls_uint32 *)
		  ((char *) data
		   + W (domain->must_swap, data->trans_sysdep_tab_offset));

		/* Compute the amount of additional memory needed for the
		   system dependent strings and the augmented hash table.  */
		memneed = 2 * n_sysdep_strings
			  * sizeof (struct sysdep_string_desc)
			  + domain->hash_size * sizeof (nls_uint32);
		for (i = 0; i < 2 * n_sysdep_strings; i++)
		  {
		    const struct sysdep_string *sysdep_string =
		      (const struct sysdep_string *)
		      ((char *) data
		       + W (domain->must_swap,
			    i < n_sysdep_strings
			    ? orig_sysdep_tab[i]
			    : trans_sysdep_tab[i - n_sysdep_strings]));
		    size_t need = 0;
		    const struct segment_pair *p = sysdep_string->segments;

		    if (W (domain->must_swap, p->sysdepref) != SEGMENTS_END)
		      for (p = sysdep_string->segments;; p++)
			{
			  nls_uint32 sysdepref;

			  need += W (domain->must_swap, p->segsize);

			  sysdepref = W (domain->must_swap, p->sysdepref);
			  if (sysdepref == SEGMENTS_END)
			    break;

			  if (sysdepref >= n_sysdep_segments)
			    {
			      /* Invalid.  */
			      freea (sysdep_segment_values);
			      goto invalid;
			    }

			  need += strlen (sysdep_segment_values[sysdepref]);
			}

		    memneed += need;
		  }

		/* Allocate additional memory.  */
		mem = (char *) malloc (memneed);
		if (mem == NULL)
		  goto invalid;

		domain->malloced = mem;
		inmem_orig_sysdep_tab = (struct sysdep_string_desc *) mem;
		mem += n_sysdep_strings * sizeof (struct sysdep_string_desc);
		inmem_trans_sysdep_tab = (struct sysdep_string_desc *) mem;
		mem += n_sysdep_strings * sizeof (struct sysdep_string_desc);
		inmem_hash_tab = (nls_uint32 *) mem;
		mem += domain->hash_size * sizeof (nls_uint32);

		/* Compute the system dependent strings.  */
		for (i = 0; i < 2 * n_sysdep_strings; i++)
		  {
		    const struct sysdep_string *sysdep_string =
		      (const struct sysdep_string *)
		      ((char *) data
		       + W (domain->must_swap,
			    i < n_sysdep_strings
			    ? orig_sysdep_tab[i]
			    : trans_sysdep_tab[i - n_sysdep_strings]));
		    const char *static_segments =
		      (char *) data
		      + W (domain->must_swap, sysdep_string->offset);
		    const struct segment_pair *p = sysdep_string->segments;

		    /* Concatenate the segments, and fill
		       inmem_orig_sysdep_tab[i] (for i < n_sysdep_strings) and
		       inmem_trans_sysdep_tab[i-n_sysdep_strings] (for
		       i >= n_sysdep_strings).  */

		    if (W (domain->must_swap, p->sysdepref) == SEGMENTS_END)
		      {
			/* Only one static segment.  */
			inmem_orig_sysdep_tab[i].length =
			  W (domain->must_swap, p->segsize);
			inmem_orig_sysdep_tab[i].pointer = static_segments;
		      }
		    else
		      {
			inmem_orig_sysdep_tab[i].pointer = mem;

			for (p = sysdep_string->segments;; p++)
			  {
			    nls_uint32 segsize =
			      W (domain->must_swap, p->segsize);
			    nls_uint32 sysdepref =
			      W (domain->must_swap, p->sysdepref);
			    size_t n;

			    if (segsize > 0)
			      {
				memcpy (mem, static_segments, segsize);
				mem += segsize;
				static_segments += segsize;
			      }

			    if (sysdepref == SEGMENTS_END)
			      break;

			    n = strlen (sysdep_segment_values[sysdepref]);
			    memcpy (mem, sysdep_segment_values[sysdepref], n);
			    mem += n;
			  }

			inmem_orig_sysdep_tab[i].length =
			  mem - inmem_orig_sysdep_tab[i].pointer;
		      }
		  }

		/* Compute the augmented hash table.  */
		for (i = 0; i < domain->hash_size; i++)
		  inmem_hash_tab[i] =
		    W (domain->must_swap_hash_tab, domain->hash_tab[i]);
		for (i = 0; i < n_sysdep_strings; i++)
		  {
		    const char *msgid = inmem_orig_sysdep_tab[i].pointer;
		    nls_uint32 hash_val = hash_string (msgid);
		    nls_uint32 idx = hash_val % domain->hash_size;
		    nls_uint32 incr = 1 + (hash_val % (domain->hash_size - 2));

		    for (;;)
		      {
			if (inmem_hash_tab[idx] == 0)
			  {
			    /* Hash table entry is empty.  Use it.  */
			    inmem_hash_tab[idx] = 1 + domain->nstrings + i;
			    break;
			  }

			if (idx >= domain->hash_size - incr)
			  idx -= domain->hash_size - incr;
			else
			  idx += incr;
		      }
		  }

		freea (sysdep_segment_values);

		domain->n_sysdep_strings = n_sysdep_strings;
		domain->orig_sysdep_tab = inmem_orig_sysdep_tab;
		domain->trans_sysdep_tab = inmem_trans_sysdep_tab;

		domain->hash_tab = inmem_hash_tab;
		domain->must_swap_hash_tab = 0;
	      }
	    else
	      {
		domain->n_sysdep_strings = 0;
		domain->orig_sysdep_tab = NULL;
		domain->trans_sysdep_tab = NULL;
	      }
	  }
	  break;
	}
      break;
    default:
      /* This is an invalid revision.  */
    invalid:
      /* This is an invalid .mo file.  */
      if (domain->malloced)
	free (domain->malloced);
#ifdef HAVE_MMAP
      if (use_mmap)
	munmap ((caddr_t) data, size);
      else
#endif
	free (data);
      free (domain);
      domain_file->data = NULL;
      return;
    }

  /* Now initialize the character set converter from the character set
     the file is encoded with (found in the header entry) to the domain's
     specified character set or the locale's character set.  */
  nullentry = _nl_init_domain_conv (domain_file, domain, domainbinding);

  /* Also look for a plural specification.  */
  EXTRACT_PLURAL_EXPRESSION (nullentry, &domain->plural, &domain->nplurals);
}
