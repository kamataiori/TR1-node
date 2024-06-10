#pragma once
#include <Novice.h>
#include <Result.h>
#include <math.h> // fmodfのため
#include "imgui.h" // Dear ImGuiのメインヘッダー
#include "imgui_internal.h" // 内部関数や構造体のため（必要な場合）
#include "imconfig.h" // 追加の設定があれば

const char kWindowTitle[] = "LE2B_16_ツユキ_イオリ_node";

class MainLoop {

private:

	//// キー入力結果を受け取る箱
	char keys[256];
	char preKeys[256];

	Result* result_;

public:

	MainLoop();

	~MainLoop();

	/// 
	/// キー入力を受け取る
	/// 
	void InPut();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// 
	/// 更新処理
	/// 
	void Updata();

	///
	/// 描画処理
	///
	void Draw();

	///
	/// 全体のループ
	/// 
	void WholeLoop();
};


