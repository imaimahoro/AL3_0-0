#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	// MODEL
	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();

	// worldTransform_.Initialize();
	viewProjection_.Initialize();

	//乱数シード生成器
	std ::random_device seed_gen;
	//メルセンヌ・ツイスタ-
	std ::mt19937_64 engine(seed_gen());
	//乱数範囲（回転角用）
	std ::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	//乱数範囲（座標用）
	std ::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	//カメラ視点座標を設定
	// viewProjection_.eye = {0, 0, -10};
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//カメラ注視点座標を設定
	// viewProjection_.target = {10, 0, 0};
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//カメラ上方向ベクトルを指定
	// viewProjection_.up=
	//{cosf(XM_PI/4.0f),sinf(XM_PI/4.0f),0.0f};
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	for (size_t i = 0; i < _countof(worldTransform_); i++) {

		// X,Y,Z方向のスケーリングを設定
		worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f};

		// X,Y,Z軸周りの回転角を設定
		worldTransform_[i].rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};

		// X,Y,Z軸周りの回転角を設定
		//  worldTransform_.rotation_ = { 0.0f, XMConvertToRadians(45.0f), 0.0f};

		// X,Y,Z軸周りの平行移動を設定
		worldTransform_[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};

		//ワールドトランスフォームの初期化
		worldTransform_[i].Initialize();
	}

	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = XMConvertToRadians(10.0f);
	//ビューロジェクションの初期化
	viewProjection_.Initialize();

	//アスペクト比を設定 
	//viewProjection_.aspectRatio = 1.0f;
	//ビュープロジェクションの初期化 
	//viewProjection_.Initialize();


	//ニアクリップ距離を設定 
	viewProjection_.nearZ = 52.0f;
	//ファークリップ距離を設定 
	viewProjection_.farZ = 53.0f;
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();


















}

void GameScene::Update() {
	//視点移動処理
	//{
	//	//視点の移動ベクトル
	//	XMFLOAT3 move = {0, 0, 0};
	//	//視点の移動速さ
	//	const float kEyeSpeed = 0.2f;
	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_W)) {
	//		move = {0, 0, kEyeSpeed};
	//	} else if (input_->PushKey(DIK_S)) {
	//		move = {0, 0, -kEyeSpeed};
	//	}

	//	//視点移動（ベクトルの加算）
	//	viewProjection_.eye.x += move.x;
	//	viewProjection_.eye.y += move.y;
	//	viewProjection_.eye.z += move.z;
	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();
		//デバッグ用表示
		debugText_->SetPos(50, 50);
		debugText_->Printf(
		  " eye: (% f, % f, % f) ", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	//}

	////注視点移動処理
	//{
	//	//注視点の移動ベクトル
	//	XMFLOAT3 move = {0, 0, 0};
	//	//注視点の移動速さ
	//	const float kTargetSpeed = 0.2f;
	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_LEFT)) {
	//		move = {-kTargetSpeed, 0, 0};
	//	} else if (input_->PushKey(DIK_RIGHT)) {
	//		move = {kTargetSpeed, 0, 0};
	//	}

	//	//注視点移動（ベクトルの加算）
	//	viewProjection_.target.x += move.x;
	//	viewProjection_.target.y += move.y;
	//	viewProjection_.target.z += move.z;
	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバッグ用表示
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	 "target: (% f, % f, % f) ", viewProjection_.target.x, viewProjection_.target.y,viewProjection_.target.z);
	//}

	//上方向回転処理
	{
		//上方向の回転速さ[ラジアン/frame]
		const float kUpRotSpeed = 0.05f;
		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_SPACE)) {
			viewAngle += kUpRotSpeed;
			// 2πを超えたら0に戻す
			viewAngle = fmodf(viewAngle, XM_2PI);
		}
		//上方向ベクトルを計算（半径1の円周上の座標）
		viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};
		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 90);
		debugText_->Printf(
		  "up: (% f, % f, % f) ", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
	}

	// FoV変更処理
	{

		//上キーで視野角が広がる
		if (input_->PushKey(DIK_W)) {
			viewProjection_.fovAngleY += 0.01f;
			viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, XM_PI);
			//下キーで視野角が狭まる
		} else if (input_->PushKey(DIK_S)) {
			viewProjection_.fovAngleY -= 0.01f;
			viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
		}
		//行列の再計算
		viewProjection_.UpdateMatrix();
		//デバッグ用表示 
		debugText_->SetPos(50, 110);
		debugText_->Printf("fovAngleY(Degree)%f",XMConvertToDegrees(viewProjection_.fovAngleY));
	}



	//クリップ距離変更処理 
	{
		//上下キーでニアクリプ距離を増減 
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.nearZ += 0.1f;
		}
		else if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.nearZ -= 0.1f;
		}
		//行列の再計算 
		viewProjection_.UpdateMatrix();
		//デバッグ用表示 
		debugText_->SetPos(50, 130);
		debugText_->Printf("nearZ:%f",viewProjection_.nearZ);
	}




	
















		//書式指定付き表示
		// debugText_->SetPos(50, 70);
		// debugText_->Printf("translation:%f,%f,%f", worldTransform_.translation_);

		// TRANSLATION XYZ
		// std::string strDebugTTX =
		//   std::string("translation:(") + std::to_string(worldTransform_.translation_.x);
		// debugText_->Print(strDebugTTX, 20, 90, 1.0f);

		// std::string strDebugTTY =
		//   std::string(",") + std::to_string(worldTransform_.translation_.y);
		// debugText_->Print(strDebugTTY, 220, 90, 1.0f);

		// std::string strDebugTTZ =
		//   std::string(",") + std::to_string(worldTransform_.translation_.z);
		// debugText_->Print(strDebugTTZ, 310, 90, 1.0f);

		// debugText_->SetPos(400, 90);
		//   debugText_->Printf(")");

		////ROTATION XYZ
		// std::string strDebugTRX =
		//   std::string("rotation:(") + std::to_string(worldTransform_.rotation_.x);
		// debugText_->Print(strDebugTRX, 20, 110, 1.0f);

		// std::string strDebugTRY = std::string(",") + std::to_string(worldTransform_.rotation_.y);
		// debugText_->Print(strDebugTRY, 185, 110, 1.0f);

		// std::string strDebugTRZ = std::string(",") + std::to_string(worldTransform_.rotation_.z);
		// debugText_->Print(strDebugTRZ, 275, 110, 1.0f);

		// debugText_->SetPos(360, 110);
		// debugText_->Printf(")");

		//// SCALE XYZ
		// std::string strDebugTSX =
		//   std::string("scale:(") + std::to_string(worldTransform_.scale_.x);
		// debugText_->Print(strDebugTSX, 20, 130, 1.0f);

		// std::string strDebugTSY = std::string(",") + std::to_string(worldTransform_.scale_.y);
		// debugText_->Print(strDebugTSY, 160, 130, 1.0f);

		// std::string strDebugTSZ = std::string(",") + std::to_string(worldTransform_.scale_.z);
		// debugText_->Print(strDebugTSZ, 250, 130, 1.0f);

		// debugText_->SetPos(330, 130);
		// debugText_->Printf(")");
	}

	void GameScene::Draw() {

		// コマンドリストの取得
		ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
		// 背景スプライト描画前処理
		Sprite::PreDraw(commandList);

		/// <summary>
		/// ここに背景スプライトの描画処理を追加できる
		/// </summary>

		// スプライト描画後処理
		Sprite::PostDraw();
		// 深度バッファクリア
		dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
		// 3Dオブジェクト描画前処理
		Model::PreDraw(commandList);

		/// <summary>
		/// ここに3Dオブジェクトの描画処理を追加できる
		/// </summary>

		for (size_t i = 0; i < _countof(worldTransform_); i++) {
			model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
		}

		// 3Dオブジェクト描画後処理
		Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
		// 前景スプライト描画前処理
		Sprite::PreDraw(commandList);

		/// <summary>
		/// ここに前景スプライトの描画処理を追加できる
		/// </summary>

		// デバッグテキストの描画
		debugText_->DrawAll(commandList);
		//
		// スプライト描画後処理
		Sprite::PostDraw();

#pragma endregion
	}
