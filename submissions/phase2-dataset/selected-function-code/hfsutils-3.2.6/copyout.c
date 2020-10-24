int copyout(Tcl_Interp *interp, hfsvol *vol, char *argv[])
{
  cpofunc copyfile;

  if (strcmp(argv[2], "macbinary") == 0 ||
      strcmp(argv[2], "macb") == 0)
    copyfile = cpo_macb;
  else if (strcmp(argv[2], "binhex") == 0 ||
	   strcmp(argv[2], "binh") == 0)
    copyfile = cpo_binh;
  else if (strcmp(argv[2], "text") == 0)
    copyfile = cpo_text;
  else if (strcmp(argv[2], "raw") == 0 ||
	   strcmp(argv[2], "data") == 0)
    copyfile = cpo_raw;
  else
    {
      interp->result = "bad mode: must be macb, binh, text, or raw";
      return TCL_ERROR;
    }

  if (copyfile(vol, argv[3], argv[4]) == -1)
    {
      ERROR(errno, cpo_error);
      return error(interp, 0);
    }

  return TCL_OK;
}
