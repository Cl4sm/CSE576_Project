init_mail()
{
#if defined(AMS_MAIL) || defined(UNIX_MAIL)
# ifdef UNIX_MAIL
	u_char	*tmp_mail_path;
# endif

	if (mail_path)
		return; /* why do it 2000 times?  -lynx */

# ifdef UNIX_MAIL
	if ((tmp_mail_path = my_getenv("MAIL")) != NULL)
		malloc_strcpy(&mail_path, tmp_mail_path);
	else
	{
		malloc_strcpy(&mail_path, UP(UNIX_MAIL));
		malloc_strcat(&mail_path, UP("/"));
		malloc_strcat(&mail_path, username);
	}
# else
#  ifdef AMS_MAIL
	malloc_strcpy(&mail_path, my_path);
	malloc_strcat(&mail_path, "/");
	malloc_strcat(&mail_path, AMS_MAIL);
#  endif /* AMS_MAIL */
# endif /* UNIX_MAIL */
#endif /* AMS_MAIL || UNIX_MAIL */
}
