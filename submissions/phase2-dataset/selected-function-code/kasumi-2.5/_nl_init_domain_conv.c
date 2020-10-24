const char *
internal_function
_nl_init_domain_conv (domain_file, domain, domainbinding)
     struct loaded_l10nfile *domain_file;
     struct loaded_domain *domain;
     struct binding *domainbinding;
{
  /* Find out about the character set the file is encoded with.
     This can be found (in textual form) in the entry "".  If this
     entry does not exist or if this does not contain the `charset='
     information, we will assume the charset matches the one the
     current locale and we don't have to perform any conversion.  */
  char *nullentry;
  size_t nullentrylen;

  /* Preinitialize fields, to avoid recursion during _nl_find_msg.  */
  domain->codeset_cntr =
    (domainbinding != NULL ? domainbinding->codeset_cntr : 0);
#ifdef _LIBC
  domain->conv = (__gconv_t) -1;
#else
# if HAVE_ICONV
  domain->conv = (iconv_t) -1;
# endif
#endif
  domain->conv_tab = NULL;

  /* Get the header entry.  */
  nullentry = _nl_find_msg (domain_file, domainbinding, "", &nullentrylen);

  if (nullentry != NULL)
    {
#if defined _LIBC || HAVE_ICONV
      const char *charsetstr;

      charsetstr = strstr (nullentry, "charset=");
      if (charsetstr != NULL)
	{
	  size_t len;
	  char *charset;
	  const char *outcharset;

	  charsetstr += strlen ("charset=");
	  len = strcspn (charsetstr, " \t\n");

	  charset = (char *) alloca (len + 1);
# if defined _LIBC || HAVE_MEMPCPY
	  *((char *) mempcpy (charset, charsetstr, len)) = '\0';
# else
	  memcpy (charset, charsetstr, len);
	  charset[len] = '\0';
# endif

	  /* The output charset should normally be determined by the
	     locale.  But sometimes the locale is not used or not correctly
	     set up, so we provide a possibility for the user to override
	     this.  Moreover, the value specified through
	     bind_textdomain_codeset overrides both.  */
	  if (domainbinding != NULL && domainbinding->codeset != NULL)
	    outcharset = domainbinding->codeset;
	  else
	    {
	      outcharset = getenv ("OUTPUT_CHARSET");
	      if (outcharset == NULL || outcharset[0] == '\0')
		{
# ifdef _LIBC
		  outcharset = _NL_CURRENT (LC_CTYPE, CODESET);
# else
#  if HAVE_ICONV
		  extern const char *locale_charset PARAMS ((void));
		  outcharset = locale_charset ();
#  endif
# endif
		}
	    }

# ifdef _LIBC
	  /* We always want to use transliteration.  */
	  outcharset = norm_add_slashes (outcharset, "TRANSLIT");
	  charset = norm_add_slashes (charset, NULL);
	  if (__gconv_open (outcharset, charset, &domain->conv,
			    GCONV_AVOID_NOCONV)
	      != __GCONV_OK)
	    domain->conv = (__gconv_t) -1;
# else
#  if HAVE_ICONV
	  /* When using GNU libc >= 2.2 or GNU libiconv >= 1.5,
	     we want to use transliteration.  */
#   if (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 2) || __GLIBC__ > 2 \
       || _LIBICONV_VERSION >= 0x0105
	  if (strchr (outcharset, '/') == NULL)
	    {
	      char *tmp;

	      len = strlen (outcharset);
	      tmp = (char *) alloca (len + 10 + 1);
	      memcpy (tmp, outcharset, len);
	      memcpy (tmp + len, "//TRANSLIT", 10 + 1);
	      outcharset = tmp;

	      domain->conv = iconv_open (outcharset, charset);

	      freea (outcharset);
	    }
	  else
#   endif
	    domain->conv = iconv_open (outcharset, charset);
#  endif
# endif

	  freea (charset);
	}
#endif /* _LIBC || HAVE_ICONV */
    }

  return nullentry;
}
