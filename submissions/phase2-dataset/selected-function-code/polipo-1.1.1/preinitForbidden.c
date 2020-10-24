void
preinitForbidden(void)
{
    CONFIG_VARIABLE_SETTABLE(forbiddenUrl, CONFIG_ATOM, configAtomSetter,
                             "URL to which forbidden requests "
                             "should be redirected.");
    CONFIG_VARIABLE_SETTABLE(forbiddenRedirectCode, CONFIG_INT,
                             configIntSetter,
                             "Redirect code, 301 or 302.");
    CONFIG_VARIABLE_SETTABLE(forbiddenFile, CONFIG_ATOM, atomSetterForbidden,
                             "File specifying forbidden URLs.");
#ifndef NO_REDIRECTOR
    CONFIG_VARIABLE_SETTABLE(redirector, CONFIG_ATOM, atomSetterForbidden,
                             "Squid-style redirector.");
    CONFIG_VARIABLE_SETTABLE(redirectorRedirectCode, CONFIG_INT,
                             configIntSetter,
                             "Redirect code to use with redirector.");
#endif
    CONFIG_VARIABLE_SETTABLE(uncachableFile, CONFIG_ATOM, atomSetterForbidden,
                             "File specifying uncachable URLs.");

    CONFIG_VARIABLE_SETTABLE(forbiddenTunnelsFile, CONFIG_ATOM, atomSetterForbidden,
                             "File specifying forbidden tunnels.");
}