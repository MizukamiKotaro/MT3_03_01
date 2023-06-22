#include <Novice.h>
#include"MyMatrix4x4.h"
#include"MatrixScreenPrintf.h"
#include"MyVector3.h"
#include"VectorScreenPrintf.h"
#include"calc.h"
#include"Grid.h"
#include"Sphere.h"
#include<imgui.h>
#include"Line.h"
#include"Collision.h"
#include"Camera.h"
#include"Plane.h"

const char kWindowTitle[] = "学籍番号";

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

const int kWindowWidth = 1280;
const int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Camera* camera = new Camera();
	camera->Initialize({ 0.0f,1.9f,-6.49f }, { 0.26f,0.0f,0.0f });

	MyMatrix4x4 originMatrix = MyMatrix4x4::MakeAffinMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
	
	Segment segment = {
		{0.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f}
	};

	Ray ray = {
		{1.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f}
	};

	Line line = {
		{-1.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f}
	};

	Plane plane = {
		{0.0f,1.0f,0.0f},
		1.0f
	};
	
	MyMatrix4x4 projectionMatrix = MyMatrix4x4::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
	MyMatrix4x4 viewportMatrix = MyMatrix4x4::MakeViewportMatrix(0.0f, 0.0f, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		
		ImGui::Begin("Window");
		ImGui::Text("R : Reset");
		ImGui::DragFloat3("segment.origin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("segment.diff", &segment.diff.x, 0.01f);
		ImGui::DragFloat3("ray.origin", &ray.origin.x, 0.01f);
		ImGui::SliderFloat3("ray.diff", &ray.diff.x, -1.0f, 1.0f);
		ImGui::DragFloat3("line.origin", &line.origin.x, 0.01f);
		ImGui::SliderFloat3("line.diff", &line.diff.x, -1.0f, 1.0f);
		ImGui::DragFloat3("plane.normal", &plane.normal.x, 0.01f);
		ImGui::DragFloat("plane.distance", &plane.distance, 0.01f);
		ImGui::Checkbox("plane.limit", &plane.isLimit);
		ImGui::End();
		plane.normal = Calc::Normalize(plane.normal);
		ray.diff= Calc::Normalize(ray.diff);
		line.diff = Calc::Normalize(line.diff);

		if (keys[DIK_R]) {
			segment = {
				{0.0f,0.0f,0.0f},
				{0.0f,1.0f,0.0f}
			};
			ray = {
				{1.0f,0.0f,0.0f},
				{0.0f,1.0f,0.0f}
			};

			line = {
				{-1.0f,0.0f,0.0f},
				{0.0f,1.0f,0.0f}
			};
			plane = {
				{0.0f,1.0f,0.0f},
				1.0f
			};
			camera->Initialize({ 0.0f,1.9f,-6.49f }, { 0.26f,0.0f,0.0f });
		}

		camera->Update(keys, preKeys);

		MyMatrix4x4 cameraMatrix = MyMatrix4x4::MakeAffinMatrix(camera->GetScale(), camera->GetRotate(), camera->GetTranslate());
		MyMatrix4x4 viewMatrix = MyMatrix4x4::Inverse(cameraMatrix);
		MyMatrix4x4 viewProjectionMatrix = MyMatrix4x4::Multiply(viewMatrix, projectionMatrix);

		MyVector3 start = MyMatrix4x4::Transform(MyMatrix4x4::Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
		MyVector3 end = MyMatrix4x4::Transform(MyMatrix4x4::Transform(segment.origin + segment.diff, viewProjectionMatrix), viewportMatrix);

		MyVector3 start1 = MyMatrix4x4::Transform(MyMatrix4x4::Transform(ray.origin, viewProjectionMatrix), viewportMatrix);
		MyVector3 end1 = MyMatrix4x4::Transform(MyMatrix4x4::Transform(ray.origin + ray.diff, viewProjectionMatrix), viewportMatrix);

		MyVector3 start2 = MyMatrix4x4::Transform(MyMatrix4x4::Transform(line.origin, viewProjectionMatrix), viewportMatrix);
		MyVector3 end2 = MyMatrix4x4::Transform(MyMatrix4x4::Transform(line.origin + line.diff, viewProjectionMatrix), viewportMatrix);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		


		Grid::DrawGrid(viewProjectionMatrix, viewportMatrix);

		if (Collision::IsCollision(segment, plane)) {
			Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0xFF0000FF);
		}
		else {
			Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0xFFFFFFFF);
		}
		if (Collision::IsCollision(ray, plane)) {
			Novice::DrawLine(int(start1.x), int(start1.y), int(end1.x), int(end1.y), 0x00FF00FF);
		}
		else {
			Novice::DrawLine(int(start1.x), int(start1.y), int(end1.x), int(end1.y), 0xFFFFFFFF);
		}
		if (Collision::IsCollision(line, plane)) {
			Novice::DrawLine(int(start2.x), int(start2.y), int(end2.x), int(end2.y), 0x0000FFFF);
		}
		else {
			Novice::DrawLine(int(start2.x), int(start2.y), int(end2.x), int(end2.y), 0xFFFFFFFF);
		}
		
		Plane::Draw(plane, viewProjectionMatrix, viewportMatrix, 0xFFFFFFFF);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	delete camera;

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
