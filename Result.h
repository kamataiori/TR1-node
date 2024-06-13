#pragma once
#include <Novice.h>
#include <math.h> // fmodfのため
#include "imgui.h" // Dear ImGuiのメインヘッダー
#include "imgui_internal.h" // 内部関数や構造体のため（必要な場合）
#include "imconfig.h" // 追加の設定があれば
#include "imgui_node_test.cpp"

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

class Result
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Updata();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

    /// <summary>
    /// サンプルアプリのカスタムノードグラフを表示
    /// </summary>
    /// <param name="opened"></param>
	static void ShowExampleAppCustomNodeGraph(bool* opened);


	//void ProcessNodes(ImVector<NodeBase>& nodes, ImVector<NodeLink>& links, ImVec2& offset, ImDrawList* draw_list, int& node_selected, int& node_hovered_in_scene, bool& open_context_menu);




private:

	bool show_demo_window = true;
	bool show_another_window = false;
	bool show_custom_node_graph = true;


};

