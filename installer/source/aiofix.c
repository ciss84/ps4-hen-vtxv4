#include "module.h"
#include "syscall.h"

#include "aiofix.h"

int libKernelHandle;

int (*sceKernelAioInitializeImpl)(void* p, int size);
int (*sceKernelAioDeleteRequest)(SceKernelAioSubmitId id, int* ret);
int (*sceKernelAioDeleteRequests)(SceKernelAioSubmitId id[], int num, int ret[]);

int (*sceKernelAioPollRequest)(SceKernelAioSubmitId id, int* state);
int (*sceKernelAioPollRequests)(SceKernelAioSubmitId id[], int num, int state[]);

int (*sceKernelAioCancelRequest)(SceKernelAioSubmitId id, int* state);

int (*sceKernelAioCancelRequests)(SceKernelAioSubmitId id[], int num, int state[]);

int (*sceKernelAioWaitRequest)(SceKernelAioSubmitId id, int* state, u32* usec);
int (*sceKernelAioWaitRequests)(SceKernelAioSubmitId id[], int num, int state[], u32 mode, u32* usec);

int (*sceKernelAioSubmitReadCommands)(SceKernelAioRWRequest req[], int size, int prio, SceKernelAioSubmitId* id);

int (*sceKernelAioSubmitReadCommandsMultiple)(SceKernelAioRWRequest req[], int size, int prio, SceKernelAioSubmitId id[]);

int (*sceKernelAioSubmitWriteCommands)(SceKernelAioRWRequest req[], int size, int prio, SceKernelAioSubmitId* id);

int (*sceKernelAioSubmitWriteCommandsMultiple)(SceKernelAioRWRequest req[], int size, int prio, SceKernelAioSubmitId id[]);

void initaiofix(void) {
  if (libKernelHandle) {
    return;
  }
  __error = NULL;

  if (loadModule("libkernel.sprx", &libKernelHandle)) {
    if (loadModule("libkernel_web.sprx", &libKernelHandle)) {
      loadModule("libkernel_sys.sprx", &libKernelHandle);
    }
  }

	RESOLVE(libKernelHandle, sceKernelAioInitializeImpl);
	RESOLVE(libKernelHandle, sceKernelAioDeleteRequest);
	RESOLVE(libKernelHandle, sceKernelAioDeleteRequests);
	RESOLVE(libKernelHandle, sceKernelAioPollRequest);
	RESOLVE(libKernelHandle, sceKernelAioPollRequests);
  RESOLVE(libKernelHandle, sceKernelAioCancelRequest);
	RESOLVE(libKernelHandle, sceKernelAioCancelRequests);	
	RESOLVE(libKernelHandle, sceKernelAioWaitRequest);
	RESOLVE(libKernelHandle, sceKernelAioWaitRequests); 	
	RESOLVE(libKernelHandle, sceKernelAioSubmitReadCommands);
	RESOLVE(libKernelHandle, sceKernelAioSubmitReadCommandsMultiple);
	RESOLVE(libKernelHandle, sceKernelAioSubmitWriteCommands);
	RESOLVE(libKernelHandle, sceKernelAioSubmitWriteCommandsMultiple); 
}