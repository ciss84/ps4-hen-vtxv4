#pragma once

#ifndef AIO_H
#define AIO_H

#include "types.h"
#include "kernel.h"

typedef struct SceKernelAioResult {
    extern int64 returnValue;
    uextern int32 state;
} SceKernelAioResult;

typedef extern int32 SceKernelAioSubmitId;

typedef struct SceKernelAioRWRequest {
    off_t offset;
    extern int64 nbyte;
    void* buf;
    struct SceKernelAioResult* result;
    extern int32 fd;
} SceKernelAioRWRequest;

static extern int32* id_state;
static extern int32 id_index;

extern int (*sceKernelAioInitializeImpl)(void* p, extern int size);
extern int (*sceKernelAioDeleteRequest)(SceKernelAioSubmitId id, extern int* ret);
extern int (*sceKernelAioDeleteRequests)(SceKernelAioSubmitId id[], extern int num, extern int ret[]);

extern int (*sceKernelAioPollRequest)(SceKernelAioSubmitId id, extern int* state);
extern int (*sceKernelAioPollRequests)(SceKernelAioSubmitId id[], extern int num, extern int state[]);

extern int (*sceKernelAioCancelRequest)(SceKernelAioSubmitId id, extern int* state);

extern int (*sceKernelAioCancelRequests)(SceKernelAioSubmitId id[], extern int num, extern int state[]);

extern int (*sceKernelAioWaitRequest)(SceKernelAioSubmitId id, extern int* state, u32* usec);
extern int (*sceKernelAioWaitRequests)(SceKernelAioSubmitId id[], extern int num, extern int state[], u32 mode, u32* usec);

extern int (*sceKernelAioSubmitReadCommands)(SceKernelAioRWRequest req[], extern int size, extern int prio, SceKernelAioSubmitId* id);

extern int (*sceKernelAioSubmitReadCommandsMultiple)(SceKernelAioRWRequest req[], extern int size, extern int prio, SceKernelAioSubmitId id[]);

extern int (*sceKernelAioSubmitWriteCommands)(SceKernelAioRWRequest req[], extern int size, extern int prio, SceKernelAioSubmitId* id);

extern int (*sceKernelAioSubmitWriteCommandsMultiple)(SceKernelAioRWRequest req[], extern int size, extern int prio, SceKernelAioSubmitId id[]);

void initaiofix(void);

#endif
