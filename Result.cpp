#include "Result.h"

void Result::Initialize()
{
}

void Result::Updata()
{

    /*bool show_demo_window = true;
    bool show_another_window = false;
    bool show_custom_node_graph = true;*/

    ShowExampleAppCustomNodeGraph(&show_custom_node_graph);
}

void Result::Draw()
{
}

void Result::ShowExampleAppCustomNodeGraph(bool* opened)
{
    ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Example: Custom Node Graph", opened))
    {
        ImGui::End();
        return;
    }

    struct NodeBase
    {
        int     ID;
        char    Name[32];
        ImVec2  Pos, Size;
        float   Value;
        ImVec4  Color;
        int     InputsCount, OutputsCount;

        NodeBase(int id, const char* name, const ImVec2& pos, float value, const ImVec4& color, int inputs_count, int outputs_count)
        {
            ID = id;
            strcpy_s(Name, name);
            Pos = pos;
            Value = value;
            Color = color;
            InputsCount = inputs_count;
            OutputsCount = outputs_count;
        }

        ImVec2 GetInputSlotPos(int slot_no) const { return ImVec2(Pos.x, Pos.y + Size.y * ((float)slot_no + 1) / ((float)InputsCount + 1)); }
        ImVec2 GetOutputSlotPos(int slot_no) const { return ImVec2(Pos.x + Size.x, Pos.y + Size.y * ((float)slot_no + 1) / ((float)OutputsCount + 1)); }
    };

    struct NodeLink
    {
        int     InputIdx, InputSlot, OutputIdx, OutputSlot;

        NodeLink(int input_idx, int input_slot, int output_idx, int output_slot)
        {
            InputIdx = input_idx;
            InputSlot = input_slot;
            OutputIdx = output_idx;
            OutputSlot = output_slot;
        }
    };

    static ImVector<NodeBase> nodes;
    static ImVector<NodeLink> links;
    static ImVec2 scrolling = ImVec2(0.0f, 0.0f);
    static bool inited = false;
    static bool show_grid = true;
    static int node_selected = -1;

    ImGuiIO& io = ImGui::GetIO();
    if (!inited)
    {
        nodes.push_back(NodeBase(0, "MainTex", ImVec2(40, 50), 0.5f, ImColor(255, 100, 100), 1, 1));
        nodes.push_back(NodeBase(1, "BumpMap", ImVec2(40, 150), 0.42f, ImColor(200, 100, 200), 1, 1));
        nodes.push_back(NodeBase(2, "Combine", ImVec2(270, 80), 1.0f, ImColor(0, 200, 100), 2, 2));
        links.push_back(NodeLink(0, 0, 2, 0));
        links.push_back(NodeLink(1, 0, 2, 1));
        inited = true;
    }

    bool open_context_menu = false;
    int node_hovered_in_list = -1;
    int node_hovered_in_scene = -1;
    ImGui::BeginChild("node_list", ImVec2(100, 0));
    ImGui::Text("Nodes");
    ImGui::Separator();
    for (int node_idx = 0; node_idx < nodes.Size; node_idx++)
    {
        NodeBase* node = &nodes[node_idx];
        ImGui::PushID(node->ID);
        if (ImGui::Selectable(node->Name, node->ID == node_selected))
            node_selected = node->ID;
        if (ImGui::IsItemHovered())
        {
            node_hovered_in_list = node->ID;
            open_context_menu |= ImGui::IsMouseClicked(1);
        }
        ImGui::PopID();
    }
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginGroup();

    const float NODE_SLOT_RADIUS = 4.0f;
    const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);

    ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", scrolling.x, scrolling.y);
    ImGui::SameLine(ImGui::GetWindowWidth() - 100);
    ImGui::Checkbox("Show grid", &show_grid);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(60, 60, 70, 200));
    ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
    ImGui::PopStyleVar(); // WindowPadding
    ImGui::PushItemWidth(120.0f);

    const ImVec2 offset = ImGui::GetCursorScreenPos() + scrolling;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    if (show_grid)
    {
        ImU32 GRID_COLOR = IM_COL32(200, 200, 200, 40);
        float GRID_SZ = 64.0f;
        ImVec2 win_pos = ImGui::GetCursorScreenPos();
        ImVec2 canvas_sz = ImGui::GetWindowSize();
        for (float x = fmodf(scrolling.x, GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
            draw_list->AddLine(ImVec2(x, 0.0f) + win_pos, ImVec2(x, canvas_sz.y) + win_pos, GRID_COLOR);
        for (float y = fmodf(scrolling.y, GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
            draw_list->AddLine(ImVec2(0.0f, y) + win_pos, ImVec2(canvas_sz.x, y) + win_pos, GRID_COLOR);
    }

    draw_list->ChannelsSplit(2);
    draw_list->ChannelsSetCurrent(0); // Background
    for (int link_idx = 0; link_idx < links.Size; link_idx++)
    {
        NodeLink* link = &links[link_idx];
        NodeBase* node_inp = &nodes[link->InputIdx];
        NodeBase* node_out = &nodes[link->OutputIdx];
        ImVec2 p1 = offset + node_inp->GetOutputSlotPos(link->InputSlot);
        ImVec2 p2 = offset + node_out->GetInputSlotPos(link->OutputSlot);
        draw_list->AddBezierCubic(p1, p1 + ImVec2(+50, 0), p2 + ImVec2(-50, 0), p2, IM_COL32(200, 200, 100, 255), 3.0f);
    }

    for (int node_idx = 0; node_idx < nodes.Size; node_idx++)
    {
        NodeBase* node = &nodes[node_idx];
        ImGui::PushID(node->ID);
        ImVec2 node_rect_min = offset + node->Pos;

        draw_list->ChannelsSetCurrent(1); // Foreground
        bool old_any_active = ImGui::IsAnyItemActive();
        ImGui::SetCursorScreenPos(node_rect_min + NODE_WINDOW_PADDING);
        ImGui::BeginGroup(); // Lock horizontal position
        ImGui::Text("%s", node->Name);
        ImGui::SliderFloat("##value", &node->Value, 0.0f, 1.0f, "Alpha %.2f");
        ImGui::ColorEdit3("##color", &node->Color.x);
        ImGui::DragFloat3("Position", &node->Pos.x, 1.0f, 0.0f, 500.0f, "%.1f");
        ImGui::EndGroup();

        bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
        node->Size = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
        ImVec2 node_rect_max = node_rect_min + node->Size;

        draw_list->ChannelsSetCurrent(0); // Background
        ImGui::SetCursorScreenPos(node_rect_min);
        ImGui::InvisibleButton("node", node->Size);
        if (ImGui::IsItemHovered())
        {
            node_hovered_in_scene = node->ID;
            open_context_menu |= ImGui::IsMouseClicked(1);
        }
        bool node_moving_active = ImGui::IsItemActive();
        if (node_widgets_active || node_moving_active)
            node_selected = node->ID;
        if (node_moving_active && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
            node->Pos = node->Pos + io.MouseDelta;

        ImU32 node_bg_color = (node_hovered_in_list == node->ID || node_hovered_in_scene == node->ID || (node_hovered_in_list == -1 && node_selected == node->ID)) ? IM_COL32(75, 75, 75, 255) : IM_COL32(60, 60, 60, 255);
        draw_list->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
        draw_list->AddRect(node_rect_min, node_rect_max, IM_COL32(100, 100, 100, 255), 4.0f);
        for (int slot_idx = 0; slot_idx < node->InputsCount; slot_idx++)
            draw_list->AddCircleFilled(offset + node->GetInputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));
        for (int slot_idx = 0; slot_idx < node->OutputsCount; slot_idx++)
            draw_list->AddCircleFilled(offset + node->GetOutputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));

        // ノード間リンクの追加
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            ImGui::SetDragDropPayload("NODE_LINK", &node->ID, sizeof(int));
            ImGui::Text("Link from %s", node->Name);
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NODE_LINK"))
            {
                int src_id = *(const int*)payload->Data;
                if (src_id != node->ID)
                {
                    links.push_back(NodeLink(src_id, 0, node->ID, 0));
                }
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::PopID();
    }
    draw_list->ChannelsMerge();

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) || !ImGui::IsAnyItemHovered())
        {
            node_selected = node_hovered_in_list = node_hovered_in_scene = -1;
            open_context_menu = true;
        }
    if (open_context_menu)
    {
        ImGui::OpenPopup("context_menu");
        if (node_hovered_in_list != -1)
            node_selected = node_hovered_in_list;
        if (node_hovered_in_scene != -1)
            node_selected = node_hovered_in_scene;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    if (ImGui::BeginPopup("context_menu"))
    {
        NodeBase* node = node_selected != -1 ? &nodes[node_selected] : NULL;
        ImVec2 scene_pos = ImGui::GetMousePosOnOpeningCurrentPopup() - offset;
        if (node)
        {
            ImGui::Text("Node '%s'", node->Name);
            ImGui::Separator();
            if (ImGui::MenuItem("Delete Node"))
            {
                nodes.erase(nodes.begin() + node_selected);
                node_selected = -1;
            }
            if (ImGui::MenuItem("Copy", NULL, false, false)) {}
        }
        else
        {
            if (ImGui::MenuItem("Add Node"))
            {
                nodes.push_back(NodeBase(nodes.Size, "New node", scene_pos, 0.5f, ImColor(100, 100, 200), 2, 2));
            }
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();

    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle, 0.0f))
        scrolling = scrolling + io.MouseDelta;

    ImGui::PopItemWidth();
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::EndGroup();

    ImGui::End();
}
