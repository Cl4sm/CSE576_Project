void 
preinitDiskcache()
{
    diskCacheRoot = internAtom(DISK_CACHE_ROOT);
    localDocumentRoot = internAtom(LOCAL_ROOT);

    CONFIG_VARIABLE_SETTABLE(diskCacheDirectoryPermissions, CONFIG_OCTAL,
                             configIntSetter,
                             "Access rights for new directories.");
    CONFIG_VARIABLE_SETTABLE(diskCacheFilePermissions, CONFIG_OCTAL,
                             configIntSetter,
                             "Access rights for new cache files.");
    CONFIG_VARIABLE_SETTABLE(diskCacheWriteoutOnClose, CONFIG_INT,
                             configIntSetter,
                             "Number of bytes to write out eagerly.");
    CONFIG_VARIABLE_SETTABLE(diskCacheRoot, CONFIG_ATOM, atomSetterFlush,
                             "Root of the disk cache.");
    CONFIG_VARIABLE_SETTABLE(localDocumentRoot, CONFIG_ATOM, atomSetterFlush,
                             "Root of the local tree.");
    CONFIG_VARIABLE_SETTABLE(maxDiskEntries, CONFIG_INT, maxDiskEntriesSetter,
                    "File descriptors used by the on-disk cache.");
    CONFIG_VARIABLE(diskCacheUnlinkTime, CONFIG_TIME,
                    "Time after which on-disk objects are removed.");
    CONFIG_VARIABLE(diskCacheTruncateTime, CONFIG_TIME,
                    "Time after which on-disk objects are truncated.");
    CONFIG_VARIABLE(diskCacheTruncateSize, CONFIG_INT, 
                    "Size to which on-disk objects are truncated.");
    CONFIG_VARIABLE(preciseExpiry, CONFIG_BOOLEAN,
                    "Whether to consider all files for purging.");
    CONFIG_VARIABLE_SETTABLE(maxDiskCacheEntrySize, CONFIG_INT,
                             configIntSetter,
                             "Maximum size of objects cached on disk.");
}