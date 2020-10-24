{
  if (Tcl_Init(interp) == TCL_ERROR ||
      Tk_Init(interp)  == TCL_ERROR ||
      Hfs_Init(interp) == TCL_ERROR)
    return TCL_ERROR;

  /* initialize bitmaps */

  if (DEFBITMAP(interp, macdaemon)        == TCL_ERROR ||
      DEFBITMAP(interp, macdaemon_mask)   == TCL_ERROR ||

      DEFBITMAP(interp, stop)             == TCL_ERROR ||
      DEFBITMAP(interp, caution)          == TCL_ERROR ||
      DEFBITMAP(interp, note)             == TCL_ERROR ||

      DEFBITMAP(interp, floppy)           == TCL_ERROR ||
      DEFBITMAP(interp, harddisk)         == TCL_ERROR ||
      DEFBITMAP(interp, cdrom)            == TCL_ERROR ||

      DEFBITMAP(interp, floppy_mask)      == TCL_ERROR ||
      DEFBITMAP(interp, harddisk_mask)    == TCL_ERROR ||
      DEFBITMAP(interp, cdrom_mask)       == TCL_ERROR ||

      DEFBITMAP(interp, sm_floppy)        == TCL_ERROR ||
      DEFBITMAP(interp, sm_harddisk)      == TCL_ERROR ||
      DEFBITMAP(interp, sm_cdrom)         == TCL_ERROR ||

      DEFBITMAP(interp, folder)           == TCL_ERROR ||
      DEFBITMAP(interp, document)         == TCL_ERROR ||
      DEFBITMAP(interp, application)      == TCL_ERROR ||

      DEFBITMAP(interp, folder_mask)      == TCL_ERROR ||
      DEFBITMAP(interp, document_mask)    == TCL_ERROR ||
      DEFBITMAP(interp, application_mask) == TCL_ERROR ||

      DEFBITMAP(interp, sm_folder)        == TCL_ERROR ||
      DEFBITMAP(interp, sm_document)      == TCL_ERROR ||
      DEFBITMAP(interp, sm_application)   == TCL_ERROR ||

      DEFBITMAP(interp, help)             == TCL_ERROR ||
      DEFBITMAP(interp, padlock)          == TCL_ERROR)
    return TCL_ERROR;

  /* execute main script */

  Tcl_SetVar(interp, "argv0", argv0, TCL_GLOBAL_ONLY);

  if (Tcl_Eval(interp, xhfs) == TCL_ERROR)
    {
      fprintf(stderr, "Error: %s\n", interp->result);
      exit(1);
    }

  return TCL_OK;
}
