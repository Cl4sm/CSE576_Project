read_pvmregistry(token_call)
const char *token_call;
{
	long return_value;
	HKEY top=NULL;
	HKEY phkResult = NULL;
	static char subkey[]="SOFTWARE\\PVM";
	static char root[]="SOFTWARE\\PVM";
 	static char token[16];

	LPTSTR key_name ="";
	DWORD type = REG_EXPAND_SZ;

	int value_size=0;
	int i=0;
	char value[64];
	char *give_back = (char *) malloc (64 * sizeof(char));
	value_size=sizeof(value);

	top = HKEY_LOCAL_MACHINE;
	for (i=0; token_call[i];i++)
		token[i]=token_call[i];
	token[i++]=0;
	token[i++]=0;

	key_name=subkey;

	return_value = RegOpenKeyEx(top, 
				key_name, 
				0, 
				KEY_EXECUTE, 
				&phkResult);
    
	if ( return_value != ERROR_SUCCESS ) {
      		fprintf(stderr,"RegOpenKey failed for %s: %d\n",token,(int) return_value);
		return NULL;
	}
	
	return_value = RegQueryValueEx(phkResult,
					token,
					NULL,
					(LPDWORD) &type,
					(LPBYTE) &value,
					(LPDWORD)&value_size);
	if ( return_value != ERROR_SUCCESS ) {
	      fprintf(stderr,"RegQueryValueEx failed for %s: %d \n", token,(int) return_value);
	      return NULL;
	}
	strcpy(give_back,(char *)value);
	RegCloseKey(phkResult);
	return (char* ) give_back;
}
