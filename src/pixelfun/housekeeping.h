#ifndef HOUSEKEEPING_H
#define HOUSEKEEPING_H

void HKError(char* msg);

int HKGetNewPointer();

void HKPtr(void* ptr);

void HKFree();

#endif

