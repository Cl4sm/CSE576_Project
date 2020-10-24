void
preinitObject()
{
    CONFIG_VARIABLE_SETTABLE(idleTime, CONFIG_TIME, configIntSetter,
                             "Time to remain idle before writing out.");
    CONFIG_VARIABLE_SETTABLE(maxWriteoutWhenIdle, CONFIG_INT, configIntSetter,
                             "Amount of data to write at a time when idle.");
    CONFIG_VARIABLE_SETTABLE(maxObjectsWhenIdle, CONFIG_INT, configIntSetter,
                             "Number of objects to write at a time "
                             "when idle.");
    CONFIG_VARIABLE_SETTABLE(cacheIsShared, CONFIG_BOOLEAN, configIntSetter,
                             "If false, ignore s-maxage and private.");
    CONFIG_VARIABLE_SETTABLE(mindlesslyCacheVary, CONFIG_BOOLEAN,
                             configIntSetter,
                             "If true, mindlessly cache negotiated objects.");
    CONFIG_VARIABLE(objectHashTableSize, CONFIG_INT,
                    "Size of the object hash table (0 = auto).");
    CONFIG_VARIABLE(objectHighMark, CONFIG_INT,
                    "High object count mark.");
    CONFIG_VARIABLE(publicObjectLowMark, CONFIG_INT,
                    "Low object count mark (0 = auto).");
    CONFIG_VARIABLE_SETTABLE(maxExpiresAge, CONFIG_TIME, configIntSetter,
                             "Max age for objects with Expires header.");
    CONFIG_VARIABLE_SETTABLE(maxAge, CONFIG_TIME, configIntSetter,
                             "Max age for objects without Expires header.");
    CONFIG_VARIABLE_SETTABLE(maxAgeFraction, CONFIG_FLOAT, configFloatSetter,
                             "Fresh fraction of modification time.");
    CONFIG_VARIABLE_SETTABLE(maxNoModifiedAge, CONFIG_TIME, configIntSetter,
                             "Max age for objects without Last-modified.");
    CONFIG_VARIABLE_SETTABLE(dontCacheCookies, CONFIG_BOOLEAN, configIntSetter,
                             "Work around cachable cookies.");
}