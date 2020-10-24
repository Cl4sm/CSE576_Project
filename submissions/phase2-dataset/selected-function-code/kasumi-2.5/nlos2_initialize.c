static __attribute__((constructor)) void
nlos2_initialize ()
{
  char *root = getenv ("UNIXROOT");
  char *gnulocaledir = getenv ("GNULOCALEDIR");

  _nlos2_libdir = gnulocaledir;
  if (!_nlos2_libdir)
    {
      if (root)
        {
          size_t sl = strlen (root);
          _nlos2_libdir = (char *) malloc (sl + strlen (LIBDIR) + 1);
          memcpy (_nlos2_libdir, root, sl);
          memcpy (_nlos2_libdir + sl, LIBDIR, strlen (LIBDIR) + 1);
        }
      else
        _nlos2_libdir = LIBDIR;
    }

  _nlos2_localealiaspath = gnulocaledir;
  if (!_nlos2_localealiaspath)
    {
      if (root)
        {
          size_t sl = strlen (root);
          _nlos2_localealiaspath = (char *) malloc (sl + strlen (LOCALE_ALIAS_PATH) + 1);
          memcpy (_nlos2_localealiaspath, root, sl);
          memcpy (_nlos2_localealiaspath + sl, LOCALE_ALIAS_PATH, strlen (LOCALE_ALIAS_PATH) + 1);
        }
     else
        _nlos2_localealiaspath = LOCALE_ALIAS_PATH;
    }

  _nlos2_localedir = gnulocaledir;
  if (!_nlos2_localedir)
    {
      if (root)
        {
          size_t sl = strlen (root);
          _nlos2_localedir = (char *) malloc (sl + strlen (LOCALEDIR) + 1);
          memcpy (_nlos2_localedir, root, sl);
          memcpy (_nlos2_localedir + sl, LOCALEDIR, strlen (LOCALEDIR) + 1);
        }
      else
        _nlos2_localedir = LOCALEDIR;
    }

  if (strlen (_nlos2_localedir) <= MAXPATHLEN)
    strcpy (libintl_nl_default_dirname, _nlos2_localedir);
}
