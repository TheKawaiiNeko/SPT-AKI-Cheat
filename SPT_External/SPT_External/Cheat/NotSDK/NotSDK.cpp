#include "NotSDK.h"
#include "..\..\Utils\Globals\Globals.h"

Globals g;

// レイドのスタート時にでも読めばいい
void Tarkov::UpdateGlobalData()
{
    // GameObjectManager
    uint64_t GOM_Address = m.Read<uint64_t>(m.BaseAddress + Offsets::GameObjectManager);
    GOM = m.Read<GameObjectManager>(GOM_Address);

    while (g.Run)
    {   
        // GameWorld
        uint64_t activeNodes = m.Read<uint64_t>(GOM.ActiveNodes);
        uint64_t lastActiveNode = m.Read<uint64_t>(GOM.LastActiveNode);
        GameWorld = GetObjectFromList(activeNodes, lastActiveNode, "GameWorld");

        if (GameWorld == 0)
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            s.ReadSuccess = false;
            continue;
        }
            
        // LocalGameWorld
        uint64_t LGWt1 = m.Read<uint64_t>(GameWorld + 0x30);
        uint64_t LGWt2 = m.Read<uint64_t>(LGWt1 + 0x18);
        s.NewLocalGameWorld = m.Read<uint64_t>(LGWt2 + 0x28);

        if (s.NewLocalGameWorld != 0 && LocalGameWorld != s.NewLocalGameWorld)
            LocalGameWorld = s.NewLocalGameWorld;
        else if (s.NewLocalGameWorld == 0)
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            s.ReadSuccess = false;
            continue;
        }

        // Camera
        GetCameras();
        s.ReadSuccess = true;

        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

void Tarkov::UpdateSomeData()
{
    while (g.Run)
    {
        uint64_t base = m.Read<uint64_t>(FpsCamera + 0x30);
        base = m.Read<uint64_t>(base + 0x18);
        MatrixFPS = m.Read<Matrix>(base + 0xDC);

        std::this_thread::sleep_for(std::chrono::microseconds(50));
    }
}

bool Tarkov::GetCameras()
{
    FpsCamera = 0;

    uint64_t tmp = m.Read<uint64_t>(m.BaseAddress + 0x179F500);
    cameras all_cameras = m.Read<cameras>(tmp);

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

            char name[64];
            for (int j = 0; j < sizeof(name); j++)
            {
                name[j] = m.Read<char>(camera_name_ptr + j);
                if (name[j] == NULL)
                    break;
            }

            if (strcmp(name, "FPS Camera") == 0)
                FpsCamera = camera_obj;
            if (FpsCamera != 0)
                return true;
        }
    }
    return false;
}

uint64_t Tarkov::GetObjectFromList(uint64_t listPtr, uint64_t lastObjectPtr, const char* objectName)
{
    char name[128];
    uint64_t classNamePtr = 0x0;

    BaseObject activeObject = m.Read<BaseObject>(listPtr);
    BaseObject lastObject = m.Read<BaseObject>(lastObjectPtr);

    if (activeObject.object != 0x0)
    {
        while (activeObject.object != 0 && activeObject.object != lastObject.object)
        {
            classNamePtr = m.Read<uint64_t>(activeObject.object + 0x60);
            ReadProcessMemory(m.pHandle, (void*)(classNamePtr), &name, sizeof(name), NULL);

            if (strcmp(name, objectName) == 0)
                return activeObject.object;

            activeObject = m.Read<BaseObject>(activeObject.nextObjectLink);
        }
    }
    if (lastObject.object != 0x0)
    {
        classNamePtr = m.Read<uint64_t>(lastObject.object + 0x60);

        ReadProcessMemory(m.pHandle, (void*)(classNamePtr), &name, sizeof(name), NULL);

        if (strcmp(name, objectName) == 0)
            return lastObject.object;
    }

    ZeroMemory(name, sizeof(name));

    return 0;
}

Matrix Tarkov::MatrixTranspose(const Matrix pM)
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

Vector2 Tarkov::WorldToScreen(const Vector3 position)
{
    Matrix m = MatrixTranspose(MatrixFPS);
    Vector3 translationVector = Vector3(m._41, m._42, m._43);
    Vector3 up = Vector3(m._21, m._22, m._23);
    Vector3 right = Vector3(m._11, m._12, m._13);

    float w = translationVector.Dot(position) + m._44;

    if (w < 0.098f)
        return Vector2(0.f, 0.f);

    float y = up.Dot(position) + m._24;
    float x = right.Dot(position) + m._14;

    return Vector2((g.GameSize.right / 2) * (1.f + x / w), (g.GameSize.bottom / 2) * (1.f - y / w));
}