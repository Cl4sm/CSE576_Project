void
preinitHttp()
{
    proxyAddress = internAtom("127.0.0.1");
    CONFIG_VARIABLE_SETTABLE(disableProxy, CONFIG_BOOLEAN, configIntSetter,
                             "Whether to be a web server only.");
    CONFIG_VARIABLE_SETTABLE(proxyOffline, CONFIG_BOOLEAN, configIntSetter,
                             "Avoid contacting remote servers.");
    CONFIG_VARIABLE_SETTABLE(relaxTransparency, CONFIG_TRISTATE, 
                             configIntSetter,
                             "Avoid contacting remote servers.");
    CONFIG_VARIABLE(proxyPort, CONFIG_INT,
                    "The TCP port on which the proxy listens.");
    CONFIG_VARIABLE(proxyAddress, CONFIG_ATOM_LOWER,
                    "The IP address on which the proxy listens.");
    CONFIG_VARIABLE_SETTABLE(proxyName, CONFIG_ATOM_LOWER, configAtomSetter,
                             "The name by which the proxy is known.");
    CONFIG_VARIABLE_SETTABLE(clientTimeout, CONFIG_TIME, 
                             timeoutSetter, "Client-side timeout.");
    CONFIG_VARIABLE_SETTABLE(serverTimeout, CONFIG_TIME,
                             timeoutSetter, "Server-side timeout.");
    CONFIG_VARIABLE_SETTABLE(serverIdleTimeout, CONFIG_TIME,
                             timeoutSetter, "Server-side idle timeout.");
    CONFIG_VARIABLE(authRealm, CONFIG_ATOM,
                    "Authentication realm.");
    CONFIG_VARIABLE(displayName, CONFIG_ATOM,
                    "Server name displayed on error pages.");
    CONFIG_VARIABLE(authCredentials, CONFIG_PASSWORD,
                    "username:password.");
    CONFIG_VARIABLE(parentAuthCredentials, CONFIG_PASSWORD,
                    "username:password.");
    CONFIG_VARIABLE(allowedClients, CONFIG_ATOM_LIST_LOWER,
                    "Networks from which clients are allowed to connect.");
    CONFIG_VARIABLE(tunnelAllowedPorts, CONFIG_INT_LIST,
                    "Ports to which tunnelled connections are allowed.");
    CONFIG_VARIABLE(allowedPorts, CONFIG_INT_LIST,
                    "Ports to which connections are allowed.");
    CONFIG_VARIABLE(expectContinue, CONFIG_TRISTATE,
                    "Send Expect-Continue to servers.");
    CONFIG_VARIABLE(bigBufferSize, CONFIG_INT,
                    "Size of big buffers (max size of headers).");
    CONFIG_VARIABLE_SETTABLE(disableVia, CONFIG_BOOLEAN, configIntSetter,
                             "Don't use Via headers.");
    CONFIG_VARIABLE(dontTrustVaryETag, CONFIG_TRISTATE,
                    "Whether to trust the ETag when there's Vary.");
    preinitHttpParser();
}