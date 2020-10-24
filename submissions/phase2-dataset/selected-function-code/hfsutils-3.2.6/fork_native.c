static
int fork_native(Tcl_Interp *interp, hfsfile *ifile, hfsfile *ofile)
{
  char buf[HFS_BLOCKSZ * 8];
  long chunk, bytes;

  while (1)
    {
      chunk = hfs_read(ifile, buf, sizeof(buf));
      if (chunk == -1)
	return error(interp, "error reading source file");
      else if (chunk == 0)
	break;

      bytes = hfs_write(ofile, buf, chunk);
      if (bytes == -1)
	return error(interp, "error writing destination file");
      else if (bytes != chunk)
	return error(interp, "wrote incomplete chunk");
    }

  return TCL_OK;
}
