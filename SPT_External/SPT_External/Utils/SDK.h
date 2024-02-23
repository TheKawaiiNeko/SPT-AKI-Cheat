#pragma once
#include "Memory\Memory.h"
#include <string>
#include "../Utils/SimpleMath/SimpleMath.h"
using namespace DirectX::SimpleMath;

namespace Offsets
{
    namespace UnityList
    {
        constexpr auto Base = 0x10; // to UnityListBase
        constexpr auto Count = 0x18; // int32
    }
    namespace UnityListBase
    {
        constexpr auto Start = 0x20; // start of list +(i * 0x8)
    }
    namespace UnityString
    {
        constexpr auto Length = 0x10; // int32
        constexpr auto Value = 0x14; // string,unicode
    }
    namespace ModuleBase
    {
        constexpr auto GameObjectManager = 0x17FFD28; // to eft_dma_radar.GameObjectManager
    }
    namespace GameObject
    {
        //static readonly uint[] To_TransformInternal = new uint[]{ 0x10, 0x30, 0x30, 0x8, 0x28, 0x10 }; // to TransformInternal
        constexpr auto ObjectClass = 0x30;
        constexpr auto ObjectName = 0x60; // string,default (null terminated)
    }
    namespace GameWorld
    {
        //static readonly uint[] To_LocalGameWorld = new uint[]{ GameObject.ObjectClass, 0x18, 0x28 };
    }
    namespace LocalGameWorld // -.ClientLocalGameWorld : ClientGameWorld
    {
        constexpr auto ExfilController = 0x18; // to ExfilController
        constexpr auto LootList = 0x98; // to UnityList
        constexpr auto RegisteredPlayers = 0xC0; // to RegisteredPlayers
        constexpr auto Grenades = 0x170; // to Grenades
    }
    namespace ExfilController // -.GClass0B67
    {
        constexpr auto ExfilCount = 0x18; // int32
        constexpr auto ExfilList = 0x20; // to UnityListBase
    }
    namespace Exfil
    {
        constexpr auto Status = 0xA8; // int32
    }
    namespace Grenades // -.GClass05FD<Int32, Throwable>
    {
        constexpr auto List = 0x18; // to UnityList
    }
    namespace Player // EFT.Player : MonoBehaviour, GInterface4AD8, GInterface5221, GInterface521C, GInterface5226, GInterface525D, GInterfaceA7EE, IDissonancePlayer
    {
        //static readonly uint[] To_TransformInternal = new uint[]{ 0xA8, 0x28, 0x28, Offsets.UnityList.Base, Offsets.UnityListBase.Start + (0 * 0x8), 0x10 }; // to TransformInternal
        constexpr auto MovementContext = 0x40; // to MovementContext
        constexpr auto Corpse = 0x724; // EFT.Interactive.Corpse
        constexpr auto Profile = 0x520; // to Profile
        constexpr auto HealthController = 0x560; // to HealthController
        constexpr auto InventoryController = 0x118; // to InventoryController
        constexpr auto IsLocalPlayer = 0x842; // bool
    }
    namespace Profile // EFT.Profile
    {
        constexpr auto Id = 0x10; // unity string
        constexpr auto AccountId = 0x18; // unity string
        constexpr auto PlayerInfo = 0x28; // to PlayerInfo
        constexpr auto Stats = 0xF0; // to Stats
    }
    namespace Stats // -.GClass0654
    {
        constexpr auto OverallCounters = 0x18; // to OverallCounters
    }
    namespace OverallCounters
    {
        constexpr auto Counters = 0x10; // to Dictionary<IntPtr, ulong>
    }
    namespace PlayerInfo // -.GClass1307
    {
        constexpr auto Nickname = 0x19; // unity string
        constexpr auto MainProfileNickname = 0x19; // unity string
        constexpr auto GroupId = 0x20; // ptr to UnityString (0/null if solo or bot)
        constexpr auto Settings = 0x50; // to PlayerSettings
        constexpr auto PlayerSide = 0xE8; // int32
        constexpr auto RegDate = 0x74; // int32
        constexpr auto MemberCategory = 0x8C; // int32 enum
        constexpr auto Experience = 0x90; // int32
    }
    namespace PlayerSettings
    {
        constexpr auto Role = 0x10; // int32 enum
    }
    namespace MovementContext
    {
        constexpr auto Rotation = 0x24C; // vector2
    }
    namespace InventoryController // -.GClass1A98
    {
        constexpr auto Inventory = 0x570; // to Inventory
    }
    namespace Inventory
    {
        constexpr auto Equipment = 0x10; // to Equipment
    }
    namespace Equipment
    {
        constexpr auto Slots = 0x78; // to UnityList
    }
    namespace Slot
    {
        constexpr auto Name = 0x10; // string,unity
        constexpr auto ContainedItem = 0x38; // to LootItemBase
    }
    namespace HealthController // -.GInterface7AEE
    {
        //static readonly uint[] To_HealthEntries = { 0x68, 0x18 }; // to HealthEntries
    }
    namespace HealthEntries
    {
        constexpr auto HealthEntries_Start = 0x30; // Each body part 0x18 , to HealthEntry
    }
    namespace HealthEntry
    {
        constexpr auto Value = 0x10; // to HealthValue
    }
    namespace HealthValue
    {
        constexpr auto Current = 0x0; // float
        constexpr auto Maximum = 0x4; // float
        constexpr auto Minimum = 0x8; // float
    }
    namespace LootListItem
    {
        constexpr auto LootUnknownPtr = 0x10; // to LootUnknownPtr
    }

    namespace LootUnknownPtr
    {
        constexpr auto LootInteractiveClass = 0x28; // to LootInteractiveClass
    }
    namespace LootInteractiveClass
    {
        constexpr auto LootBaseObject = 0x10; // to LootBaseObject
        constexpr auto LootItemBase = 0xB0; // to LootItemBase
        constexpr auto ContainerItemOwner = 0x110; // to ContainerItemOwner
    }
    namespace LootItemBase //EFT.InventoryLogic.Item
    {
        constexpr auto ItemTemplate = 0x40; // to ItemTemplate
        constexpr auto Grids = 0x70; // to Grids
        constexpr auto Slots = 0x78; // to UnityList
        constexpr auto Cartridges = 0x90; // via -.GClass19FD : GClass19D6, IAmmoContainer , to StackSlot
    }
    namespace StackSlot // EFT.InventoryLogic.StackSlot : Object, IContainer
    {
        constexpr auto Items = 0x10; // to UnityList , of LootItemBase
    }
    namespace ItemTemplate //EFT.InventoryLogic.ItemTemplate
    {
        constexpr auto BsgId = 0x50; // string,unity
        constexpr auto IsQuestItem = 0x9C; // bool
    }
    namespace LootBaseObject
    {
        constexpr auto GameObject = 0x30; // to GameObject
    }
    namespace LootGameObjectClass
    {
        //static readonly uint[] To_TransformInternal = new uint[]{ 0x8, 0x28, 0x10 };
    }
    namespace ContainerItemOwner
    {
        constexpr auto LootItemBase = 0xC0; // to LootItemBase
    }
    namespace Grids
    {
        constexpr auto GridsEnumerableClass = 0x40;
    }
    namespace TransformInternal
    {
        constexpr auto Hierarchy = 0x38; // to TransformHierarchy
        constexpr auto HierarchyIndex = 0x40; // int32
    }
    namespace TransformHierarchy
    {
        constexpr auto Vertices = 0x18; // List<Vector128<float>>
        constexpr auto Indices = 0x20; // List<int>
    }
}

namespace structs 
{
    enum Bones : int
    {
        HumanBase = 0,
        HumanPelvis = 14,
        HumanLThigh1 = 15,
        HumanLThigh2 = 16,
        HumanLCalf = 17,
        HumanLFoot = 18,
        HumanLToe = 19,
        HumanRThigh1 = 20,
        HumanRThigh2 = 21,
        HumanRCalf = 22,
        HumanRFoot = 23,
        HumanRToe = 24,
        HumanSpine1 = 29,
        HumanSpine2 = 36,
        HumanSpine3 = 37,
        HumanLCollarbone = 89,
        HumanLUpperarm = 90,
        HumanLForearm1 = 91,
        HumanLForearm2 = 92,
        HumanLForearm3 = 93,
        HumanLPalm = 94,
        HumanRCollarbone = 110,
        HumanRUpperarm = 111,
        HumanRForearm1 = 112,
        HumanRForearm2 = 113,
        HumanRForearm3 = 114,
        HumanRPalm = 115,
        HumanNeck = 132,
        HumanHead = 133
    };

    struct BaseObject
    {
        uint64_t previousObjectLink; //0x0000
        uint64_t nextObjectLink; //0x0008
        uint64_t object; //0x0010
    };
    struct GameObjectManager
    {
        uint64_t lastTaggedObject; //0x0000
        uint64_t taggedObjects; //0x0008
        uint64_t lastActiveObject; //0x0010
        uint64_t activeObjects; //0x0018
        uint64_t LastActiveNode; // 0x20
        uint64_t ActiveNodes; // 0x28
    }; //Size: 0x0010

    class ListInternal
    {
    public:
        char pad_0x0000[0x20]; //0x0000
        uintptr_t* firstEntry; //0x0020 
    }; //Size=0x0028

    class List
    {
    public:
        char pad_0x0000[0x10]; //0x0000
        ListInternal* listBase; //0x0010 
        __int32 itemCount; //0x0018 
    }; //Size=0x001C
}

class SDK
{
private:
    uint64_t UnityPlayer = 0x0, FpsCamera = 0x0, OpticCamera = 0x0, GameWorld = 0x0;
    Matrix matrixfps, matrixoptic;

    struct OpticThing
    {
        float fov, aspect_ratio;
    }idk;

    Matrix MatrixTranspose(const Matrix pM)
    {
        Matrix pOut = {};
        pOut._11 = pM._11;
        pOut._12 = pM._21;
        pOut._13 = pM._31;
        pOut._14 = pM._41;

        pOut._21 = pM._12;
        pOut._22 = pM._22;
        pOut._23 = pM._32;
        pOut._24 = pM._42;

        pOut._31 = pM._13;
        pOut._32 = pM._23;
        pOut._33 = pM._33;
        pOut._34 = pM._43;

        pOut._41 = pM._14;
        pOut._42 = pM._24;
        pOut._43 = pM._34;
        pOut._44 = pM._44;

        return pOut;
    }
public:
    bool IsRaid = false;
    structs::GameObjectManager GOM;
    uint64_t LocalGameWorld;

    // Sys
    bool get_GameObjectManager();
    bool get_LocalGameWorld();
    bool GetCameras();
    uint64_t GetObjectFromList(uint64_t listPtr, uint64_t lastObjectPtr, const char* objectName);

    void UpdateCamera()
    {
        uint64_t base = m.Read<uint64_t>(FpsCamera + 0x30);
        base = m.Read<uint64_t>(base + 0x18);
        matrixfps = m.Read<Matrix>(base + 0xDC);
        /*
        idk.fov = m.Read<float>(base + 0x15C);
        idk.aspect_ratio = m.Read<float>(base + 0x4C8);
        base = m.Read<uint64_t>(OpticCamera + 0x30);
        base = m.Read<uint64_t>(base + 0x18);
        matrixoptic = m.Read<Matrix>(base + 0xDC);
        */
    }

    Vector2 WorldToScreen(const Vector3 point3D)
    {
        Matrix m = MatrixTranspose(matrixfps);
        Vector3 translationVector = Vector3(m._41, m._42, m._43);
        Vector3 up = Vector3(m._21, m._22, m._23);
        Vector3 right = Vector3(m._11, m._12, m._13);

        float w = translationVector.Dot(point3D) + m._44;

        if (w < 0.098f)
            return Vector2(0.f, 0.f);

        float y = up.Dot(point3D) + m._24;
        float x = right.Dot(point3D) + m._14;

        return Vector2((1920 / 2) * (1.f + x / w), (1080 / 2) * (1.f - y / w));
    }
};

bool SDK::get_GameObjectManager()
{
    uint64_t addr = m.Read<uint64_t>(m.BaseAddress + Offsets::ModuleBase::GameObjectManager);
    GOM = m.Read<structs::GameObjectManager>(addr);

    return true;
}

bool SDK::get_LocalGameWorld()
{
    get_GameObjectManager();

    uint64_t activeNodes = m.Read<uint64_t>(GOM.ActiveNodes);
    uint64_t lastActiveNode = m.Read<uint64_t>(GOM.LastActiveNode);
    uint64_t gameWorld = GetObjectFromList(activeNodes, lastActiveNode, "GameWorld"); // OK

    uint64_t L1 = m.Read<uint64_t>(gameWorld + 0x30);
    uint64_t L2 = m.Read<uint64_t>(L1 + 0x18);
    LocalGameWorld = m.Read<uint64_t>(L2 + 0x28);

    return true;
}

bool SDK::GetCameras()
{
    struct cameras
    {
        uint64_t list;
        uint64_t min;
        uint64_t count;
        uint64_t max;
    };

    FpsCamera = 0x0;
    OpticCamera = 0x0;

    uint64_t tmp = m.Read<uint64_t>(m.BaseAddress + 0x179F500);
    cameras all_cameras = m.Read<cameras>(tmp + 0x0);

    if (all_cameras.count == 0)
        return false;

    for (int i = 0; i < all_cameras.count; i++)
    {
        auto camera = m.Read<uint64_t>(all_cameras.list + ((uint64_t)i * 0x8));
        if (camera != NULL)
        {
            auto camera_obj = m.Read<uint64_t>(camera + 0x30);
            auto camera_name_ptr = m.Read<uint64_t>(camera_obj + 0x60);

            if (!camera_name_ptr)
                return false;

            char name[32];
            ReadProcessMemory(m.pHandle, (void*)camera_name_ptr, name, sizeof(name), nullptr);

            if (strcmp(name, "BaseOpticCamera(Clone)") == 0)
                OpticCamera = camera_obj;
            if (strcmp(name, "FPS Camera" )== 0)
                FpsCamera = camera_obj;
            if (FpsCamera != 0 && OpticCamera != 0)
                return true;
        }
    }
    return false;
}

uint64_t SDK::GetObjectFromList(uint64_t listPtr, uint64_t lastObjectPtr, const char* objectName)
{
    char name[256];
    uint64_t classNamePtr = 0x0;

    structs::BaseObject activeObject = m.Read<structs::BaseObject>(listPtr);
    structs::BaseObject lastObject = m.Read<structs::BaseObject>(lastObjectPtr);

    if (activeObject.object != 0x0)
    {
        while (activeObject.object != 0 && activeObject.object != lastObject.object)
        {
            classNamePtr = m.Read<uint64_t>(activeObject.object + 0x60);
            ReadProcessMemory(m.pHandle, (void*)(classNamePtr), &name, sizeof(name), NULL);

            if (strcmp(name, objectName) == 0)
                return activeObject.object;

            activeObject = m.Read<structs::BaseObject>(activeObject.nextObjectLink);
        }
    }
    if (lastObject.object != 0x0)
    {
        classNamePtr = m.Read<uint64_t>(lastObject.object + 0x60);

        ReadProcessMemory(m.pHandle, (void*)(classNamePtr), &name, sizeof(256), NULL);

        if (strcmp(name, objectName) == 0)
            return lastObject.object;
    }

    return 0;
}