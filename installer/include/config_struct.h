#ifndef CONFIG_STRUCT_H_
#define CONFIG_STRUCT_H_

#define TARGET_ID_SIZE 4 // eg. 0x84

#include "common.h"

struct configuration {
  int exploit_fixes;
  int mmap_patches;
  int block_updates;
  int disable_aslr;
  int enable_browser;
  int kernel_clock;  
  int nobd_patches;
  int skip_patches;
  int upload_prx;
  int enable_plugins;
  char target_id[TARGET_ID_SIZE + 1]; // Add null term
};

struct Backup_Jail bkJail;

struct Backup_Jail
{
    struct prison* cr_prison;
    uid_t cr_uid;
    uid_t cr_ruid;
    gid_t cr_rgid;
    gid_t cr_groups;

    struct vnode* fd_jdir;
    struct vnode* fd_rdir;
};

#endif
