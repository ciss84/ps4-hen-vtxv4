#include <ps4.h>

#include "common.h"

#define PS4UPDATE_FILE "/update/PS4UPDATE.PUP"
#define PS4UPDATE_TEMP_FILE "/update/PS4UPDATE.PUP.net.temp"

void write_blob(const char *path, const void *blob, const size_t blobsz) {
  if (!path || !blob || blobsz == 0) {
    printf_notification("Invalid parameters for write_blob");
    return;
  }

  int fd = open(path, O_CREAT | O_RDWR, 0777);
  printf_debug("fd %s %d\n", path, fd);
  if (fd > 0) {
    ssize_t written = write(fd, blob, blobsz);
    if (written != (ssize_t)blobsz) {
      printf_notification("Partial write to %s: %zd/%zu bytes", path, written, blobsz);
    }
    close(fd);
  } else {
    printf_notification("Failed to write %s!\nFile descriptor %d", path, fd);
  }
}

void kill_proc(const char *proc) {
  if (!proc) {
    return;
  }
  const int party = findProcess(proc);
  printf_debug("%s %d\n", proc, party);
  if (party > 0) {
    const int k = kill(party, SIGKILL);
    printf_debug("sent SIGKILL(%d) to %s(%d)\n", k, proc, party);
  }
}

/*void RestoreJail(struct proc* proc, struct Backup_Jail jail)
{
    if(proc)
    {
        struct ucred* cred = proc->p_ucred;
        struct filedesc* fd = proc->p_fd;

        if(!cred || !fd)
            return;

        cred->cr_prison = jail.cr_prison;
        cred->cr_uid = jail.cr_uid;
        cred->cr_ruid = jail.cr_ruid;
        cred->cr_rgid = jail.cr_rgid;
        cred->cr_groups[0] = jail.cr_groups;

        fd->fd_jdir = jail.fd_jdir;
        fd->fd_rdir = jail.fd_rdir;
    }
}*/

void block_updates(void) {
  // Delete existing updates/blocker and recreate
  unlink(PS4UPDATE_FILE);
  rmdir(PS4UPDATE_FILE);
  mkdir(PS4UPDATE_FILE, 777);

  unlink(PS4UPDATE_TEMP_FILE);
  rmdir(PS4UPDATE_TEMP_FILE);
  mkdir(PS4UPDATE_TEMP_FILE, 777);

  // Unmount update directory. From etaHEN
  if ((int)unmount("/update", 0x80000LL) < 0) {
    unmount("/update", 0);
  }
}
