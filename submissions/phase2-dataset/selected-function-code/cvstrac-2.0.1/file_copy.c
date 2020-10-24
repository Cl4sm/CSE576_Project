static const char *file_copy(const char *zFrom, const char *zTo){
  FILE *pIn, *pOut;
  int n;
  long long int total = 0;
  char zBuf[10240];
  pIn = fopen(zFrom, "r");
  if( pIn==0 ){
    return mprintf(
      "Unable to copy files - cannot open \"%h\" for reading.", zFrom
    );
  }
  unlink(zTo);
  pOut = fopen(zTo, "w");
  if( pOut==0 ){
    fclose(pIn);
    return mprintf(
      "Unable to copy files - cannot open \"%h\" for writing.", zTo
    );
  }
  while( (n = fread(zBuf, 1, sizeof(zBuf), pIn))>0 ){
    if( fwrite(zBuf, 1, n, pOut)<n ){
      fclose(pIn);
      fclose(pOut);
      return mprintf(
        "Copy operation failed after %lld bytes.  Is the disk full?", total
      );
    }
    total += n;
  }
  fclose(pIn);
  fclose(pOut);
  return 0;
}