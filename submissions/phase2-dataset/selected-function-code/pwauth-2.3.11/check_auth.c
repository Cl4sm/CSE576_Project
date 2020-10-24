int check_auth(char *login, char *passwd)
{
#ifndef PAM_SOLARIS_26
    struct ad_user user_info= {login, passwd};
#endif /* PAM_SOLARIS_26 */
    struct pam_conv conv= { PAM_conv, (void *)&user_info };
    pam_handle_t *pamh= NULL;
    int retval;

#ifdef NEED_UID
    struct passwd *pwd;

    if ((pwd= getpwnam(login)) == NULL) return STATUS_UNKNOWN;
    hisuid= pwd->pw_uid;
    haveuid= 1;
#endif
#ifdef MIN_UNIX_UID
    if (hisuid < MIN_UNIX_UID) return STATUS_BLOCKED;
#endif

#ifdef PAM_SOLARIS_26
    user_info.login= login;
    user_info.passwd= passwd;
#endif /* PAM_SOLARIS_26 */

    retval= pam_start("pwauth", login, &conv, &pamh);

    if (retval == PAM_SUCCESS)
	retval= pam_authenticate(pamh, PAM_SILENT);

    if (retval == PAM_SUCCESS)
	retval= pam_acct_mgmt(pamh, 0);		/* permitted access? */

    if (pam_end(pamh,retval) != PAM_SUCCESS)	/* close PAM */
    {
	pamh= NULL;
	return STATUS_INT_ERR;
    }

    /* On failure we always return STATUS_UNKNOWN although we can't tell
     * if the failure was because of a bad login or a bad password */
    return (retval == PAM_SUCCESS ? STATUS_OK : STATUS_UNKNOWN);
}
