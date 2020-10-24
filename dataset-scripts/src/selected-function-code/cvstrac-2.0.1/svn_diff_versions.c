static int svn_diff_versions(
  const char *oldVersion,
  const char *newVersion,
  const char *zFile
){
  const char *zTemplate;
  char *zCmd;
  FILE *in;
  int i;
  const char *azSubst[16];
  char zLine[2][2000];
  int nBuf, inFile;
  char *z;
  long long nLine;
  int file_cat; /* set when we want "svnlook cat" instead of "svnlook diff" */
  
  if( zFile==0 ) return -1;
    
  /* If this is the first time file is checked into repository (Added), 
  ** don't diff it just display it's contents
  */
  z = mprintf("%s", newVersion);
  previous_version(z, zFile);
  if( z==0 || z[0]==0 )
    file_cat = 1; /* svnlook cat */
  else
    file_cat = 0; /* svnlook diff */
  
  free(z);
  
  /* Find the command used to compute the file difference.
  */
  azSubst[0] = "F";
  azSubst[1] = zFile;
  azSubst[2] = "V1";
  azSubst[3] = oldVersion;
  azSubst[4] = "V2";
  azSubst[5] = newVersion;
  azSubst[6] = "RP";
  azSubst[7] = db_config("cvsroot", "");
  azSubst[8] = "V";
  azSubst[9] = newVersion;
  azSubst[10] = 0;
  if( file_cat==1 ){
    zTemplate = db_config("filelist","svnlook cat -r '%V' '%RP' '%F' 2>/dev/null");
  }else{
    zTemplate = db_config("filediff","svnlook diff -r '%V2' '%RP' 2>/dev/null");
  }
  zCmd = subst(zTemplate, azSubst);
  in = popen(zCmd, "r");
  free(zCmd);
  if( in==0 ) return -1;
  if( file_cat==0 ){
    /* We're looking for something like this:
    **
    ** Modified: trunk/vendors/deli/sandwich.txt
    ** ==============================================================================
    **
    ** So first we look for a delimiter line since it should be more uniqe 
    ** then one before it. When we find it, we go one line back and check
    ** if that is the file we're after. That line can begin with any element
    ** of zMarker.
    */
    const char zMarker[3][11] = { "Modified: ", "Added: ", "Deleted: " };
    int nMarkerLen[3] = { 10, 7, 9 };
    nBuf = nLine = inFile = 0;
    @ <pre>
    while( fgets(zLine[nBuf], sizeof(zLine[nBuf]), in) ){
      if( strncmp(zLine[nBuf], "===================================================================", 67)==0
          && (zLine[nBuf][67]=='\n' || zLine[nBuf][67]=='\r')
      ){
        if( inFile ){
          /* This is the begging of some other file, and end of ours */
          break;
        } else {
          /* If previous line begins with one of our markers, 
          ** it could be what we're looking for
          */
          for(i=0; i<sizeof(nMarkerLen)/sizeof(nMarkerLen[0]); i++)
            if( strncmp(zLine[(nBuf+1)%2], zMarker[i], nMarkerLen[i])==0 )
              break;
          
          if( i<sizeof(nMarkerLen)/sizeof(nMarkerLen[0]) ){
            /* We found one of the markers on previous line! Now we need to 
            ** check if our filename is present, which would mean we found 
            ** our file and can strat sending it to browser
            */
            int iLen = strlen(zFile);
            if( strncmp(&zLine[(nBuf+1)%2][nMarkerLen[i]], zFile, iLen)==0 
                && (zLine[(nBuf+1)%2][nMarkerLen[i]+iLen]=='\n' 
                    || zLine[(nBuf+1)%2][nMarkerLen[i]+iLen]=='\r')
            ){
              inFile = 1;
            }
          }
        }
      }
      
      if( inFile ){
        /* We can't print current line yet since it may be end of our 
        ** file in diff output, so we print previous line here
        */
        cgi_printf("%h", zLine[(nBuf+1)%2]);
      }
      nBuf = ++nLine % 2;
    }
    @ </pre>
  } else {
    output_pipe_as_html(in,1);
    pclose(in);
  }
  
  return 0;
}