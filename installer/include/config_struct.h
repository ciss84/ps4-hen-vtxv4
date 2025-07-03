#ifndef CONFIG_STRUCT_H_
#define CONFIG_STRUCT_H_

#define TARGET_ID_SIZE 4 // eg. 0x84

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

/*struct Backup_Jail bkJail;

struct Backup_Jail
{
    struct prison* cr_prison;
    int cr_uid;
    int cr_ruid;
    int cr_rgid;
    int cr_groups;

    struct vnode* fd_jdir;
    struct vnode* fd_rdir;
};*/

#endif
