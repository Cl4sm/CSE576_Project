static
int copyin(Tcl_Interp *interp, hfsvol *vol, char *argv[])
{
  cpifunc copyfile;

  if (strcmp(argv[2], "macbinary") == 0 ||
      strcmp(argv[2], "macb") == 0)
    copyfile = cpi_macb;
  else if (strcmp(argv[2], "binhex") == 0 ||
	   strcmp(argv[2], "binh") == 0)
    copyfile = cpi_binh;
  else if (strcmp(argv[2], "text") == 0)
    copyfile = cpi_text;
  else if (strcmp(argv[2], "raw") == 0 ||
	   strcmp(argv[2], "data") == 0)
    copyfile = cpi_raw;
  else
    {
      interp->result = "bad mode: must be macb, binh, text, or raw";
      return TCL_ERROR;
    }

  if (copyfile(argv[3], vol, argv[4]) == -1)
    {
      ERROR(errno, cpi_error);
      return error(interp, 0);
    }

  return TCL_OK;
}
