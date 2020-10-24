set_binding_values (domainname, dirnamep, codesetp)
     const char *domainname;
     const char **dirnamep;
     const char **codesetp;
{
  struct binding *binding;
  int modified;

  /* Some sanity checks.  */
  if (domainname == NULL || domainname[0] == '\0')
    {
      if (dirnamep)
	*dirnamep = NULL;
      if (codesetp)
	*codesetp = NULL;
      return;
    }

  __libc_rwlock_wrlock (_nl_state_lock);

  modified = 0;

  for (binding = _nl_domain_bindings; binding != NULL; binding = binding->next)
    {
      int compare = strcmp (domainname, binding->domainname);
      if (compare == 0)
	/* We found it!  */
	break;
      if (compare < 0)
	{
	  /* It is not in the list.  */
	  binding = NULL;
	  break;
	}
    }

  if (binding != NULL)
    {
      if (dirnamep)
	{
	  const char *dirname = *dirnamep;

	  if (dirname == NULL)
	    /* The current binding has be to returned.  */
	    *dirnamep = binding->dirname;
	  else
	    {
	      /* The domain is already bound.  If the new value and the old
		 one are equal we simply do nothing.  Otherwise replace the
		 old binding.  */
	      char *result = binding->dirname;
	      if (strcmp (dirname, result) != 0)
		{
		  if (strcmp (dirname, INTUSE(_nl_default_dirname)) == 0)
		    result = (char *) INTUSE(_nl_default_dirname);
		  else
		    {
#if defined _LIBC || defined HAVE_STRDUP
		      result = strdup (dirname);
#else
		      size_t len = strlen (dirname) + 1;
		      result = (char *) malloc (len);
		      if (__builtin_expect (result != NULL, 1))
			memcpy (result, dirname, len);
#endif
		    }

		  if (__builtin_expect (result != NULL, 1))
		    {
		      if (binding->dirname != INTUSE(_nl_default_dirname))
			free (binding->dirname);

		      binding->dirname = result;
		      modified = 1;
		    }
		}
	      *dirnamep = result;
	    }
	}

      if (codesetp)
	{
	  const char *codeset = *codesetp;

	  if (codeset == NULL)
	    /* The current binding has be to returned.  */
	    *codesetp = binding->codeset;
	  else
	    {
	      /* The domain is already bound.  If the new value and the old
		 one are equal we simply do nothing.  Otherwise replace the
		 old binding.  */
	      char *result = binding->codeset;
	      if (result == NULL || strcmp (codeset, result) != 0)
		{
#if defined _LIBC || defined HAVE_STRDUP
		  result = strdup (codeset);
#else
		  size_t len = strlen (codeset) + 1;
		  result = (char *) malloc (len);
		  if (__builtin_expect (result != NULL, 1))
		    memcpy (result, codeset, len);
#endif

		  if (__builtin_expect (result != NULL, 1))
		    {
		      if (binding->codeset != NULL)
			free (binding->codeset);

		      binding->codeset = result;
		      binding->codeset_cntr++;
		      modified = 1;
		    }
		}
	      *codesetp = result;
	    }
	}
    }
  else if ((dirnamep == NULL || *dirnamep == NULL)
	   && (codesetp == NULL || *codesetp == NULL))
    {
      /* Simply return the default values.  */
      if (dirnamep)
	*dirnamep = INTUSE(_nl_default_dirname);
      if (codesetp)
	*codesetp = NULL;
    }
  else
    {
      /* We have to create a new binding.  */
      size_t len = strlen (domainname) + 1;
      struct binding *new_binding =
	(struct binding *) malloc (offsetof (struct binding, domainname) + len);

      if (__builtin_expect (new_binding == NULL, 0))
	goto failed;

      memcpy (new_binding->domainname, domainname, len);

      if (dirnamep)
	{
	  const char *dirname = *dirnamep;

	  if (dirname == NULL)
	    /* The default value.  */
	    dirname = INTUSE(_nl_default_dirname);
	  else
	    {
	      if (strcmp (dirname, INTUSE(_nl_default_dirname)) == 0)
		dirname = INTUSE(_nl_default_dirname);
	      else
		{
		  char *result;
#if defined _LIBC || defined HAVE_STRDUP
		  result = strdup (dirname);
		  if (__builtin_expect (result == NULL, 0))
		    goto failed_dirname;
#else
		  size_t len = strlen (dirname) + 1;
		  result = (char *) malloc (len);
		  if (__builtin_expect (result == NULL, 0))
		    goto failed_dirname;
		  memcpy (result, dirname, len);
#endif
		  dirname = result;
		}
	    }
	  *dirnamep = dirname;
	  new_binding->dirname = (char *) dirname;
	}
      else
	/* The default value.  */
	new_binding->dirname = (char *) INTUSE(_nl_default_dirname);

      new_binding->codeset_cntr = 0;

      if (codesetp)
	{
	  const char *codeset = *codesetp;

	  if (codeset != NULL)
	    {
	      char *result;

#if defined _LIBC || defined HAVE_STRDUP
	      result = strdup (codeset);
	      if (__builtin_expect (result == NULL, 0))
		goto failed_codeset;
#else
	      size_t len = strlen (codeset) + 1;
	      result = (char *) malloc (len);
	      if (__builtin_expect (result == NULL, 0))
		goto failed_codeset;
	      memcpy (result, codeset, len);
#endif
	      codeset = result;
	      new_binding->codeset_cntr++;
	    }
	  *codesetp = codeset;
	  new_binding->codeset = (char *) codeset;
	}
      else
	new_binding->codeset = NULL;

      /* Now enqueue it.  */
      if (_nl_domain_bindings == NULL
	  || strcmp (domainname, _nl_domain_bindings->domainname) < 0)
	{
	  new_binding->next = _nl_domain_bindings;
	  _nl_domain_bindings = new_binding;
	}
      else
	{
	  binding = _nl_domain_bindings;
	  while (binding->next != NULL
		 && strcmp (domainname, binding->next->domainname) > 0)
	    binding = binding->next;

	  new_binding->next = binding->next;
	  binding->next = new_binding;
	}

      modified = 1;

      /* Here we deal with memory allocation failures.  */
      if (0)
	{
	failed_codeset:
	  if (new_binding->dirname != INTUSE(_nl_default_dirname))
	    free (new_binding->dirname);
	failed_dirname:
	  free (new_binding);
	failed:
	  if (dirnamep)
	    *dirnamep = NULL;
	  if (codesetp)
	    *codesetp = NULL;
	}
    }

  /* If we modified any binding, we flush the caches.  */
  if (modified)
    ++_nl_msg_cat_cntr;

  __libc_rwlock_unlock (_nl_state_lock);
}
