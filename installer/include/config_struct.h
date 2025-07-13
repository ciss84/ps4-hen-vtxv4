#ifndef CONFIG_STRUCT_H_
#define CONFIG_STRUCT_H_

#define TARGET_ID_SIZE 4 // eg. 0x84

struct configuration {
  int config_version;
  int exploit_fixes;
  int mmap_patches;
  int block_updates;
  int disable_aslr;
  int enable_browser;
  int kernel_clock;
  int fan;
  int temp;  
  int nobd_patches;
  int skip_patches;
  int upload_prx;
  int enable_ftp;  
  int enable_plugins;
  char target_id[TARGET_ID_SIZE + 1]; // Add null term
};

#endif
