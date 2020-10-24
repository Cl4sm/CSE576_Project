static int internal_upgrade_state_file(struct since_state *sn)
{
  char line[MAX_FMT], *ptr;
  int result, sw, sr, i, k, x, pad[3], end[3], sep[3];

#define bounded_delta(x, y)   (((x) < (y)) ? 0 : ((x) - (y)))

  pad[0] = bounded_delta(sn->s_arch_device, sn->s_disk_device) * 2;
  pad[1] = bounded_delta(sn->s_arch_inode, sn->s_disk_inode) * 2;
  pad[2] = bounded_delta(sn->s_arch_size, sn->s_disk_size) * 2;

  end[0] = sn->s_disk_device * 2;
  end[1] = sn->s_disk_inode * 2;
  end[2] = sn->s_disk_size * 2;

  sep[0] = ':';
  sep[1] = ':';
  sep[2] = '\n';

#undef bounded_delta

  sw = 0;
  sr = 0;
  for(x = 0; x < 3; x++){
    sw += pad[x] + end[x] + 1;
    sr += end[x] + 1;
#ifdef DEBUG
    fprintf(stderr, "since: padding[%d]: have=%d, expand=%d\n", x, end[x], pad[x]);
#endif
  }
#ifdef DEBUG
  fprintf(stderr, "since: total: %d->%d\n", sr, sw);
#endif
  if(sw >= MAX_FMT){
    fprintf(stderr, "since: major logic failure: want to rewrite to %d, limit at %d\n", sw, MAX_FMT);
    return -1;
  }

  for(i = 0; i < sn->s_size; i+= sr){
    ptr = sn->s_buffer + i;
    k = 0;
    for(x = 0; x < 3; x++){
      if(pad[x] > 0){
        memset(line + k, '0', pad[x]);
        k += pad[x];
      }
      memcpy(line + k, ptr, end[x]);
      if(ptr[end[x]] != sep[x]){
        fprintf(stderr, "since: data corruption: expected a separator, not 0x%x in line at %d in %s\n", ptr[end[x]], i, sn->s_name);
        return -1;
      }
      k += end[x];
      line[k] = sep[x];
      k++;
      ptr += end[x] + 1;
    }
    result = write(sn->s_fd, line, k);
    if(result != sw){
      fprintf(stderr, "since: unable to write line to %s: %s\n", sn->s_name, (result < 0) ? strerror(errno) : "incomplete write");
      return -1;
    }
  }

  sn->s_disk_device += (pad[0] / 2);
  sn->s_disk_inode += (pad[1] / 2);
  sn->s_disk_size += (pad[2] / 2);

  if(lseek(sn->s_fd, 0, SEEK_SET) != 0){
    fprintf(stderr, "since: unable to rewind tmp file to start: %s\n", strerror(errno));
    return -1;
  }

  return 0;
}