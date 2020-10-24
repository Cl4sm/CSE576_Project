static int
set_security_context (const char *temp_name, const char *name,
		      int ignore_errors)
{
#ifdef USE_SELINUX
  static int selinux_enabled = -1;
  if (selinux_enabled == -1)
    selinux_enabled = is_selinux_enabled ();
  if (selinux_enabled > 0)
    {
      security_context_t scontext;
      if (getfilecon (name, &scontext) < 0)
	{
	  /* If the filesystem doesn't support extended attributes,
	     the original had no special security context and the
	     target cannot have one either.  */
	  if (errno == EOPNOTSUPP)
	    return 0;

	  error (0, errno, "Could not get security context for %s",
		 name);
	  return 1;
	}
      if (setfilecon (temp_name, scontext) < 0 && !ignore_errors)
	{
	  error (0, errno, "Could not set security context for %s",
		 name);
	  freecon (scontext);
	  return 1;
	}
      freecon (scontext);
    }
#endif
  return copy_xattrs (temp_name, name, ignore_errors);
}
