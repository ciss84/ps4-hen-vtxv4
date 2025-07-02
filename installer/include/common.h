#ifndef COMMON_H_
#define COMMON_H_

#include <types.h>

typedef unsigned int uid_t;
typedef unsigned int gid_t;

void write_blob(const char *path, const void *blob, const size_t blobsz);
void kill_proc(const char *proc);
void block_updates(void);
void RestoreJail(proc* proc, Backup_Jail jail);

#define printf_notification3(icon_uri, ...)    {                                                       \
  SceNotificationRequest noti_buffer;                                                                  \
  noti_buffer.type = 0;                                                                                \
  noti_buffer.unk3 = 0;                                                                                \
  noti_buffer.use_icon_image_uri = 1;                                                                  \
  noti_buffer.target_id = -1;                                                                          \
  snprintf_s(noti_buffer.uri, sizeof(noti_buffer.uri), icon_uri);                                     \
  snprintf_s(noti_buffer.message, sizeof(noti_buffer.message), ##__VA_ARGS__);                        \
  printf_debug("[NOTIFICATION]: %s\n", noti_buffer.message);                                           \
  sceKernelSendNotificationRequest(0, (SceNotificationRequest *)&noti_buffer, sizeof(noti_buffer), 0); \
}
#endif
