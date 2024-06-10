#pragma once
#include <Novice.h>
#include <math.h> // fmodfのため
#include "imgui.h" // Dear ImGuiのメインヘッダー
#include "imgui_internal.h" // 内部関数や構造体のため（必要な場合）
#include "imconfig.h" // 追加の設定があれば
#include "imgui_node_graph_test.cpp"

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

private:

	bool show_demo_window = true;
	bool show_another_window = false;
	bool show_custom_node_graph = true;


};

