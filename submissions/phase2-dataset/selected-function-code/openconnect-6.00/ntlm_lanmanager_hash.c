static void ntlm_lanmanager_hash (const char *password, char hash[21])
{
	unsigned char lm_password[15];
	DES_KS ks;
	int i;

	for (i = 0; i < 14 && password[i]; i++)
		lm_password[i] = toupper ((unsigned char) password[i]);

	for (; i < 15; i++)
		lm_password[i] = '\0';

	memcpy (hash, LM_PASSWORD_MAGIC, 21);

	setup_schedule (lm_password, ks);
	des (ks, (unsigned char *) hash);

	setup_schedule (lm_password + 7, ks);
	des (ks, (unsigned char *) hash + 8);

	memset(lm_password, 0, sizeof(lm_password));
}