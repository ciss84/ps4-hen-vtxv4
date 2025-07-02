#include "sections.h"

#include "offsets/1102.h"

// clang-format off

const struct kpayload_offsets offsets_1102 PAYLOAD_RDATA = {
  // data
  .XFAST_SYSCALL_addr              = 0x000001C0,
  .M_TEMP_addr                     = 0x015415B0,
  .MINI_SYSCORE_SELF_BINARY_addr   = 0x0155CC48,
  .ALLPROC_addr                    = 0x022D0A98,
  .SBL_DRIVER_MAPPED_PAGES_addr    = 0x02646688,
  .SBL_PFS_SX_addr                 = 0x0264C080,
  .SBL_KEYMGR_KEY_SLOTS_addr       = 0x026606E8,
  .SBL_KEYMGR_KEY_RBTREE_addr      = 0x026606F8,
  .SBL_KEYMGR_BUF_VA_addr          = 0x02664000,
  .SBL_KEYMGR_BUF_GVA_addr         = 0x02664808,
  .FPU_CTX_addr                    = 0x02660180,
  .SYSENT_addr                     = 0x01101760,

  // common
  .memcmp_addr                     = 0x000948B0,
  ._sx_xlock_addr                  = 0x000E3200,
  ._sx_xunlock_addr                = 0x000E33C0,
  .malloc_addr                     = 0x001A4240,
  .free_addr                       = 0x001A4400,
  .strstr_addr                     = 0x002C5760,
  .fpu_kern_enter_addr             = 0x000C0660,
  .fpu_kern_leave_addr             = 0x000C0720,
  .memcpy_addr                     = 0x002DDE10,
  .memset_addr                     = 0x000482D0,
  .strlen_addr                     = 0x0021DC60,
  .printf_addr                     = 0x002FCBF0,
  .eventhandler_register_addr      = 0x0043E3D0,

  // Fself
  .sceSblACMgrGetPathId_addr       = 0x003D0EB0,
  .sceSblServiceMailbox_addr       = 0x0062F7B0,
  .sceSblAuthMgrSmIsLoadable2_addr = 0x00642F30,
  ._sceSblAuthMgrGetSelfInfo_addr  = 0x00643770,
  ._sceSblAuthMgrSmStart_addr      = 0x0063E960,
  .sceSblAuthMgrVerifyHeader_addr  = 0x00642F90,

  // Fpkg
  .RsaesPkcs1v15Dec2048CRT_addr    = 0x003C8080,
  .Sha256Hmac_addr                 = 0x002D1CC0,
  .AesCbcCfb128Encrypt_addr        = 0x002DEAC0,
  .AesCbcCfb128Decrypt_addr        = 0x002DECF0,
  .sceSblDriverSendMsg_0_addr      = 0x0061AF00,
  .sceSblPfsSetKeys_addr           = 0x0061D8A0,
  .sceSblKeymgrSetKeyStorage_addr  = 0x0062ED60,
  .sceSblKeymgrSetKeyForPfs_addr   = 0x00625D90,
  .sceSblKeymgrCleartKey_addr      = 0x006260D0,
  .sceSblKeymgrSmCallfunc_addr     = 0x00625960,

  // Patch
  .vmspace_acquire_ref_addr        = 0x00357740,
  .vmspace_free_addr               = 0x00357570,
  .vm_map_lock_read_addr           = 0x003578D0,
  .vm_map_unlock_read_addr         = 0x00357920,
  .vm_map_lookup_entry_addr        = 0x00357F10,
  .proc_rwmem_addr                 = 0x003838C0,

  // Fself hooks
  .sceSblAuthMgrIsLoadable__sceSblACMgrGetPathId_hook        = 0x0063D0DC,
  .sceSblAuthMgrIsLoadable2_hook                             = 0x0063D22E,
  .sceSblAuthMgrVerifyHeader_hook1                           = 0x0063D9C6,
  .sceSblAuthMgrVerifyHeader_hook2                           = 0x0063E6A9,
  .sceSblAuthMgrSmLoadSelfSegment__sceSblServiceMailbox_hook = 0x00640DAD,
  .sceSblAuthMgrSmLoadSelfBlock__sceSblServiceMailbox_hook   = 0x006419E8,

  // Fpkg hooks
  .sceSblKeymgrSetKeyStorage__sceSblDriverSendMsg_hook       = 0x0062EE05,
  .sceSblKeymgrInvalidateKey__sx_xlock_hook                  = 0x00626F4D,
  .sceSblKeymgrSmCallfunc_npdrm_decrypt_isolated_rif_hook    = 0x0064D1D0,
  .sceSblKeymgrSmCallfunc_npdrm_decrypt_rif_new_hook         = 0x0064DF9E,
  .mountpfs__sceSblPfsSetKeys_hook1                          = 0x00699359,
  .mountpfs__sceSblPfsSetKeys_hook2                          = 0x0069958A,

  // SceShellUI patches - debug patches - libkernel_sys.sprx
  .sceSblRcMgrIsAllowDebugMenuForSettings_patch              = 0x0001D100,
  .sceSblRcMgrIsStoreMode_patch                              = 0x0001D460,

  // SceShellUI patches - remote play patches
  .CreateUserForIDU_patch                                    = 0x0018B110, // system_ex\app\NPXS20001\eboot.bin
  .remote_play_menu_patch                                    = 0x00ECAB92, // system_ex\app\NPXS20001\psm\Application\app.exe.sprx

  // SceRemotePlay patches - remote play patches - system\vsh\app\NPXS21006
  .SceRemotePlay_patch1                                      = 0x000ECCB5,
  .SceRemotePlay_patch2                                      = 0x000ECCD0,

  // SceShellCore patches - call sceKernelIsGenuineCEX
  .sceKernelIsGenuineCEX_patch1    = 0x0016B664,
  .sceKernelIsGenuineCEX_patch2    = 0x0086BD24,
  .sceKernelIsGenuineCEX_patch3    = 0x008BC022,
  .sceKernelIsGenuineCEX_patch4    = 0x00A1D6C4,

  // SceShellCore patches - call nidf_libSceDipsw
  .nidf_libSceDipsw_patch1         = 0x0016B692,
  .nidf_libSceDipsw_patch2         = 0x00249E0C,
  .nidf_libSceDipsw_patch3         = 0x0086BD52,
  .nidf_libSceDipsw_patch4         = 0x00A1D6F2,

  // SceShellCore patches - bypass firmware checks
  .check_disc_root_param_patch     = 0xDEADC0DE,
  .app_installer_patch             = 0x00134A50,
  .check_system_version            = 0x003C41A7,
  .check_title_system_update_patch = 0x003C73F0,

  // SceShellCore patches - enable remote pkg installer
  .enable_data_mount_patch         = 0x0031F070,

  // SceShellCore patches - enable VR without spoof
  .enable_psvr_patch               = 0x00DA7D00,

  // SceShellCore patches - enable fpkg
  .enable_fpkg_patch               = 0x003D7C9F,

  // SceShellCore patches - use `free` prefix instead `fake`
  .fake_free_patch                 = 0x00FC8439,

  // SceShellCore patches - enable official external HDD support
  .pkg_installer_patch             = 0x00A06C11,
  .ext_hdd_patch                   = 0x0060E17D,

  // SceShellCore patches - enable debug trophies
  .debug_trophies_patch            = 0x007456E9,

  // SceShellCore patches - disable screenshot block
  .disable_screenshot_patch        = 0x000CF876,

  // Process structure offsets
  .proc_p_comm_offset = 0x454,
  .proc_path_offset   = 0x474,
};

// clang-format on
