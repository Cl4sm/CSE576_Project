	char *script;
	script = getPath(PATH_SCRIPTS, "save.lua");
	scripting_RunFile(script);
	free(script);

#ifdef WIN32
	scripting_RunFormat("writeto(\"%s\")", "gltron.ini");
#else
	{
		char *path = getPossiblePath(PATH_PREFERENCES, RC_NAME);
		if(path == NULL)
			return;
		scripting_RunFormat("writeto(\"%s\")", path);
		free(path);
	}
#endif

	scripting_Run("save()");
	scripting_Run("write \"save_completed = 1\\n\"");
	scripting_Run("writeto()"); // select stdout again
}
