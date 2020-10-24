static int dup_to_cstring(JNIEnv *jenv, jstring in, char **out)
{
	const char *tmp;

	if (in == NULL) {
		*out = NULL;
		return 0;
	}

	tmp = (*jenv)->GetStringUTFChars(jenv, in, NULL);
	if (!tmp) {
		OOM(jenv);
		return -1;
	}

	*out = strdup(tmp);
	(*jenv)->ReleaseStringUTFChars(jenv, in, tmp);

	if (!*out) {
		OOM(jenv);
		return -1;
	}
	return 0;
}