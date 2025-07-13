#pragma once

#ifndef KERNEL_H
#define KERNEL_H

#include "types.h"

typedef struct SceKernelAioResult {
    int64 returnValue;
    uint32 state;
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

void initaiofix(void);

#endif
