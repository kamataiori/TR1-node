#include <MainLoop.h>

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	MainLoop* mainLoop = new MainLoop;

	mainLoop->WholeLoop();

	delete mainLoop;

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
