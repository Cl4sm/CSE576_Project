static void highlightFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
  int i, j, k;
  int size;
  int wn, idx;
  int nKey;                 /* Number of search terms in zPattern */
  char *azKey[50];          /* Up to 50 search terms in zPattern */
  int keySize[50];          /* Number of characters in each search term */
  int begin[5], end[5];     /* Up to 5 100-character segments of text */
  int sbegin[5], send[5];   /* The same 5 segments, sorted and coalesced */
  char mask[256];           /* True if first character of a key */
  char *zAll;
  char *zPattern;
  char *z;

  /* Output is written into zOut[].  There are at most 5 exemplars of
  ** about 100 characters each - total 500 characters.  But then we have
  ** to insert <b> and </b> around search terms and escape HTML characters
  ** such as < and > and &.  The maximum expansion is "& " converted
  ** into "<b>&amp;</b> " or less than 7-to-1.  So the maximum output
  ** is 3500.  Add a little extra space for the ellipses between
  ** exemplars.  Plus some margin in case the calculation above is
  ** wrong in some way.
  */
  char zOut[8000];

  /*
  ** We must have a pattern and at least one text block to function.
  */
  if( argc<2 || argv[0]==0 ) return;
  zPattern = (char *)sqlite3_value_text(argv[0]);
  if( zPattern[0]==0 ) return;

  /*
  ** Make a copy of the pattern.
  */
  zPattern = strdup(zPattern);
  if( zPattern==0 ) return;

  /*
  ** Concatenate all text to be analyzed.
  */
  size = 0;
  for(k=1; k<argc; k++){
    if( argv[k]==0 ) continue;  
    z = (char*)sqlite3_value_text(argv[k]);
    if( z==0 ) continue;
    size += strlen(z)+1;
  }
  zAll = malloc( size );
  if( zAll==0 ) return;
  for(i=0, k=1; k<argc; k++){
    if( argv[k]==0 ) continue;
    z = (char*)sqlite3_value_text(argv[k]);
    if( z==0 ) continue;
    if( i>0 ){ zAll[i++] = '\n'; }
    strcpy(&zAll[i], z);
    i += strlen(&zAll[i]);
  }

  /*
  ** Find as many as 5 exemplar segments in the text with each segment
  ** as long as 100 characters.
  */
  nKey = 0;
  for(wn=i=0; wn<50 && zPattern[i];){
    int score;
    if( isspace(zPattern[i]) ){ i++; continue; }
    for(j=1; zPattern[i+j] && !isspace(zPattern[i+j]); j++){}
    azKey[nKey] = &zPattern[i];
    keySize[nKey++] = j;
    score = score_word(&zPattern[i], zAll, j, &idx);
    i += j;
    if( zPattern[i] ) zPattern[i++] = 0;
    if( score==0 ) continue;
    begin[wn] = idx-50 > 0 ? idx-50 : 0;
    end[wn] = begin[wn]+100 > size ? size : begin[wn]+100;
    wn++;
  }

  /*
  ** Sort an coalesce the exemplars
  */
  if( wn==0 ){
    begin[0] = 0;
    end[0] = size>100 ? 100 : size;
    wn = 1;
  }
  if( g.useUTF8 ){
    /*
    ** Avoid splitting UTF-8 characters by detecting high bits in the
    ** begin/end characters.
    */
    for(i=0; i<wn; i++){
      while( begin[i]>0 && (zAll[begin[i]]&0xc0)==0x80 ){
        begin[i]--;
      }
      while( (zAll[end[i]]&0xc0)==0x80 ){
        end[i]++;
      }
    }
  }
  for(i=0; i<5 && wn>0; i++){
    int min = begin[0];
    k = 0;
    for(j=1; j<wn; j++){
      if( begin[j]<min ){
        k = j;
        min = begin[j];
      }
    }
    sbegin[i] = begin[k];
    send[i] = end[k];
    wn--;
    begin[k] = begin[wn];
    end[k] = end[wn];
  }
  wn = i;
  begin[0] = sbegin[0];
  end[0] = send[0];
  for(i=j=1; i<wn; i++){
    if( sbegin[i]>end[j-1]+1 ){
      begin[j] = sbegin[i];
      end[j] = send[i];
      j++;
    }else if( send[i]>end[j-1] ){
      end[j-1] = send[i];
    }
  }
  wn = j;

  /*
  ** Initialize the mask[] array so that mask[x] has value 1 if x is the
  ** first character of any key pattern.
  */
  memset(mask, 0, sizeof(mask));
  for(i=0; i<nKey; i++){
    mask[toupper(azKey[i][0])] = 1;
    mask[tolower(azKey[i][0])] = 1;
  }

  /*
  ** Generate the output stream
  */
  z = zOut;
  for(i=0; i<wn; i++){
    if( begin[i]>0 ){
      if( i>0 ){ *(z++) = ' '; }
      strcpy(z, "<b>...</b> ");
      z += strlen(z);
    }
    for(j=begin[i]; j<end[i]; j++){
      int c = zAll[j];
      if( c=='&' ){
        strcpy(z, "&amp;");
        z += 5;
      }else if( c=='<' ){
        strcpy(z, "&lt;");
        z += 4;
      }else if( c=='>' ){
        strcpy(z, "&gt;");
        z += 4;
      }else if( mask[c] ){
        for(k=0; k<nKey; k++){
          int n;
          if( tolower(c)!=tolower(azKey[k][0]) ) continue;
          n = keySize[k];
          if( strncasecmp(&zAll[j],azKey[k],n)==0 ){
            strcpy(z,"<b>");
            z += 3;
            while( n ){
              c = zAll[j++];
              if( c=='&' ){
                strcpy(z, "&amp;");
                z += 5;
              }else if( c=='<' ){
                strcpy(z, "&lt;");
                z += 4;
              }else if( c=='>' ){
                strcpy(z, "&gt;");
                z += 4;
              }else{
                *(z++) = c;
              }
              n--;
            }
            j--;
            strcpy(z,"</b>");
            z += 4;
            break;
          }
        }
        if( k>=nKey ){
          *(z++) = c;
        }
      }else{
        *(z++) = c;
      }
    }
  }
  if( end[wn-1]<size ){
    strcpy(z, " <b>...</b>");
  }else{
    *z = 0;
  }

  /*
  ** Report back the results
  */
  sqlite3_result_text(context, zOut, -1, SQLITE_TRANSIENT);
  free(zAll);
  free(zPattern);
}