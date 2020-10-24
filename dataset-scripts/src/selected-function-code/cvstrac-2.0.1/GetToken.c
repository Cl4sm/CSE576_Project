static int GetToken(InStream *pIn, Token *pToken){
  int i;
  const char *z;
  int cStart;
  int c;
  int startLine;   /* Line on which a structure begins */
  int nlisc = 0;   /* True if there is a new-line in a ".." or '..' */
  int nErr = 0;    /* Number of errors seen */

  z = pIn->z;
  i = pIn->i;
  pToken->nLine = pIn->nLine;
  pToken->zText = &z[i];
  switch( z[i] ){
    case 0:
      pToken->eType = TT_EOF;
      pToken->nText = 0;
      break;

    case '#':
      if( i==0 || z[i-1]=='\n' || (i>1 && z[i-1]=='\r' && z[i-2]=='\n')){
        /* We found a preprocessor statement */
        pToken->eType = TT_Preprocessor;
        i++;
        while( z[i]!=0 && z[i]!='\n' ){
          if( z[i]=='\\' ){
            i++;
            if( z[i]=='\n' ) pIn->nLine++;
          }
          i++;
        }
        pToken->nText = i - pIn->i;
      }else{
        /* Just an operator */
        pToken->eType = TT_Other;
        pToken->nText = 1;
      }
      break;

    case ' ':
    case '\t':
    case '\r':
    case '\f':
    case '\n':
      while( isspace(z[i]) ){
        if( z[i]=='\n' ) pIn->nLine++;
        i++;
      }
      pToken->eType = TT_Space;
      pToken->nText = i - pIn->i;
      break;

    case '\\':
      pToken->nText = 2;
      pToken->eType = TT_Other;
      if( z[i+1]=='\n' ){
        pIn->nLine++;
        pToken->eType = TT_Space;
      }else if( z[i+1]==0 ){
        pToken->nText = 1;
      }
      break;

    case '\'':
    case '\"':
      cStart = z[i];
      startLine = pIn->nLine;
      do{
        i++;
        c = z[i];
        if( c=='\n' ){
          if( !nlisc ){
            fprintf(stderr,
              "%s:%d: (warning) Newline in string or character literal.\n",
              zFilename, pIn->nLine);
            nlisc = 1;
          }
          pIn->nLine++;
        }
        if( c=='\\' ){
          i++;
          c = z[i];
          if( c=='\n' ){
            pIn->nLine++;
          }
        }else if( c==cStart ){
          i++;
          c = 0;
        }else if( c==0 ){
          fprintf(stderr, "%s:%d: Unterminated string or character literal.\n",
             zFilename, startLine);
          nErr++;
        }
      }while( c );
      pToken->eType = TT_String;
      pToken->nText = i - pIn->i;
      break;

    case '/':
      if( z[i+1]=='/' ){
        /* C++ style comment */
        while( z[i] && z[i]!='\n' ){ i++; }
        pToken->eType = TT_Comment;
        pToken->nText = i - pIn->i;
      }else if( z[i+1]=='*' ){
        /* C style comment */
        int isBlockComment = i==0 || z[i-1]=='\n';
        i += 2;
        startLine = pIn->nLine;
        while( z[i] && (z[i]!='*' || z[i+1]!='/') ){
          if( z[i]=='\n' ){
            pIn->nLine++;
            if( isBlockComment ){
              if( z[i+1]=='*' || z[i+2]=='*' ){
                 isBlockComment = 2;
              }else{
                 isBlockComment = 0;
              }
            }
          }
          i++;
        }
        if( z[i] ){ 
          i += 2; 
        }else{
          isBlockComment = 0;
          fprintf(stderr,"%s:%d: Unterminated comment\n",
            zFilename, startLine);
          nErr++;
        }
        pToken->eType = isBlockComment==2 ? TT_BlockComment : TT_Comment;
        pToken->nText = i - pIn->i;
      }else{
        /* A divide operator */
        pToken->eType = TT_Other;
        pToken->nText = 1 + (z[i+1]=='+');
      }
      break;

    case '0': 
      if( z[i+1]=='x' || z[i+1]=='X' ){
        /* A hex constant */
        i += 2;
        while( isxdigit(z[i]) ){ i++; }
      }else{
        /* An octal constant */
        while( isdigit(z[i]) ){ i++; }
      }
      pToken->eType = TT_Number;
      pToken->nText = i - pIn->i;
      break;

    case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      while( isdigit(z[i]) ){ i++; }
      if( (c=z[i])=='.' ){
         i++;
         while( isdigit(z[i]) ){ i++; }
         c = z[i];
         if( c=='e' || c=='E' ){
           i++;
           if( ((c=z[i])=='+' || c=='-') && isdigit(z[i+1]) ){ i++; }
           while( isdigit(z[i]) ){ i++; }
           c = z[i];
         }
         if( c=='f' || c=='F' || c=='l' || c=='L' ){ i++; }
      }else if( c=='e' || c=='E' ){
         i++;
         if( ((c=z[i])=='+' || c=='-') && isdigit(z[i+1]) ){ i++; }
         while( isdigit(z[i]) ){ i++; }
      }else if( c=='L' || c=='l' ){
         i++;
         c = z[i];
         if( c=='u' || c=='U' ){ i++; }
      }else if( c=='u' || c=='U' ){
         i++;
         c = z[i];
         if( c=='l' || c=='L' ){ i++; }
      }
      pToken->eType = TT_Number;
      pToken->nText = i - pIn->i;
      break;

    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
    case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
    case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
    case 'v': case 'w': case 'x': case 'y': case 'z': case 'A': case 'B':
    case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I':
    case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P':
    case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W':
    case 'X': case 'Y': case 'Z': case '_':
      while( isalnum(z[i]) || z[i]=='_' ){ i++; };
      pToken->eType = TT_Id;
      pToken->nText = i - pIn->i;
      break;

    case ':': 
      pToken->eType = TT_Other;
      pToken->nText = 1 + (z[i+1]==':');
      break;

    case '=':
    case '<':
    case '>':
    case '+':
    case '-':
    case '*':
    case '%':
    case '^':
    case '&':
    case '|': 
      pToken->eType = TT_Other;
      pToken->nText = 1 + (z[i+1]=='=');
      break;

    default:
      pToken->eType = TT_Other;
      pToken->nText = 1;
      break;
  }
  pIn->i += pToken->nText;
  return nErr;
}