#pragma once
#include "..\..\Utils\Memory\Memory.h"
#include "..\..\Utils\SimpleMath\SimpleMath.h"
#include "Struct.h"
using namespace DirectX::SimpleMath;

// Credit
// https://www.unknowncheats.me/forum/escape-from-tarkov/579954-dma-based-esp.html
// https://www.unknowncheats.me/forum/escape-from-tarkov/482418-2d-map-dma-radar-wip.html

// Old (maybe)
namespace Offsets
{
    constexpr auto GameObjectManager = 0x17FFD28;

    namespace LocalGameWorld 
    {
        constexpr auto RegisteredPlayers = 0xC0;
    }
    namespace Player
    {
        constexpr auto Profile = 0x520;
        constexpr auto HealthController = 0x560;
        constexpr auto IsLocalPlayer = 0x842; 
    }
    namespace Profile // EFT.Profile
    {
        constexpr auto Id = 0x10;
        constexpr auto AccountId = 0x18; 
        constexpr auto PlayerInfo = 0x28; 
        constexpr auto Stats = 0xF0; 
    }
    namespace PlayerInfo // -.GClass1307
    {
        constexpr auto Nickname = 0x19;
        constexpr auto MainProfileNickname = 0x19;
        constexpr auto GroupId = 0x20; 
        constexpr auto Settings = 0x50; 
        constexpr auto PlayerSide = 0xE8;
    }
}

class Tarkov
{
private:
    bool GetCameras();
    Matrix MatrixTranspose(const Matrix pM);
    uint64_t GetObjectFromList(uint64_t listPtr, uint64_t lastObjectPtr, const char* objectName);
public:
    // Status
    struct status
    {
        bool InRaid = false;
        bool ReadSuccess = false;
        uint64_t NewLocalGameWorld;
    }s;

    // EFT Data
    GameObjectManager GOM;
    uint64_t GameWorld = 0;
    uint64_t LocalGameWorld = 0;

    uint64_t FpsCamera = 0;
    Matrix MatrixFPS = {};

    void UpdateSomeData();
    void UpdateGlobalData();
    Vector2 WorldToScreen(const Vector3 position);
};