static int ParseFile(Token *pList, int initFlags){
  int nErr = 0;
  Token *pStart = 0;
  int flags = initFlags;
  int presetFlags = initFlags;
  int resetFlag = 0;

  includeList = 0;
  while( pList ){
    switch( pList->eType ){
    case TT_EOF:
      goto end_of_loop;

    case TT_Preprocessor:
      nErr += ParsePreprocessor(pList,flags,&presetFlags);
      pStart = 0;
      presetFlags |= initFlags;
      flags = presetFlags;
      break;

    case TT_Other:
      switch( pList->zText[0] ){
      case ';':
        nErr += ProcessDecl(pStart,pList,flags);
        pStart = 0;
        flags = presetFlags;
        break;

      case '=':
        if( pList->pPrev->nText==8 
            && strncmp(pList->pPrev->zText,"operator",8)==0 ){
          break;
        }
        nErr += ProcessDecl(pStart,pList,flags);
        pStart = 0;
        while( pList && pList->zText[0]!=';' ){
          pList = pList->pNext;
        }
        if( pList==0 ) goto end_of_loop;
        flags = presetFlags;
        break;

      case ':':
        if( pList->zText[1]==':' ){
          flags |= PS_Method;
        }
        break;

      default:
        break;
      }
      break;

    case TT_Braces:
      nErr += ProcessProcedureDef(pStart,pList,flags);
      pStart = 0;
      flags = presetFlags;
      break;

    case TT_Id:
       if( pStart==0 ){
          pStart = pList;
          flags = presetFlags;
       }
       resetFlag = 0;
       switch( pList->zText[0] ){
       case 'c':
         if( pList->nText==5 && strncmp(pList->zText,"class",5)==0 ){
           nErr += ProcessTypeDecl(pList,flags,&resetFlag);
         }
         break;

       case 'E':
         if( pList->nText==6 && strncmp(pList->zText,"EXPORT",6)==0 ){
           flags |= PS_Export2;
           /* pStart = 0; */
         }
         break;

       case 'e':
         if( pList->nText==4 && strncmp(pList->zText,"enum",4)==0 ){
           if( pList->pNext && pList->pNext->eType==TT_Braces ){
             pList = pList->pNext;
           }else{
             nErr += ProcessTypeDecl(pList,flags,&resetFlag);
           }
         }else if( pList->nText==6 && strncmp(pList->zText,"extern",6)==0 ){
           pList = pList->pNext;
           if( pList && pList->nText==3 && strncmp(pList->zText,"\"C\"",3)==0 ){
             pList = pList->pNext;
             flags &= ~DP_Cplusplus;
           }else{
             flags |= PS_Extern;
           }
           pStart = pList;
         }
         break;

       case 'i':
         if( pList->nText==6 && strncmp(pList->zText,"inline",6)==0 ){
           nErr += ProcessInlineProc(pList,flags,&resetFlag);
         }
         break;

       case 'L':
         if( pList->nText==5 && strncmp(pList->zText,"LOCAL",5)==0 ){
           flags |= PS_Local2;
           pStart = pList;
         }
         break;

       case 'P':
         if( pList->nText==6 && strncmp(pList->zText, "PUBLIC",6)==0 ){
           flags |= PS_Public;
           pStart = pList;
         }else if( pList->nText==7 && strncmp(pList->zText, "PRIVATE",7)==0 ){
           flags |= PS_Private;
           pStart = pList;
         }else if( pList->nText==9 && strncmp(pList->zText,"PROTECTED",9)==0 ){
           flags |= PS_Protected;
           pStart = pList;
         }
         break;

       case 's':
         if( pList->nText==6 && strncmp(pList->zText,"struct",6)==0 ){
           if( pList->pNext && pList->pNext->eType==TT_Braces ){
             pList = pList->pNext;
           }else{
             nErr += ProcessTypeDecl(pList,flags,&resetFlag);
           }
         }else if( pList->nText==6 && strncmp(pList->zText,"static",6)==0 ){
           flags |= PS_Static;
         }
         break;

       case 't':
         if( pList->nText==7 && strncmp(pList->zText,"typedef",7)==0 ){
           flags |= PS_Typedef;
         }
         break;

       case 'u':
         if( pList->nText==5 && strncmp(pList->zText,"union",5)==0 ){
           if( pList->pNext && pList->pNext->eType==TT_Braces ){
             pList = pList->pNext;
           }else{
             nErr += ProcessTypeDecl(pList,flags,&resetFlag);
           }
         }
         break;

       default:
         break;
       }
       if( resetFlag!=0 ){
         while( pList && pList->zText[0]!=resetFlag ){
           pList = pList->pNext;
         }
         if( pList==0 ) goto end_of_loop;
         pStart = 0;
         flags = presetFlags;
       }
       break;

    case TT_String:
    case TT_Number:
       break;

    default:
       pStart = pList;
       flags = presetFlags;
       break;
    }
    pList = pList->pNext;
  }
  end_of_loop:

  /* Verify that all #ifs have a matching "#endif" */
  while( ifStack ){
    Ifmacro *pIf = ifStack;
    ifStack = pIf->pNext;
    fprintf(stderr,"%s:%d: This '#if' has no '#endif'\n",zFilename,
      pIf->nLine);
    SafeFree(pIf);
  }

  return nErr;
}