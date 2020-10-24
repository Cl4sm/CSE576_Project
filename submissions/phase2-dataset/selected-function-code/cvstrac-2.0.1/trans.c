static void trans(FILE *in, FILE *out){
  int i, j, k, l;     /* Loop counters */
  int lastWasEq = 0;  /* True if last non-whitespace character was "=" */
  char zLine[2000];   /* A single line of input */
  char zOut[4000];    /* The input line translated into appropriate output */
  char *zLF;

  while( fgets(zLine, sizeof(zLine), in) ){
    for(i=0; zLine[i] && isspace(zLine[i]); i++){}
    if( zLine[i]!='@' ){
      if( inPrint || inStr ) end_block(out);
      fprintf(out,"%s",zLine);
      i += strlen(&zLine[i]);
      while( i>0 && isspace(zLine[i-1]) ){ i--; }
      lastWasEq = i>0 && zLine[i-1]=='=';
    }else if( lastWasEq ){
      /* If the last non-whitespace character before the first @ was
      ** an "=" then generate a string literal.  But skip SQL comments
      ** consisting of all text between "--" and end of line.
      */
      int indent, omitline;
      i++;
      if( isspace(zLine[i]) ){ i++; }
      indent = i - 2;
      if( indent<0 ) indent = 0;
      omitline = 0;
      for(j=0; zLine[i] && zLine[i]!='\r' && zLine[i]!='\n'; i++){
        if( zLine[i]=='-' && zLine[i+1]=='-' ){ omitline = 1; break; }
        if( zLine[i]=='"' || zLine[i]=='\\' ){ zOut[j++] = '\\'; }
        zOut[j++] = zLine[i];
      }
      while( j>0 && isspace(zOut[j-1]) ){ j--; }
      zOut[j] = 0;
      if( j<=0 && omitline ){
        fprintf(out,"\n");
      }else{
        fprintf(out,"%*s\"%s\\n\"\n",indent, "", zOut);
      }
    }else{
      /* Otherwise (if the last non-whitespace was not '=') then generate
      ** a cgi_printf() statement whose format is the text following the '@'.
      ** Substrings of the form "%C(...)" where C is any character will
      ** puts "%C" in the format and add the "..." as an argument to the
      ** cgi_printf call.
      */
      int indent;
      i++;
      if( isspace(zLine[i]) ){ i++; }
      indent = i;
      
      /* If last non-whitespace character on the line is '\', 
      ** make sure we don't break the line in printed output.
      ** This should be something like a C style line continuation
      ** for cgi_printf() output, not on the level of generated 
      ** C source code.
      */
      l = strlen(zLine);
      while( l>0 && isspace(zLine[l-1]) ){ l--; }
      if( zLine[l-1]=='\\' ){
        zLine[l-1] = 0;
        zLF = "";
      }else{
        zLF = "\\n";
      }
    
      for(j=0; zLine[i] && zLine[i]!='\r' && zLine[i]!='\n'; i++){
        if( zLine[i]=='"' || zLine[i]=='\\' ){ zOut[j++] = '\\'; }
        zOut[j++] = zLine[i];
        if( zLine[i]!='%' || zLine[i+1]=='%' || zLine[i+1]==0 ) continue;
        if( zLine[i+2]!='(' ) continue;
        i++;
        zOut[j++] = zLine[i];
        zArg[nArg++] = ',';
        i += 2;
        k = 1;
        while( zLine[i] ){
          if( zLine[i]==')' ){
            k--;
            if( k==0 ) break;
          }else if( zLine[i]=='(' ){
            k++;
          }
          zArg[nArg++] = zLine[i++];
        }
      }
      zOut[j] = 0;
      
      if( !inPrint ){
        fprintf(out,"%*scgi_printf(\"%s%s\"",indent-2,"", zOut, zLF);
        inPrint = 1;
      }else{
        fprintf(out,"\n%*s\"%s%s\"",indent+5, "", zOut, zLF);
      }
    }
  }
}