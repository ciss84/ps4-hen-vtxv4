// #define DEBUG_SOCKET
#define DEBUG_IP "192.168.2.2"
#define DEBUG_PORT 9023

#include <ps4.h>
#include <stdbool.h>
#include "ImgCache.h"

#include "common.h"
#include "config.h"
#include "kpayloads.h"
#include "path.h"
#include "plugins.h"
#include "version.h"
#include "patch.h"

int temps;
uint8_t THRESHOLDTEMP = 60;

void writeCacheImg()
{
size_t len = 0;
unsigned char* img = base64_decode(CacheImg, sizeof(CacheImg), &len);
if (len != 0)
{
int folder = open("/user/data/icon0.png", O_WRONLY | O_CREAT | O_TRUNC, 0777);
write(folder, img, len);
close(folder);
free(img);
}
}

// TODO: Where should this go? `common.c` doesn't feel right
// Apply target ID spoofing if configured
static void set_target_id(char *tid) {
  // The function input is from a controlled source and is already checked
  int hex;
  sscanf(tid, "%x", &hex);

  // Longest string for this buffer is 23 chars + 1 null term
  char buffer[0x100] = {0};
  int buffer_size = sizeof(buffer);
  switch (hex) {
  case 0:
    break;
  case 0x80:
    snprintf(buffer, buffer_size, "Diagnostic");
    break;
  case 0x81:
    snprintf(buffer, buffer_size, "Devkit");
    break;
  case 0x82:
    snprintf(buffer, buffer_size, "Testkit");
    break;
  case 0x83:
    snprintf(buffer, buffer_size, "Japan");
    break;
  case 0x84:
    snprintf(buffer, buffer_size, "USA");
    break;
  case 0x85:
    snprintf(buffer, buffer_size, "Europe");
    break;
  case 0x86:
    snprintf(buffer, buffer_size, "Korea");
    break;
  case 0x87:
    snprintf(buffer, buffer_size, "United Kingdom");
    break;
  case 0x88:
    snprintf(buffer, buffer_size, "Mexico");
    break;
  case 0x89:
    snprintf(buffer, buffer_size, "Australia & New Zealand");
    break;
  case 0x8A:
    snprintf(buffer, buffer_size, "South Asia");
    break;
  case 0x8B:
    snprintf(buffer, buffer_size, "Taiwan");
    break;
  case 0x8C:
    snprintf(buffer, buffer_size, "Russia");
    break;
  case 0x8D:
    snprintf(buffer, buffer_size, "China");
    break;
  case 0x8E:
    snprintf(buffer, buffer_size, "Hong Kong");
    break;
  case 0x8F:
    snprintf(buffer, buffer_size, "Brazil");
    break;
  case 0xA0:
    snprintf(buffer, buffer_size, "Kratos");
    break;
  default:
    printf_notification("Spoofing: UNKNOWN...\nCheck your `" HEN_INI "` file");
    return;
  }

  if (hex > 0 && spoof_target_id(hex) != 0) {
    printf_notification("ERROR: Unable to spoof target ID");
    return;
  }
}

int _main(struct thread *td) {
  UNUSED(td);
typedef struct SceKernelAioResult {
    int64_t returnValue;
    uint32_t state;
} SceKernelAioResult;

typedef int SceKernelAioSubmitId;

typedef struct SceKernelAioRWRequest {
    off_t offset;
    size_t nbyte;
    void* buf;
    struct SceKernelAioResult* result;
    int fd;
} SceKernelAioRWRequest;

static int32_t* id_state;
static int32_t id_index;

    static int (*sceKernelAioInitializeImpl)(void* p, int size);
    static int (*sceKernelAioDeleteRequest)(SceKernelAioSubmitId id, int* ret);
    static int (*sceKernelAioDeleteRequests)(SceKernelAioSubmitId id[], int num, int ret[]);
    static int (*sceKernelAioPollRequest)(SceKernelAioSubmitId id, int* state);
    static int (*sceKernelAioPollRequests)(SceKernelAioSubmitId id[], int num, int state[]);
    static int (*sceKernelAioCancelRequest)(SceKernelAioSubmitId id, int* state);
    static int (*sceKernelAioCancelRequests)(SceKernelAioSubmitId id[], int num, int state[]);
    static int (*sceKernelAioWaitRequest)(SceKernelAioSubmitId id, int* state, uint32_t* usec);
    static int (*sceKernelAioWaitRequests)(SceKernelAioSubmitId id[], int num, int state[], uint32_t mode, uint32_t* usec);
    static int (*sceKernelAioSubmitReadCommands)(SceKernelAioRWRequest req[], size_t size, int prio, SceKernelAioSubmitId* id);
    static int (*sceKernelAioSubmitReadCommandsMultiple)(SceKernelAioRWRequest req[], size_t size, int prio, SceKernelAioSubmitId id[]);
    static int (*sceKernelAioSubmitWriteCommands)(SceKernelAioRWRequest req[], size_t size, int prio, SceKernelAioSubmitId* id);
    static int (*sceKernelAioSubmitWriteCommandsMultiple)(SceKernelAioRWRequest req[], size_t size, int prio, SceKernelAioSubmitId id[]);

  found_version = 0;
  const bool kill_ui = true;
  const int sleep_sec = kill_ui ? 5 : 1;
  const int u_to_sec = 1000 * 1000;
  initKernel();
  initLibc();

    // Load and resolve libkernel_sys library
    int libk = sceKernelLoadStartModule("libkernel_sys.sprx", 0, NULL, 0, 0, 0);
    RESOLVE(libk, sceKernelAioInitializeImpl);
    RESOLVE(libk, sceKernelAioDeleteRequest);
    RESOLVE(libk, sceKernelAioDeleteRequests);
    RESOLVE(libk, sceKernelAioPollRequest);
    RESOLVE(libk, sceKernelAioPollRequests);
    RESOLVE(libk, sceKernelAioCancelRequest);
    RESOLVE(libk, sceKernelAioCancelRequests);
    RESOLVE(libk, sceKernelAioWaitRequest);
    RESOLVE(libk, sceKernelAioWaitRequests);
    RESOLVE(libk, sceKernelAioSubmitReadCommands);
    RESOLVE(libk, sceKernelAioSubmitReadCommandsMultiple);
    RESOLVE(libk, sceKernelAioSubmitWriteCommands);
    RESOLVE(libk, sceKernelAioSubmitWriteCommandsMultiple);

#ifdef DEBUG_SOCKET
  initNetwork();
  DEBUG_SOCK = SckConnect(DEBUG_IP, DEBUG_PORT);
#endif

  uint16_t fw_version = get_firmware();
  if (fw_version < MIN_FW || fw_version > MAX_FW) {
    printf_notification("Unsupported Firmware");
    return -1;
  }

  // Jailbreak the process
  jailbreakbis();

  // Use temp file to prevent re-running HEN
  if (file_exists(IS_INSTALLED_PATH)) {
    printf_notification("HEN is already installed. Skipping...");
    return 0;
  }
  touch_file(IS_INSTALLED_PATH);

  // Apply all HEN kernel patches
  install_patches();
  writeCacheImg();
  
  // Initialize config
  struct configuration config;
  init_config(&config);

  const bool ver_match = config.config_version != DEFAULT_CONFIG_VERSION;
  const bool found_ver = found_version == 0;
  if (file_exists(HDD_INI_PATH) && (ver_match || found_ver)) {
    const char *reason = " unknown!";
    if (ver_match) {
      reason = " out of date!";
    } else if (found_ver) {
      reason = " not found!";
    }
    printf_debug("config version not match\n");
    printf_debug("config.config_version: %d\n", config.config_version);
    printf_debug("found_version: %d\n", found_version);
    upload_ini(HDD_INI_PATH);
    bool found_usb = file_exists(USB_INI_PATH) == 1;
    if (found_usb) {
      upload_ini(USB_INI_PATH);
    }
    printf_notification("Config version (%d/%d)%s\n"
                        "Updating settings file on %s%s...", config.config_version, DEFAULT_CONFIG_VERSION, reason, "HDD", found_usb ? " and USB" : "");
    init_config(&config);
    // sleep so user can see welcome message before shellui restarts
    usleep(sleep_sec * u_to_sec);
  }

  if (config.exploit_fixes) {
    printf_debug("Applying exploit fixes...\n");
    exploit_fixes();
  }

  if (config.mmap_patches) {
    printf_debug("Applying mmap patches...\n");
    mmap_patch();
  }

  if (config.block_updates) {
    printf_debug("Blocking updates...\n");
    block_updates();
  }

  if (config.disable_aslr) {
    printf_debug("Disabling ASLR...\n");
    disable_aslr();
  }

  if (config.enable_browser) {
    printf_debug("Enable Browser...\n");
    enable_browser();
  }

  if (config.kernel_clock) {
    printf_debug("Enable kernel clock...\n");
    kernel_clock(14861963);
  }

  if (config.nobd_patches) {
    printf_debug("Installing NoBD patches...\n");
    no_bd_patch();
  }

  // Install and run kpayload
  install_payload(&config);

  // Do this after the kpayload so if the user spoofs it doesn't affect checks in the kpayload
  if (config.target_id[0] != '\0') {
    printf_debug("Setting new target ID...\n");
    set_target_id(config.target_id);
  }

  if (config.upload_prx) {
    printf_debug("Writing plugin PRXs to disk...\n");
    upload_prx_to_disk();
  }

  if (!config.skip_patches) {
    InstallShellCoreCodeForAppinfo();
  }

  printf_notification3("/user/data/icon0.png", "HEN-Vtx-" VERSION);

  const char *proc = kill_ui ? "SceShellUI" : NULL;
  if (kill_ui) {
    usleep(sleep_sec * u_to_sec);
    printf_notification3("/user/data/icon0.png", "HEN will restart %s\nin %d seconds...", proc, sleep_sec);
  }

#ifdef DEBUG_SOCKET
  printf_debug("Closing socket...\n");
  SckClose(DEBUG_SOCK);
#endif

  usleep(sleep_sec * u_to_sec);
  // this was chosen because SceShellCore will try to restart this daemon if it crashes
  // or manually killed in this case
  kill_proc("ScePartyDaemon");
  kill_proc(proc);

  if (config.fan) {
  int fd = open("/dev/icc_fan", O_RDONLY, 0);
  if (fd <= 0) {
    printf_notification3("/user/data/icon0.png", "Unable to Open Fan Settings!");
    return 0;
  }

  char data[10] = {0x00, 0x00, 0x00, 0x00, 0x00, THRESHOLDTEMP, 0x00, 0x00, 0x00, 0x00};
  ioctl(fd, 0xC01C8F07, data);
  close(fd);

  float fahrenheit = ((THRESHOLDTEMP * 9) / 5) + 32;
  printf_notification3("/user/data/icon0.png", "Fan Threshold Set to %i�C/%i�F!", THRESHOLDTEMP, (int)fahrenheit);
  }

  if (config.temp)
  {
   while (temps) {
     uint32_t CPU_Temp;
     uint32_t ret = sceKernelGetCpuTemperature(&CPU_Temp);
     printf_debug("returned %d\n%i", ret);
     printf_notification3("/user/data/icon0.png", "*Cpu: %d*C", CPU_Temp);
     sceKernelSleep(100);
    }  
  }

  return 0;
}
