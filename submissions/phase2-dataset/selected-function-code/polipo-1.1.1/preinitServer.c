void
preinitServer(void)
{
    CONFIG_VARIABLE_SETTABLE(parentProxy, CONFIG_ATOM_LOWER, parentProxySetter,
                    "Parent proxy (host:port).");
    CONFIG_VARIABLE(serverExpireTime, CONFIG_TIME,
                    "Time during which server data is valid.");
    CONFIG_VARIABLE_SETTABLE(smallRequestTime, CONFIG_TIME, configIntSetter,
                             "Estimated time for a small request.");
    CONFIG_VARIABLE_SETTABLE(replyUnpipelineTime, CONFIG_TIME, configIntSetter,
                             "Estimated time for a pipeline break.");
    CONFIG_VARIABLE_SETTABLE(replyUnpipelineSize, CONFIG_INT, configIntSetter,
                    "Size for a pipeline break.");
    CONFIG_VARIABLE_SETTABLE(pipelineAdditionalRequests, CONFIG_TRISTATE,
                             configIntSetter,
                             "Pipeline requests on an active connection.");
    CONFIG_VARIABLE_SETTABLE(maxPipelineTrain, CONFIG_INT,
                             configIntSetter,
                             "Maximum number of requests "
                             "pipelined at a time.");
    CONFIG_VARIABLE(pmmFirstSize, CONFIG_INT,
                    "The size of the first PMM chunk.");
    CONFIG_VARIABLE(pmmSize, CONFIG_INT,
                    "The size of a PMM chunk.");
    CONFIG_VARIABLE(serverSlots, CONFIG_INT,
                    "Maximum number of connections per server.");
    CONFIG_VARIABLE(serverSlots1, CONFIG_INT,
                    "Maximum number of connections per HTTP/1.0 server.");
    CONFIG_VARIABLE(serverMaxSlots, CONFIG_INT,
                    "Maximum number of connections per broken server.");
    CONFIG_VARIABLE(dontCacheRedirects, CONFIG_BOOLEAN,
                    "If true, don't cache redirects.");
    CONFIG_VARIABLE_SETTABLE(allowUnalignedRangeRequests,
                             CONFIG_BOOLEAN, configIntSetter,
                             "Allow unaligned range requests (unreliable).");
    CONFIG_VARIABLE_SETTABLE(maxSideBuffering,
                             CONFIG_INT, configIntSetter,
                             "Maximum buffering for PUT and POST requests.");
    CONFIG_VARIABLE_SETTABLE(maxConnectionAge,
                             CONFIG_TIME, configIntSetter,
                             "Maximum age of a server-side connection.");
    CONFIG_VARIABLE_SETTABLE(maxConnectionRequests,
                             CONFIG_INT, configIntSetter,
                             "Maximum number of requests on a server-side connection.");
    CONFIG_VARIABLE(alwaysAddNoTransform, CONFIG_BOOLEAN,
                    "If true, add a no-transform directive to all requests.");
}