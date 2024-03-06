#pragma once
#include <iostream>

struct BaseObject
{
    uint64_t previousObjectLink;
    uint64_t nextObjectLink;
    uint64_t object;
};
struct GameObjectManager
{
    uint64_t lastTaggedObject;
    uint64_t taggedObjects;
    uint64_t lastActiveObject;
    uint64_t activeObjects;
    uint64_t LastActiveNode;
    uint64_t ActiveNodes;
};

class ListInternal
{
public:
    char pad_0x0000[0x20];
    uintptr_t* firstEntry;
};

class List
{
public:
    char pad_0x0000[0x10];
    ListInternal* listBase;
    __int32 itemCount;
};