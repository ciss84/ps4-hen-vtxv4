#pragma once

#ifndef AIO_H
#define AIO_H

#include "types.h"
#include "kernel.h"

typedef struct SceKernelAioResult {
    int64 returnValue;
    int32 state;
} SceKernelAioResult;

typedef int32 SceKernelAioSubmitId;

typedef struct SceKernelAioRWRequest {
    off_t offset;
    int64 nbyte;
    void* buf;
    struct SceKernelAioResult* result;
    int32 fd;
} SceKernelAioRWRequest;

static int32* id_state;
static int32 id_index;

extern int (*sceKernelAioInitializeImpl)(void* p, int size);
extern int (*sceKernelAioDeleteRequest)(SceKernelAioSubmitId id, int* ret);
extern int (*sceKernelAioDeleteRequests)(SceKernelAioSubmitId id[], int num, int ret[]);

extern int (*sceKernelAioPollRequest)(SceKernelAioSubmitId id, int* state);
extern int (*sceKernelAioPollRequests)(SceKernelAioSubmitId id[], int num, int state[]);

extern int (*sceKernelAioCancelRequest)(SceKernelAioSubmitId id, int* state);

extern int (*sceKernelAioCancelRequests)(SceKernelAioSubmitId id[], int num, int state[]);

extern int (*sceKernelAioWaitRequest)(SceKernelAioSubmitId id, int* state, uint32* usec);
extern int (*sceKernelAioWaitRequests)(SceKernelAioSubmitId id[], int num, int state[], uint32 mode, uint32* usec);

extern int (*sceKernelAioSubmitReadCommands)(SceKernelAioRWRequest req[], int size, int prio, SceKernelAioSubmitId* id);

extern int (*sceKernelAioSubmitReadCommandsMultiple)(SceKernelAioRWRequest req[], int size, int prio, SceKernelAioSubmitId id[]);

extern int (*sceKernelAioSubmitWriteCommands)(SceKernelAioRWRequest req[], int size, int prio, SceKernelAioSubmitId* id);

extern int (*sceKernelAioSubmitWriteCommandsMultiple)(SceKernelAioRWRequest req[], int size, int prio, SceKernelAioSubmitId id[]);

void initaiofix(void);

#endif
