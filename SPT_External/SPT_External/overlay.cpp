#include "overlay.h"
#include "Utils\SDK.h"
#include <string>

SDK sdk, *EFT = &sdk;
bool Crosshair = true;

void Overlay::m_Info()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)GameRect.right, (float)GameRect.bottom));
    ImGui::Begin("##Info", (bool*)NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs);

    ImGui::Text("Tarkov External [%.1f FPS]", ImGui::GetIO().Framerate);

    if (Crosshair)
        ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)GameRect.right / 2.f, (float)GameRect.bottom / 2.f), 2, WHITE, NULL);

    ImGui::End();
}

void Overlay::m_Menu()
{
    ImGui::SetNextWindowBgAlpha(0.95f);
    ImGui::SetNextWindowSize(ImVec2(500.f, 450.f));
    ImGui::Begin("SPT-AKI [ EXTERNAL ]", (bool*)NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Checkbox("ESP", &ESP);
    ImGui::Checkbox("Line", &ESP_Line);
    ImGui::Checkbox("Distance", &ESP_Distance);

    ImGui::End();
}

float GetDistance(Vector3 value1, Vector3 value2)
{
    float num = value1.x - value2.x;
    float num2 = value1.y - value2.y;
    float num3 = value1.z - value2.z;

    return sqrt(num * num + num2 * num2 + num3 * num3);
}

Vector3 LoadlPosition = Vector3(0.f, 0.f, 0.f);

// Player Only
void Overlay::m_ESP()
{
    // ImGui Window
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)GameRect.right, (float)GameRect.bottom));
    ImGui::Begin("##ESP", &ShowMenu, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs);

    EFT->get_LocalGameWorld();

    uint64_t OnlineUser = m.Read<uint64_t>(EFT->LocalGameWorld + Offsets::LocalGameWorld::RegisteredPlayers);

    if (!OnlineUser)
        return;

    uint64_t PlayerListBase = m.Read<uint64_t>(OnlineUser + offsetof(structs::List, listBase));
    structs::List PlayerList = m.Read<structs::List>(PlayerListBase);

    if (PlayerList.itemCount <= 0 || !PlayerListBase)
        return;

    EFT->GetCameras();
    EFT->UpdateCamera();

    for (int i = 0; i < PlayerList.itemCount; i++)
    {
        uint64_t player = m.Read<uint64_t>(PlayerListBase + (offsetof(structs::ListInternal, firstEntry) + (i * 8)));

        if (!player)
            return;

        uint64_t profile = m.Read<uint64_t>(player + 0x530); // EFT.Profile
        uint64_t PlayerInfo = m.Read<uint64_t>(profile + 0x28);

        // OK
        int pSide = m.Read<int>(PlayerInfo + 0x70);

        /*
        pSide :
            1 = Usec
            2 = Bear
            4 = Scav
        */

        // Health
        uint64_t m_pHealthController = m.Read<uint64_t>(player + 0x570);
        uint64_t m_pHealthBody = m.Read<uint64_t>(m_pHealthController + 0x68);
        uint64_t m_pBodyController = m.Read<uint64_t>(m_pHealthBody + 0x18);

        // êgëÃÇÃäeïîà Ç≤Ç∆ - Raw Data
        float Health = 0.f;
        for (int q = 0; q <= 6; q++)
        {
            auto body_part = m.Read<uintptr_t>(m_pBodyController + 0x30 + (q * 0x18));
            auto health_container = m.Read<uintptr_t>(body_part + 0x10);
            Health = m.Read<float>(health_container + 0x10);

            //auto health_max = m.Read<float>(health_container + 0x14);
            //float hp = 0.f;
            //hp += health;
            //health = hp / health_max * 100;
        }

        // Position
        uint64_t tmp = m.Read<uint64_t>(player + 0x10);
        tmp = m.Read<uint64_t>(tmp + 0x30);
        tmp = m.Read<uint64_t>(tmp + 0x30);
        tmp = m.Read<uint64_t>(tmp + 0x8);
        tmp = m.Read<uint64_t>(tmp + 0x38);
        Vector3 pos = m.Read<Vector3>(tmp + 0x90);

        if (i == 0)
        {
            LoadlPosition = pos;
            continue;
        }

        if (pos.x == 0.f || pos.y == 0.f || pos.z == 0.f || Health < 0.f)
            continue;

        // Line
        Vector2 ScreenPosition = EFT->WorldToScreen(pos);
        if (ScreenPosition.x == 0.f || ScreenPosition.y == 0.f)
            continue;

        ImColor color = pSide == 4 ? ImColor(1.f, 1.f, 1.f, 1.f) : ImColor(1.f, 0.f, 0.f, 1.f);

        float distance = GetDistance(LoadlPosition, pos);

        // Line
        if (ESP_Line)
            DrawLine(ImVec2((float)GameRect.right / 2.f, (float)GameRect.bottom), ImVec2(ScreenPosition.x, ScreenPosition.y), color, 1.f);

        // Distance
        if (ESP_Distance)
        {
            std::string text = std::to_string((int)distance) + "m";
            String(ImVec2(ScreenPosition.x, ScreenPosition.y), ImColor(1.f, 1.f, 1.f, 1.f), text.c_str());
        }
    }

    ImGui::End();
}