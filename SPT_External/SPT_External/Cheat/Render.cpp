#include "Cheat.h"

extern Tarkov EFT;

// tmp
float LocalHealth;
Vector3 LocalPosition;

float GetDistance(Vector3 value1, Vector3 value2);

void Cheat::RenderInfo()
{
    ImGui::SetNextWindowPos(ImVec2((float)g.GameSize.right - 300.f, 12.f));
    ImGui::Begin("Debug", (bool*)NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

    std::string text = EFT.s.ReadSuccess ? "Success" : "Fail";
    ImColor col = EFT.s.ReadSuccess ? ImColor(0.f, 1.f, 0.f, 1.f) : ImColor(1.f, 0.f, 0.f, 1.f);

    ImGui::Text("Game");
    ImGui::Separator();
    ImGui::Text("Status :"); ImGui::SameLine();
    ImGui::TextColored(col, "%s", text.c_str());
    ImGui::Text("   GameWorld    : 0x%I64x", &EFT.GameWorld);
    ImGui::Text(" LGWorld - Old  : 0x%I64x", &EFT.LocalGameWorld);
    ImGui::Text(" LGW - Current  : 0x%I64x", &EFT.s.NewLocalGameWorld);
    ImGui::Text("  FPS Camera    : 0x%I64x", &EFT.FpsCamera);

    ImGui::NewLine();
    ImGui::Text("Player");
    ImGui::Separator();
    ImGui::Text(" Health  : %.f", LocalHealth);
    ImGui::Text("Position : %.f, %.f, %.f", LocalPosition.x, LocalPosition.y, LocalPosition.z);

    text = EFT.s.InRaid ? "Raid" : "Menu";
    col = EFT.s.InRaid ? ImColor(0.f, 1.f, 0.f, 1.f) : ImColor(1.f, 0.f, 0.f, 1.f);

    ImGui::NewLine();
    ImGui::Text("System");
    ImGui::Separator();
    ImGui::Text("Status :"); ImGui::SameLine();
    ImGui::TextColored(col, "%s", text.c_str());

    ImGui::End();
}

void Cheat::RenderMenu()
{
    ImGui::SetNextWindowSize(ImVec2(600.f, 550.f));
    ImGui::Begin("SPT-AKI [ EXTERNAL ]", (bool*)NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Checkbox("ESP", &g.ESP);
    ImGui::Checkbox("ESP Line", &g.ESP_Line);
    ImGui::Checkbox("ESP Distance", &g.ESP_Distance);

    ImGui::End();
}

void Cheat::RenderESP()
{
    // ImGui Window
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)g.GameSize.right, (float)g.GameSize.bottom));
    ImGui::Begin("##ESP", (bool*)NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs);

    if (!EFT.s.ReadSuccess || LocalPosition == Vector3(0.f, 0.f, 0.f) && LocalHealth < 0.f)
    {
        EFT.s.InRaid = false;
    }
    else
    {
        EFT.s.InRaid = true;
    }

    LocalPosition = Vector3(0.f, 0.f, 0.f);
    LocalHealth = 0.f;

    uint64_t OnlineUser = m.Read<uint64_t>(EFT.LocalGameWorld + 0xC0); // LocalGameWorld -> RegisterdPlayers

    uint64_t PlayerListPtr= m.Read<uint64_t>(OnlineUser + offsetof(List, listBase));
    List PlayerList = m.Read<List>(PlayerListPtr);

    if (PlayerList.itemCount <= 0)
        return;

    for (int i = 0; i < PlayerList.itemCount; i++)
    {
        uint64_t player = m.Read<uint64_t>(PlayerListPtr + (offsetof(ListInternal, firstEntry) + (i * 8)));

        if (!player)
            break;

        uint64_t EFT_Profile = m.Read<uint64_t>(player + 0x530); // EFT.Profile
        uint64_t PlayerInfo = m.Read<uint64_t>(EFT_Profile + 0x28);

        int PlayerSide = m.Read<int>(PlayerInfo + 0x70); // 1 : Usec, 2 : Bear, 4 : Scavs

        // Health
        uint64_t m_pHealthController = m.Read<uint64_t>(player + 0x570);
        uint64_t m_pHealthBody = m.Read<uint64_t>(m_pHealthController + 0x68);
        uint64_t m_pBodyController = m.Read<uint64_t>(m_pHealthBody + 0x18);

        float Health = 0.f;
       
        for (int h = 0; h <= 6; h++)
        {
            auto body_part = m.Read<uintptr_t>(m_pBodyController + 0x30 + (h * 0x18));
            auto health_container = m.Read<uintptr_t>(body_part + 0x10);
            float hp = hp = m.Read<float>(health_container + 0x10);
            Health += hp;

            //float MaxHealth = m.Read<float>(health_container + 0x14);
        }

        // Position - tmp
        uint64_t tmp = m.Read<uint64_t>(player + 0x10);
        tmp = m.Read<uint64_t>(tmp + 0x30);
        tmp = m.Read<uint64_t>(tmp + 0x30);
        tmp = m.Read<uint64_t>(tmp + 0x8);
        tmp = m.Read<uint64_t>(tmp + 0x38);
        Vector3 pos = m.Read<Vector3>(tmp + 0x90);

        // tmp
        if (i == 0)
        {
            LocalHealth = Health;
            LocalPosition = pos;

            continue;
        }
        else if (pos.x == 0.f || pos.y == 0.f || pos.z == 0.f || Health < 0.f)
        {
            continue;
        }

        // Line
        Vector2 ScreenPosition = EFT.WorldToScreen(pos);

        if (ScreenPosition.x == 0.f || ScreenPosition.y == 0.f)
            continue;

        ImColor color = PlayerSide == 4 ? ImColor(1.f, 1.f, 1.f, 1.f) : ImColor(1.f, 0.f, 0.f, 1.f);

        float distance = GetDistance(LocalPosition, pos);

        // Line
        if (g.ESP_Line)
            DrawLine(ImVec2((float)g.GameSize.right / 2.f, (float)g.GameSize.bottom), ImVec2(ScreenPosition.x, ScreenPosition.y), color, 1.f);

        // Distance
        if (g.ESP_Distance)
        {
            std::string text = std::to_string((int)distance) + "m";
            String(ImVec2(ScreenPosition.x, ScreenPosition.y), ImColor(1.f, 1.f, 1.f, 1.f), text.c_str());
        }
    }

    ImGui::End();
}

float GetDistance(Vector3 value1, Vector3 value2)
{
    float num = value1.x - value2.x;
    float num2 = value1.y - value2.y;
    float num3 = value1.z - value2.z;

    return sqrt(num * num + num2 * num2 + num3 * num3);
}