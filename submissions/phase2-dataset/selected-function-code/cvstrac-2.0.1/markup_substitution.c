static char *markup_substitution(
  int strip_quotes,
  const char *zF,
  const Markup* sMarkup,
  const char *zInBlock,
  int lenBlock
){
  char *zOutput = NULL;
  unsigned const char *zFormat = (unsigned const char*)zF;
  char *azStrings[256];
  int  anLens[256];
  int j, k;

  /* If we don't treat args as blank where there aren't any,
  ** we can't create rules like <b>%k %a</b> that work
  ** with both {markup: this} and {markup: this is} formats. This
  ** is a fairly common convention with most of the existing markups.
  ** We strdup() the blank string because we _will_ free it when
  ** we leave this subroutine.
  */
  char *zArgs = (sMarkup->zArgs==sMarkup->zKey)
                ? strdup("")
                : mprintf("%.*s", sMarkup->lenArgs, sMarkup->zArgs );
  char *zMarkup = mprintf("%.*s", sMarkup->lenType, sMarkup->zType );
  char *zKey = mprintf("%.*s", sMarkup->lenKey, sMarkup->zKey );
  char *zBlock = mprintf("%.*s", lenBlock, zInBlock );
  const char *zRoot = db_config("cvsroot", "");

  if( strip_quotes ){
    /* if we're dealing with a program markup, strip out
    ** backslashes and quotes. This is why we can't just use
    ** "subst".
    */
    sanitize_string(zMarkup);
    sanitize_string(zKey);
    sanitize_string(zArgs);
    sanitize_string(zBlock);
  }

  memset( anLens, 0, sizeof(anLens) );
  memset( azStrings, 0, sizeof(azStrings) );

  azStrings['%'] = "%";
  anLens['%'] = 1;

  /* markup name substitution */
  azStrings['m'] = zMarkup;
  anLens['m'] = sMarkup->lenType;

  /* key substitution */
  azStrings['k'] = zKey;
  anLens['k'] = sMarkup->lenKey;

  /* block substitution */
  azStrings['b'] = zBlock;
  anLens['b'] = lenBlock;

  /* argument substitution. args isn't necessarily the same as
  ** sMarkup->zArgs. */
  azStrings['a'] = zArgs;
  anLens['a'] = strlen(zArgs);

  /* argument substitution. args isn't necessarily the same as
  ** sMarkup->zArgs. */
  azStrings['x'] = zArgs[0] ? zArgs : zKey;
  anLens['x'] = zArgs[0] ? strlen(zArgs) : sMarkup->lenKey;

  /* cvsroot */
  azStrings['r'] = (char*)zRoot;
  anLens['r'] = strlen(zRoot);

  /* basename... from this someone can get the db name */
  azStrings['n'] = (char*)g.zName;
  anLens['n'] = strlen(g.zName);

  /* logged in user */
  azStrings['u'] = (char*)g.zUser;
  anLens['u'] = strlen(g.zUser);

  /* capabilities */
  azStrings['c'] = db_short_query(
      "SELECT capabilities FROM user WHERE id='%q'",g.zUser);
  anLens['c'] = azStrings['c'] ? strlen(azStrings['c']) : 0;

  /* Calculate the space needed for the % subs.
  */
  for(k=j=0; zFormat[j]; j++){
    if( zFormat[j] == '%' && anLens[zFormat[j+1]] ){
      j ++;
      k += anLens[zFormat[j]];
      continue;
    }
    k ++;
  }

  /* (over)allocate an output buffer. By "over", I mean we get
  ** the length of the original plus the length we think we need
  ** for a fully substituted buffer.
  */
  zOutput = malloc(j + k + 1);
  if( zOutput == NULL ){
    free(zKey);
    free(zArgs);
    free(zMarkup);
    free(zBlock);
    if(azStrings['c']) free(azStrings['c']);
    return NULL;
  }

  /* actually perform the substitutions */
  for(k=j=0; zFormat[j]; j++){
    if( zFormat[j] == '%' && azStrings[zFormat[j+1]]!=0 ){
      j ++;
      memcpy(&zOutput[k],azStrings[zFormat[j]],anLens[zFormat[j]]);
      k += anLens[zFormat[j]];
      continue;
    }
    zOutput[k++] = zFormat[j];
  }
  zOutput[k] = 0;

  free(zKey);
  free(zArgs);
  free(zMarkup);
  free(zBlock);
  if(azStrings['c']) free(azStrings['c']);
  return zOutput;
}