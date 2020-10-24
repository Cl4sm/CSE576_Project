static
struct super_block *hfs_read_super(struct super_block *sb,
				   void *data, int silent)
{
  char *opts = data;

  MOD_INC_USE_COUNT;

  lock_super(sb);

  set_blocksize(sb->s_dev, HFS_BLOCKSZ);

  sb->s_blocksize      = HFS_BLOCKSZ;
  sb->s_blocksize_bits = HFS_BLOCKSZ_BITS;
  sb->s_op             = &hfs_super_operations;
  sb->s_magic          = HFS_SIGWORD;
  sb->s_mounted        = 0;

  unlock_super(sb);

  return sb;
}
