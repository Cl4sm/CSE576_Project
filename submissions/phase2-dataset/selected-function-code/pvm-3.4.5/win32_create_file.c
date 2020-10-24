win32_create_file(TMP_AREA, mode)
char *TMP_AREA;
int mode;
{
	HANDLE hFile;
	PSECURITY_DESCRIPTOR pSD;
	PACL pACLNew;
	DWORD cbACL = 1024;
	PSID pSID;
	DWORD cbSID = 1024;
	LPSTR szUserName;
	DWORD dwUserNameLength;
	LPSTR lpszDomain;
	DWORD cchDomainName = 80;
	PSID_NAME_USE psnuType;

  	/* begin of adding, by Jigen Zhou */

	LPSTR szUser = NULL;
	DWORD cchUser = 81;
	LPSTR szDomain = NULL;
	DWORD cchDomain = 81;
	HANDLE       hToken   = NULL;
	PTOKEN_USER  ptiUser  = NULL;
	DWORD        cbti     = 0;
	SID_NAME_USE snu;

	/* end of adding */

	/* Initialize a new security descriptor. */


	pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR,
		SECURITY_DESCRIPTOR_MIN_LENGTH);	/* defined in WINNT.H */
	if (pSD == NULL) {
		ErrorHandler("LocalAlloc");
		return FALSE;
	}

	if (!InitializeSecurityDescriptor(pSD,
			SECURITY_DESCRIPTOR_REVISION)) {

		ErrorHandler("InitializeSecurityDescriptor");
		goto Cleanup;
	}

	/* Initialize a new ACL.				 */

	pACLNew = (PACL) LocalAlloc(LPTR, cbACL);
	if (pACLNew == NULL) {
		ErrorHandler("LocalAlloc");
		goto Cleanup;
	}

	if (!InitializeAcl(pACLNew, cbACL, ACL_REVISION2)) {
		if (GetLastError() == ERROR_CALL_NOT_IMPLEMENTED) {
			return (HANDLE) -2;
		}
		ErrorHandler("InitializeAcl");
		goto Cleanup;
	}

	/* Retrieve the SID for User		*/
	if (username)
		szUserName=username;
	else
 		szUserName=MyGetUserName();

	/* begin of changing, by Jigen Zhou */

	/* Get the calling thread's access token. */
	if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE, &hToken)) {

	    /* Retry against process token if no thread token exists. */
	   if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
	      fprintf(stderr,"Error to open thread/process token\n");
	}

	if (hToken) {

	   /* Obtain the size of the user information in the token. */
	   if (GetTokenInformation(hToken, TokenUser, NULL, 0, &cbti)) {
	      /* Call should have failed due to zero-length buffer. */
	      ErrorHandler("GetTokenInformation");
	      goto Cleanup;
	   }

	   /* Allocate buffer for user information in the token. */
	   ptiUser = (PTOKEN_USER)LocalAlloc(LPTR, cbti);
	   if (ptiUser== NULL) {
	      ErrorHandler("LocalAlloc");
	      goto Cleanup;
	   }

	   /* Retrieve the user information from the token. */
	   if (!GetTokenInformation(hToken, TokenUser, ptiUser, cbti, &cbti)) {
	      pvmlogprintf("Error while GetTokenInformation: %d\n",
				GetLastError());
	      pvmlogprintf("Continued by LookupAccountName\n");
	      hToken = NULL;
	   } else {
	      /* Retrieve user name and domain name based on user's SID. */
	      szUser   = (LPSTR)LocalAlloc(LPTR, cchUser);
	      szDomain = (LPSTR)LocalAlloc(LPTR, cchDomain);
	      if ((szUser == NULL) || (szDomain == NULL)) {
	         ErrorHandler("LocalAlloc");
	         goto Cleanup;
	      }
	      if (!LookupAccountSid(NULL, ptiUser->User.Sid, szUser, &cchUser,
	              szDomain, &cchDomain, &snu)) {
	         pvmlogprintf("Error while LookupAccountSid: %d\n",
				GetLastError());
	         pvmlogprintf("Continued by LookupAccountName\n");
	         hToken = NULL;
	      } else {
	         if (strcmp(szUser, szUserName)) {
	            pvmlogprintf("LookupAccountSid get %s does not match %s\n",
	                      szUser, szUserName);
	            pvmlogprintf("Continued by LookupAccountName\n");
	            hToken = NULL;
	         } else {
	            pSID = ptiUser->User.Sid;
	         }
	      }
	   }
	}

	if (hToken == NULL) {    /* This is the original block of code */
 
	   pSID = (PSID) LocalAlloc(LPTR, cbSID);
	   psnuType = (PSID_NAME_USE) LocalAlloc(LPTR, 1024);
	   lpszDomain = (LPSTR) LocalAlloc(LPTR, cchDomainName);
	   if (pSID == NULL || psnuType == NULL ||
		lpszDomain == NULL) {
		ErrorHandler("LocalAlloc");
		goto Cleanup;
	   }

	   if (!LookupAccountName((LPSTR) NULL, /* local name */
			szUserName,
			pSID,
			&cbSID,
			lpszDomain,
			&cchDomainName,
			psnuType)) {
		ErrorHandler("LookupAccountName");
		goto Cleanup;
	   }
	}

   /* end of changing, by Jigen Zhou */


	if (!IsValidSid(pSID))
		ErrorHandler("SID is not valid.\n");


	/* Allow read but not write access to the file. */

	if (!AddAccessAllowedAce(pACLNew,
			ACL_REVISION2,
			GENERIC_ALL,
			pSID)) {
		ErrorHandler("AddAccessAllowedAce");
		goto Cleanup;
	}

	/* Add a new ACL to the security descriptor. */

	if (!SetSecurityDescriptorDacl(pSD,
			TRUE,			/* fDaclPresent flag */
			pACLNew,
			FALSE)) {		/* not a default disc. ACL */
		ErrorHandler("SetSecurityDescriptorDacl");
		goto Cleanup;
	}

	hFile = CreateFile(TMP_AREA,
					GENERIC_WRITE | GENERIC_READ,
					// FILE_SHARE_READ | FILE_SHARE_WRITE,
					0,
					NULL,
					mode,
					FILE_ATTRIBUTE_NORMAL,
					NULL);

	if (GetLastError() == ERROR_ALREADY_EXISTS)
		if (mode == CREATE_NEW)
			return (HANDLE) -1;

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return (HANDLE) -1;
	}

	/* Apply the new security descriptor to the file. */

	if (!SetFileSecurity(TMP_AREA,
		DACL_SECURITY_INFORMATION,
		pSD)) {
		ErrorHandler("SetFileSecurity");
		goto Cleanup;
	}

	/*
	printf("Successfully added access-allowed ACE to file's DACL.\n");
	*/

	return hFile;

Cleanup:
		FreeSid(pSID);
		if(pSD != NULL)
			LocalFree((HLOCAL) pSD);
		if(pACLNew != NULL)
			LocalFree((HLOCAL) pACLNew);
		if(psnuType != NULL)
			LocalFree((HLOCAL) psnuType);
		if(lpszDomain != NULL)
			LocalFree((HLOCAL) lpszDomain);
	return 0;
}
