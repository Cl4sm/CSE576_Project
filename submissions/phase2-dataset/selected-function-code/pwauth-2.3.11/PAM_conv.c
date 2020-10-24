     struct pam_response **resp, void *appdata_ptr)
{
    struct ad_user *user= (struct ad_user *)appdata_ptr;
#endif /* PAM_SOLARIS_26 */
    struct pam_response *response;
    int i;

    /* Sanity checking */
    if (msg == NULL || resp == NULL || user == NULL)
    	return PAM_CONV_ERR;

#ifdef PAM_SOLARIS
    if (*msg == NULL)
	return PAM_CONV_ERR;
#endif

    response= (struct pam_response *)
    	malloc(num_msg * sizeof(struct pam_response));

    for (i= 0; i < num_msg; i++)
    {
	response[i].resp_retcode= 0;
	response[i].resp= NULL;

	switch (msgi(i).msg_style)
	{
	case PAM_PROMPT_ECHO_ON:
	    /* Store the login as the response */
	    /* This likely never gets called, since login was on pam_start() */
	    response[i].resp= appdata_ptr ? (char *)strdup(user->login) : NULL;
	    break;

	case PAM_PROMPT_ECHO_OFF:
	    /* Store the password as the response */
	    response[i].resp= appdata_ptr ? (char *)strdup(user->passwd) : NULL;
	    break;

	case PAM_TEXT_INFO:
	case PAM_ERROR_MSG:
	    /* Shouldn't happen since we have PAM_SILENT set. If it happens
	     * anyway, ignore it. */
	    break;

	default:
	    /* Something strange... */
	    if (response != NULL) free(response);
	    return PAM_CONV_ERR;
	}
    }
    /* On success, return the response structure */
    *resp= response;
    return PAM_SUCCESS;
}
