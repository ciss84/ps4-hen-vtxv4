#include <ps4.h>

#include "config_struct.h"
#include "offsets.h"

#include "kpayloads.h"

#include "kpayload.inc.c"

#define KERNEL_BASE_FILE 0xFFFFFFFF82200000

#define patch_macro(x)                                                                           \
  kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K##x##_XFAST_SYSCALL];                      \
  kernel_ptr = (uint8_t *)kernel_base;                                                           \
  copyin_patch_1 = &kernel_ptr[K##x##_COPYIN_PATCH_1];                                           \
  copyin_patch_2 = &kernel_ptr[K##x##_COPYIN_PATCH_2];                                           \
  copyout_patch_1 = &kernel_ptr[K##x##_COPYOUT_PATCH_1];                                         \
  copyout_patch_2 = &kernel_ptr[K##x##_COPYOUT_PATCH_2];                                         \
  copyinstr_patch_1 = &kernel_ptr[K##x##_COPYINSTR_PATCH_1];                                     \
  copyinstr_patch_2 = &kernel_ptr[K##x##_COPYINSTR_PATCH_2];                                     \
  copyinstr_patch_3 = &kernel_ptr[K##x##_COPYINSTR_PATCH_3];                                     \
  setlogin_patch = &kernel_ptr[K##x##_SETLOGIN_PATCH];                                           \
  Patch_setuid = &kernel_ptr[K##x##_PATCH_SETUID];                                               \
  dispsw_patch = &kernel_ptr[K##x##_DIPSW_PATCH];                                                \
  debug_settings_error_patch_1 = &kernel_ptr[K##x##_DEBUG_SETTINGS_ERROR_PATCH_1];               \
  debug_settings_error_patch_2 = &kernel_ptr[K##x##_DEBUG_SETTINGS_ERROR_PATCH_2];               \
  pfs_signature_check_patch = &kernel_ptr[K##x##_PFS_SIGNATURE_CHECK_PATCH];                     \
  debug_rif_patch_1 = &kernel_ptr[K##x##_DEBUG_RIF_PATCH_1];                                     \
  debug_rif_patch_2 = &kernel_ptr[K##x##_DEBUG_RIF_PATCH_2];                                     \
  uart_patch = &kernel_ptr[K##x##_UART_PATCH];                                                   \
  depth_limit_patch = &kernel_ptr[K##x##_DEPTH_LIMIT_PATCH];

#define install_macro(x)                                                    \
  kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K##x##_XFAST_SYSCALL]; \
  kernel_ptr = (uint8_t *)kernel_base;                                      \
  kernel_pmap_store = &kernel_ptr[K##x##_PMAP_STORE];                       \
  pmap_protect_p_patch = &kernel_ptr[K##x##_PMAP_PROTECT_P];                \
  pmap_protect = (void *)(kernel_base + K##x##_PMAP_PROTECT);

#define jailbreakbis_macro(x)                                               \
  kernel_base = &((uint8_t *)__readmsr(0xC0000082))[-K##x##_XFAST_SYSCALL]; \
  kernel_ptr = (uint8_t *)kernel_base;                                      \
  prison0 = (void **)&kernel_ptr[K##x##_PRISON_0];                          \
  rootvnode = (void **)&kernel_ptr[K##x##_ROOTVNODE];

struct kpayload_payload_header {
  uint64_t signature;
  size_t entrypoint_offset;
};

struct kpayload_payload_info {
  uint16_t fw_version;
  struct configuration config;
  uint8_t *buffer;
  size_t size;
};

struct kpayload_install_payload_args {
  void *syscall_handler;
  struct kpayload_payload_info *kpayload_payload_info;
};

// Return 0 on success
// Return -1 on unsupported firmware error
// Can also just give a memory error in the browser or panic the console on failure
static int kpayload_patches(struct thread *td, struct kpayload_firmware_args *args) {
  UNUSED(td);
  void *kernel_base;
  uint8_t *kernel_ptr;

  // Use "kmem" for all patches
  uint8_t *kmem;

  // Pointers to be assigned in build_kpayload macro
  uint8_t *copyin_patch_1;
  uint8_t *copyin_patch_2;
  uint8_t *copyout_patch_1;
  uint8_t *copyout_patch_2;
  uint8_t *copyinstr_patch_1;
  uint8_t *copyinstr_patch_2;
  uint8_t *copyinstr_patch_3;
  uint8_t *setlogin_patch;
  //uint8_t *panic_patch;
  uint8_t *Patch_setuid;
  uint8_t *dispsw_patch;  
  uint8_t *debug_settings_error_patch_1;
  uint8_t *debug_settings_error_patch_2;
  uint8_t *pfs_signature_check_patch;
  uint8_t *debug_rif_patch_1;
  uint8_t *debug_rif_patch_2;
  //uint8_t *enable_ptrace_patch1;
  //uint8_t *sys_dynlib_dlsym_patch1;
  //uint8_t *dynlib_patch_1;
  //uint8_t *dynlib_patch_2;
  //uint8_t *enable_ptrace_patch2; 
  //uint8_t *enable_debug_log_patch;  
  uint8_t *uart_patch;  
  uint8_t *depth_limit_patch;

  uint16_t fw_version = args->kpayload_firmware_info->fw_version;

  // NOTE: This is a C preprocessor macro
  build_kpayload(fw_version, patch_macro);

  // Disable write protection
  uint64_t cr0 = readCr0();
  writeCr0(cr0 & ~X86_CR0_WP);

  // Patch copyin/copyout/copyinstr to allow userland + kernel addresses in both params
  // copyin
  kmem = (uint8_t *)copyin_patch_1;
  kmem[0] = 0xEB;
  kmem[1] = 0x00;

  if (fw_version >= 550) {
    kmem = (uint8_t *)copyin_patch_2;
    kmem[0] = 0xEB;
    kmem[1] = 0x01;
  }

  kmem = (uint8_t *)copyout_patch_1;
  kmem[0] = 0xEB;
  kmem[1] = 0x00;

  if (fw_version >= 550) {
    kmem = (uint8_t *)copyout_patch_2;
    kmem[0] = 0xEB;
    kmem[1] = 0x01;
  }

  // copyinstr
  kmem = (uint8_t *)copyinstr_patch_1;
  kmem[0] = 0xEB;
  kmem[1] = 0x00;

  if (fw_version >= 550) {
    kmem = (uint8_t *)copyinstr_patch_2;
    kmem[0] = 0xEB;
    kmem[1] = 0x01;
  }

  kmem = (uint8_t *)copyinstr_patch_3;
  kmem[0] = 0xEB;
  kmem[1] = 0x00;

  // setlogin patch (for autolaunch check)
  kmem = (uint8_t *)setlogin_patch;
  kmem[0] = 0x48;
  kmem[1] = 0x31;
  kmem[2] = 0xC0;
  kmem[3] = 0xEB;
  kmem[4] = 0x00;

  //panic
 	/*kmem = (uint8_t *)panic_patch;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;*/
  
  // Patch setuid: Don't run kernel exploit more than once/privilege escalation
	kmem = (uint8_t *)Patch_setuid;
	kmem[0] = 0xB8;
	kmem[1] = 0x00;
	kmem[2] = 0x00;
	kmem[3] = 0x00;
	kmem[4] = 0x00;
	//kmem = (uint8_t *)target_id;
	//kmem[0] = 0x82;  
	kmem = (uint8_t *)dispsw_patch;
	kmem[0x36] |= 0x14;
	kmem[0x59] |= 0x02;
	kmem[0x5A] |= 0x00;
	kmem[0x78] |= 0x00;

  // Patch debug setting errors
  kmem = (uint8_t *)debug_settings_error_patch_1;
  kmem[0] = 0x00;
  kmem[1] = 0x00;
  kmem[2] = 0x00;
  kmem[3] = 0x00;

  kmem = (uint8_t *)debug_settings_error_patch_2;
  kmem[0] = 0x00;
  kmem[1] = 0x00;
  kmem[2] = 0x00;
  kmem[3] = 0x00;

  // Disable PFS signature check
  kmem = (uint8_t *)pfs_signature_check_patch;
  kmem[0] = 0x31;
  kmem[1] = 0xC0;
  kmem[2] = 0xC3;

  // Enable debug RIFs
  kmem = (uint8_t *)debug_rif_patch_1;
  kmem[0] = 0xB0;
  kmem[1] = 0x01;
  kmem[2] = 0xC3;

  kmem = (uint8_t *)debug_rif_patch_2;
  kmem[0] = 0xB0;
  kmem[1] = 0x01;
  kmem[2] = 0xC3;

  // ptrace patches	
	/*kmem = (uint8_t*)enable_ptrace_patch1;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	// flatz Patch sys_dynlib_dlsym: Allow from anywhere
	kmem = (uint8_t *)sys_dynlib_dlsym_patch1;
	kmem[0] = 0xEB;
	kmem[1] = 0x4C;

  kmem = (uint8_t *)dynlib_patch_1;
  kmem[0] = 0xE9;
  kmem[1] = 0x90;
 
  kmem = (uint8_t *)dynlib_patch_2;
  kmem[0] = 0x48;
  kmem[1] = 0x31;
  kmem[2] = 0xC0;
  kmem[3] = 0xC3;

	// second ptrace patch
	// via DeathRGH
	kmem = (uint8_t *)enable_ptrace_patch2;
	kmem[0] = 0xE9;
	kmem[1] = 0x7C;
	kmem[2] = 0x02;
	kmem[3] = 0x00;
	kmem[4] = 0x00;

	// Enable *all* debugging logs (in vprintf)
	// Patch by: SiSTRo
	kmem = (uint8_t *)enable_debug_log_patch;
	kmem[0] = 0xEB;
	kmem[1] = 0x3B;*/

  // Enable UART
  kmem = (uint8_t *)uart_patch;
	kmem[0] = 0x00;
	kmem[1] = 0x00;
	kmem[2] = 0x00;
	kmem[3] = 0x00;

  // Change directory depth limit from 9 to 64
  kmem = (uint8_t *)depth_limit_patch;
  kmem[0] = 0x40;

  // Restore write protection
  writeCr0(cr0);

  return 0;
}

static void get_memory_dump2(uintptr_t addr, void *out, uint64_t outsz) {
  uint8_t *pout = (uint8_t *)out;
  uint8_t *paddr = (uint8_t *)addr;
  for (uint64_t o = 0; o < outsz; o++) {
    pout[o] = paddr[o];
  }
}

static uint64_t get_kernel_size(uint64_t kernel_base) {
  uint16_t elf_header_size;       // ELF header size
  uint16_t elf_header_entry_size; // ELF header entry size
  uint16_t num_of_elf_entries;    // Number of entries in the ELF header

  get_memory_dump2(kernel_base + 0x34, &elf_header_size, sizeof(uint16_t));
  get_memory_dump2(kernel_base + 0x34 + sizeof(uint16_t), &elf_header_entry_size, sizeof(uint16_t));
  get_memory_dump2(kernel_base + 0x34 + (sizeof(uint16_t) * 2), &num_of_elf_entries, sizeof(uint16_t));

  // printf_debug("elf_header_size: %u bytes\n", elf_header_size);
  // printf_debug("elf_header_entry_size: %u bytes\n", elf_header_entry_size);
  // printf_debug("num_of_elf_entries: %u\n", num_of_elf_entries);

  uint64_t max = 0;
  for (int i = 0; i < num_of_elf_entries; i++) {
    uint64_t temp_memsz;
    uint64_t temp_vaddr;
    uint64_t temp_align;
    uint64_t temp_max;

    uint64_t memsz_offset = elf_header_size + (i * elf_header_entry_size) + 0x28;
    uint64_t vaddr_offset = elf_header_size + (i * elf_header_entry_size) + 0x10;
    uint64_t align_offset = elf_header_size + (i * elf_header_entry_size) + 0x30;
    get_memory_dump2(kernel_base + memsz_offset, &temp_memsz, sizeof(uint64_t));
    get_memory_dump2(kernel_base + vaddr_offset, &temp_vaddr, sizeof(uint64_t));
    get_memory_dump2(kernel_base + align_offset, &temp_align, sizeof(uint64_t));

    temp_vaddr -= kernel_base;
    temp_vaddr += KERNEL_BASE_FILE;

    temp_max = (temp_vaddr + temp_memsz + (temp_align - 1)) & ~(temp_align - 1);

    if (temp_max > max) {
      max = temp_max;
    }
  }

  return max - KERNEL_BASE_FILE;
}

static uint64_t *u64_Scan(const void *module, uint64_t size_of_image, uint64_t value) {
  uint8_t *scanBytes = (uint8_t *)module;
  for (size_t i = 0; i < size_of_image; ++i) {
    uint64_t currentValue = *(uint64_t *)&scanBytes[i];
    if (currentValue == value) {
      return (uint64_t *)&scanBytes[i];
    }
  }
  return 0;
}

// Return 0 on success
// Return -1 on memory allocation error or unsupported firmware error
// Can also just give a memory error in the browser or panic the console on failure
static int kpayload_install_payload(struct thread *td, struct kpayload_install_payload_args *args) {
  UNUSED(td);
  void *kernel_base;
  uint8_t *kernel_ptr;

  // Use "kmem" for all patches
  uint8_t *kmem;

  // Pointers to be assigned in build_kpayload macro
  void *kernel_pmap_store;
  uint8_t *pmap_protect_p_patch;
  uint8_t *payload_buffer;

  void (*pmap_protect)(void *pmap, uint64_t sva, uint64_t eva, uint8_t pr);

  uint16_t fw_version = args->kpayload_payload_info->fw_version;
  struct configuration config = args->kpayload_payload_info->config;

  // NOTE: This is a C preprocessor macro
  build_kpayload(fw_version, install_macro);

  const uint64_t kptr = (uint64_t)kernel_ptr;
  const uint64_t kernelsz = get_kernel_size(kptr);
  if (!kptr || !kernelsz) {
    return -1;
  }
  const uint64_t SCE_RELA_tag = 0x6100002F;
  const uintptr_t *sce_reloc = u64_Scan(kernel_base, kernelsz, SCE_RELA_tag);
  // discard old value
  payload_buffer = 0;
  if (sce_reloc) {
    payload_buffer = (uint8_t *)(kptr + (sce_reloc[1] - KERNEL_BASE_FILE));
  } else {
    return -1;
  }
  if (!payload_buffer) {
    return -1;
  }

  uint8_t *payload_data = args->kpayload_payload_info->buffer;
  size_t payload_size = args->kpayload_payload_info->size;

  struct kpayload_payload_header *payload_header = (struct kpayload_payload_header *)payload_data;

  if (!payload_data || payload_size < sizeof(payload_header) || payload_header->signature != 0x5041594C4F414458ull) { // `payloadx`
    return -1;
  }

  // Disable write protection
  uint64_t cr0 = readCr0();
  writeCr0(cr0 & ~X86_CR0_WP);

  memset(payload_buffer, '\0', PAGE_SIZE);
  memcpy(payload_buffer, payload_data, payload_size);

  uint64_t sss = ((uint64_t)payload_buffer) & ~(uint64_t)(PAGE_SIZE - 1);
  uint64_t eee = ((uint64_t)payload_buffer + payload_size + PAGE_SIZE - 1) & ~(uint64_t)(PAGE_SIZE - 1);

  kmem = (uint8_t *)pmap_protect_p_patch;
  kmem[0] = 0xEB;
  pmap_protect(kernel_pmap_store, sss, eee, 7);
  kmem = (uint8_t *)pmap_protect_p_patch;  
  kmem[0] = 0x75;

  // Restore write protection
  writeCr0(cr0);

  int (*payload_entrypoint)(uint16_t, struct configuration);
  *((void **)&payload_entrypoint) = (void *)(&payload_buffer[payload_header->entrypoint_offset]);

  return payload_entrypoint(fw_version, config);
}

// HACK: Fix missing/bad/conflicting exploit patches for supported FWs //////////////////////////////////////////////////////
// Lua+Lapse and PSFree+Lapse have the correct patch from 7.00-12.02, every FW *should* match these
// Try to get these patches fixed/added upstream if possible
// It's hard to tell with ps4jb2 because so many people forked/tweaked it
//     Try to get it fixed in the "official" release and assume hosts will update
// Check these:
// 5.00, 5.01, 5.03, 5.05, 5.07                                          // PS4-5.05-Kernel-Exploit, ps4-ipv6-uaf
// 6.00, 6.02, 6.20, 6.50, 6.51,                                         // ps4-ipv6-uaf
// 6.70, 6.71, 6.72                                                      // ps4-ipv6-uaf, ps4jb2
// 7.00, 7.01, 7.02, 7.50, 7.51, 7.55                                    // ps4-ipv6-uaf, ps4jb2, pppwn
// 8.00, 8.01, 8.03, 8.50, 8.52                                          // pppwn
// 9.00                                                                  // pOOBs4, pppwn
// 9.03, 9.04, 9.50, 9.51, 9.60                                          // pppwn
// 10.00, 10.01, 10.50, 10.70, 10.71                                     // pppwn
// 11.00                                                                 // pppwn
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int kpayload_exploit_fixes(struct thread *td, struct kpayload_firmware_args *args) {
  UNUSED(td);
  void *kernel_base;
  uint8_t *kernel_ptr;

  // Use "kmem" for all patches
  uint8_t *kmem;

  uint16_t fw_version = args->kpayload_firmware_info->fw_version;

  // NOTE: This is a C preprocessor macro
  build_kpayload(fw_version, kernel_ptr_macro);

  // Disable write protection
  uint64_t cr0 = readCr0();
  writeCr0(cr0 & ~X86_CR0_WP);

  // patch sys_dynlib_dlsym() to allow dynamic symbol resolution everywhere
  if (fw_version >= 505 && fw_version <= 507) {
    // Cryptogenic/PS4-5.05-Kernel-Exploit: ????
    // ChendoChap/ps4-ipv6-uaf:             ????
    kmem = (uint8_t *)&kernel_ptr[0x00237F3A];
    kmem[0] = 0x90;
    kmem[1] = 0xE9;
    kmem[2] = 0xC0;
    kmem[3] = 0x01;
    kmem[4] = 0x00;
    kmem[5] = 0x00;
  } else if (fw_version == 672) {
    // ChendoChap/ps4-ipv6-uaf: Good
    // sleirsgoevy/ps4jb2:      Bad/Missing?
    kmem = (uint8_t *)&kernel_ptr[0x001D895A];
    kmem[0] = 0x90;
    kmem[1] = 0xE9;
    kmem[2] = 0xC6;
    kmem[3] = 0x01;
    kmem[4] = 0x00;
    kmem[5] = 0x00;
  } else if (fw_version >= 700 && fw_version <= 702) {
    // ChendoChap/ps4-ipv6-uaf: Good
    // sleirsgoevy/ps4jb2:      Bad/Missing?
    // TheOfficialFloW/PPPwn:   Missing
    kmem = (uint8_t *)&kernel_ptr[0x0009547B];
    kmem[0] = 0x90;
    kmem[1] = 0xE9;
    kmem[2] = 0xBC;
    kmem[3] = 0x01;
    kmem[4] = 0x00;
    kmem[5] = 0x00;
  } else if (fw_version >= 750 && fw_version <= 7.55) {
    // ChendoChap/ps4-ipv6-uaf: Good
    // sleirsgoevy/ps4jb2:      Bad/Missing?
    // TheOfficialFloW/PPPwn:   Missing
    kmem = (uint8_t *)&kernel_ptr[0x004523C4];
    kmem[0] = 0x90;
    kmem[1] = 0xE9;
    kmem[2] = 0xC7;
    kmem[3] = 0x01;
    kmem[4] = 0x00;
    kmem[5] = 0x00;
  }

  // Restore write protection
  writeCr0(cr0);

  return 0;
}

int kpayload_jailbreakbis(struct thread *td, struct kpayload_firmware_args *args) {
  struct filedesc *fd;
  struct ucred *cred;
  fd = td->td_proc->p_fd;
  cred = td->td_proc->p_ucred;

  void *kernel_base;
  uint8_t *kernel_ptr;
  void **prison0;
  void **rootvnode;

  uint16_t fw_version = args->kpayload_firmware_info->fw_version;

  // NOTE: This is a C preprocessor macro
  build_kpayload(fw_version, jailbreakbis_macro);

  cred->cr_uid = 0;
  cred->cr_ruid = 0;
  cred->cr_rgid = 0;
  cred->cr_groups[0] = 0;

  cred->cr_prison = *prison0;
  fd->fd_rdir = fd->fd_jdir = *rootvnode;

	// escalate ucred privs, needed for access to the filesystem ie* mounting & decrypting files
	void *td_ucred = *(void **)(((char *)td) + 304); // p_ucred == td_ucred

	// sceSblACMgrIsSystemUcred
	uint64_t *sonyCred = (uint64_t *)(((char *)td_ucred) + 96);
	*sonyCred = 0xffffffffffffffff;

	// sceSblACMgrGetDeviceAccessType
	uint64_t *sceProcType = (uint64_t *)(((char *)td_ucred) + 88);
	*sceProcType = 0x3801000000000013; // Max access

	// sceSblACMgrGetDeviceAccessType
	uint64_t *sceRemotePlay = (uint64_t *)(((char *)td_ucred) + 88);
	*sceRemotePlay = 0x3800000000000019; // SceRemotePlay
	
	// sceSblACMgrGetDeviceAccessType
	uint64_t *sceOSUPDATE = (uint64_t *)(((char *)td_ucred) + 88);
	*sceOSUPDATE = 0x3801000000000024; // sceOSUPDATE
	
	// sceSblACMgrGetDeviceAccessType
	uint64_t *scevtr = (uint64_t *)(((char *)td_ucred) + 88);
	*scevtr = 0x3800800000000002; // scevtr

	// sceSblACMgrGetDeviceAccessType
	uint64_t *NPXS20103 = (uint64_t *)(((char *)td_ucred) + 88);
	*NPXS20103 = 0x3800000000000011; // NPXS20103
	
	// sceSblACMgrIsAllowedToUseUNK_PFS
	uint64_t *UNK_PFS = (uint64_t *)(((char *)td_ucred) + 88);
	*UNK_PFS = 0x380100000000000A; // UNK_PFS

	// sceSblACMgrIsAllowedToUseUNK_ICC
	uint64_t *UNK_ICC = (uint64_t *)(((char *)td_ucred) + 88);
	*UNK_ICC = 0x3800800000000024; // UNK_ICC
	
	// sceSblACMgrIsAllowedToUsePupUpdate0
	uint64_t *PupUpdate0 = (uint64_t *)(((char *)td_ucred) + 88);
	*PupUpdate0 = 0x3800100000000001; // PupUpdate0
	
	// sceSblACMgrIsAllowedToUseSecureWebProcess
	uint64_t *SecureWebProcess = (uint64_t *)(((char *)td_ucred) + 88);
	*SecureWebProcess = 0x3800000010000003; // SecureWebProcess
	
	// sceSblACMgrIsAllowedToUseSceNKWebProcess
	uint64_t *SceNKWebProcess = (uint64_t *)(((char *)td_ucred) + 88);
	*SceNKWebProcess = 0x3800000000010003; // SceNKWebProcess

	// sceSblACMgrIsAllowedToUseSecureUIProcess
	uint64_t *SecureUIProcess = (uint64_t *)(((char *)td_ucred) + 88);
	*SecureUIProcess = 0x3800000000000033; // SecureUIProcess
	
	// sceSblACMgrIsAllowedToUseSceNKUIProcess
	uint64_t *SceNKUIProcess = (uint64_t *)(((char *)td_ucred) + 88);
	*SceNKUIProcess = 0x380000000000003c; // SceNKUIProcess
	
	// sceSblACMgrIsAllowedToUseSceSysAvControl
	uint64_t *SceSysAvControl = (uint64_t *)(((char *)td_ucred) + 88);
	*SceSysAvControl = 0x380000000000001; // SceSysAvControl

	// sceSblACMgrIsAllowedToUseSceShellUI
	uint64_t *SceShellUI = (uint64_t *)(((char *)td_ucred) + 88);
	*SceShellUI = 0x380000000000000f; // SceShellUI

	// sceSblACMgrIsAllowedToUseSceShellCore
	uint64_t *SceShellCore = (uint64_t *)(((char *)td_ucred) + 88);
	*SceShellCore = 0x3800000000000010; // SceShellCore

	// sceSblACMgrIsAllowedToUseDecid
	uint64_t *Decid = (uint64_t *)(((char *)td_ucred) + 88);
	*Decid = 0x3800000000010003; // Decid

	// sceSblACMgrGetDeviceSceVdecProxy
	uint64_t *SceVdecProxy = (uint64_t *)(((char *)td_ucred) + 88);
	*SceVdecProxy = 0x3800000000000003; // SceVdecProxy

	// sceSblACMgrGetDeviceSceVencProxy
	uint64_t *SceVencProxy = (uint64_t *)(((char *)td_ucred) + 88);
	*SceVencProxy = 0x3800000000000004; // SceVencProxy
	
	// sceSblACMgrGetDeviceOrbisaudiod
	uint64_t *Orbisaudiod = (uint64_t *)(((char *)td_ucred) + 88);
	*Orbisaudiod = 0x3800000000000005; // Orbisaudiod
	
	// sceSblACMgrGetDeviceCoredump
	uint64_t *Coredump = (uint64_t *)(((char *)td_ucred) + 88);
	*Coredump = 0x3800000000000006; // Coredump

	// sceSblACMgrGetDeviceOrbissetip
	uint64_t *Orbissetip = (uint64_t *)(((char *)td_ucred) + 88);
	*Orbissetip = 0x3800000000000008; // Orbissetip

	// sceSblACMgrIsAllowedToUseGnmCompositor
	uint64_t *GnmCompositor = (uint64_t *)(((char *)td_ucred) + 88);
	*GnmCompositor = 0x3800000000000009; // GnmCompositor

	// sceSblACMgrIsAllowedToUseSceGameLiveStreaming
	uint64_t *SceGameLiveStreaming = (uint64_t *)(((char *)td_ucred) + 88);
	*SceGameLiveStreaming = 0x3800000000000012; // SceGameLiveStreaming
	
	// sceSblACMgrIsAllowedToUseSCE_SYS_SERVICES
	uint64_t *SCE_SYS_SERVICES = (uint64_t *)(((char *)td_ucred) + 88);
	*SCE_SYS_SERVICES = 0x3800000000010003; // SCE_SYS_SERVICES
	
	// sceSblACMgrIsAllowedToUseScePartyDaemon
	uint64_t *ScePartyDaemon = (uint64_t *)(((char *)td_ucred) + 88);
	*ScePartyDaemon = 0x3800000000000014; // ScePartyDaemon

	// sceSblACMgrIsAllowedToUseSceAvCapture
	uint64_t *SceAvCapture = (uint64_t *)(((char *)td_ucred) + 88);
	*SceAvCapture = 0x3800000000000015; // SceAvCapture
	
	// sceSblACMgrIsAllowedToUseSceVideoCoreServer
	uint64_t *SceVideoCoreServer = (uint64_t *)(((char *)td_ucred) + 88);
	*SceVideoCoreServer = 0x3800000000000016; // SceVideoCoreServer	

	// sceSblACMgrIsAllowedToUsemini_syscore
	uint64_t *mini_syscore = (uint64_t *)(((char *)td_ucred) + 88);
	*mini_syscore = 0x3800000000000022; // mini_syscore

	// sceSblACMgrIsAllowedToUseSceCloudClientDaemon
	uint64_t *SceCloudClientDaemon = (uint64_t *)(((char *)td_ucred) + 88);
	*SceCloudClientDaemon = 0x3800000000000028; // SceCloudClientDaemon
	
	// sceSblACMgrIsAllowedToUsefs_cleaner
	uint64_t *fs_cleaner = (uint64_t *)(((char *)td_ucred) + 88);
	*fs_cleaner = 0x380000000000001d; // fs_cleaner	

	// sceSblACMgrIsAllowedToUseSceSocialScreenMgr
	uint64_t *SceSocialScreenMgr = (uint64_t *)(((char *)td_ucred) + 88);
	*SceSocialScreenMgr = 0x3800000000000037; // SceSocialScreenMgr

	// sceSblACMgrIsAllowedToUseSceSpZeroConf
	uint64_t *SceSpZeroConf = (uint64_t *)(((char *)td_ucred) + 88);
	*SceSpZeroConf = 0x380000001000000E; // SceSpZeroConf

	// sceSblACMgrIsAllowedToUseSceMusicCoreServer
	uint64_t *SceMusicCoreServer = (uint64_t *)(((char *)td_ucred) + 88);
	*SceMusicCoreServer = 0x380000000000001a; // SceMusicCoreServer

	// sceSblACMgrIsAllowedToUsesceSblACMgrHasUseHp3dPipeCapability
	uint64_t *sceSblACMgrHasUseHp3dPipeCapability = (uint64_t *)(((char *)td_ucred) + 88);
	*sceSblACMgrHasUseHp3dPipeCapability = 0x3800000010000009; // sceSblACMgrHasUseHp3dPipeCapability

	// sceSblACMgrIsAllowedToUsesceSblACMgrHasUseHp3dPipeCapability2
	uint64_t *sceSblACMgrHasUseHp3dPipeCapability2 = (uint64_t *)(((char *)td_ucred) + 88);
	*sceSblACMgrHasUseHp3dPipeCapability2 = 0x380100000000002C; // sceSblACMgrHasUseHp3dPipeCapability2
	
	// sceSblACMgrIsAllowedToUseSceSysCore
	uint64_t *SceSysCore = (uint64_t *)(((char *)td_ucred) + 88);
	*SceSysCore = 0x3800000000000007; // SceSysCore	
				
	// sceSblACMgrHasSceProcessCapability
	uint64_t *sceProcCap = (uint64_t *)(((char *)td_ucred) + 104);
	*sceProcCap = 0xffffffffffffffff; // Sce Process

  return 0;
}

int jailbreakbis() {
  if (is_jailbroken()) {
    return 0;
  }
  struct kpayload_firmware_info kpayload_firmware_info;
  kpayload_firmware_info.fw_version = get_firmware();
  return kexec(&kpayload_jailbreakbis, &kpayload_firmware_info);
}

// Passes on the result of kpayload_patches
int install_patches() {
  struct kpayload_firmware_info kpayload_firmware_info;
  kpayload_firmware_info.fw_version = get_firmware();
  return kexec(&kpayload_patches, &kpayload_firmware_info);
}

// Passes on the result of kpayload_install_payload
int install_payload(struct configuration *config) {
  struct kpayload_payload_info kpayload_payload_info;
  kpayload_payload_info.fw_version = get_firmware();
  kpayload_payload_info.config = *config;
  kpayload_payload_info.buffer = (uint8_t *)kpayload_bin;
  kpayload_payload_info.size = (size_t)kpayload_bin_len;

  return kexec(&kpayload_install_payload, &kpayload_payload_info);
}

// Passes on the result of kpayload_exploit_fixes
int exploit_fixes() {
  struct kpayload_firmware_info kpayload_firmware_info;
  kpayload_firmware_info.fw_version = get_firmware();
  return kexec(&kpayload_exploit_fixes, &kpayload_firmware_info);
}
