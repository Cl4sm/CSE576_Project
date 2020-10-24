SFreplaceText(dir, str)
	SFDir	*dir;
	char	*str;
{
	int	len;

	*(dir->path) = 0;
	len = strlen(str);
	if (str[len - 1] == '/') {
		(void) strcat(SFcurrentPath, str);
	} else {
		(void) strncat(SFcurrentPath, str, len - 1);
	}
	if (strncmp(SFcurrentPath, SFstartDir, strlen(SFstartDir))) {
		SFsetText(SFcurrentPath);
	} else {
		SFsetText(&(SFcurrentPath[strlen(SFstartDir)]));
	}

	SFtextChanged();
}
