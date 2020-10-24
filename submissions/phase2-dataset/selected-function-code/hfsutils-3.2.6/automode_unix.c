cpifunc automode_unix(const char *path)
{
  int i;
  struct {
    const char *ext;
    cpifunc func;
  } exts[] = {
    { ".bin",  cpi_macb },
    { ".hqx",  cpi_binh },

    { ".txt",  cpi_text },
    { ".c",    cpi_text },
    { ".h",    cpi_text },
    { ".html", cpi_text },
    { ".htm",  cpi_text },
    { ".rtf",  cpi_text },

    { 0,       0        }
  };

  path += strlen(path);

  for (i = 0; exts[i].ext; ++i)
    {
      if (strcasecmp(path - strlen(exts[i].ext), exts[i].ext) == 0)
	return exts[i].func;
    }

  return cpi_raw;
}
