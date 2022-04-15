#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

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

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	

	//X,Y,Z方向のスケーリングを設定 
	 worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};
	

    //X,Y,Z軸周りの回転角を設定 
	 worldTransform_.rotation_ = {XM_PI / 4.0f, XM_PI / 4.0f, 0.0f};
	
	//X,Y,Z軸周りの回転角を設定 
	// worldTransform_.rotation_ = { 0.0f, XMConvertToRadians(45.0f), 0.0f};
	
	//X,Y,Z軸周りの平行移動を設定 
	worldTransform_.translation_ = {10.0f, 10.0f, 10.0f};

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();








}

void GameScene::Update() 
{


	//書式指定付き表示
	//debugText_->SetPos(50, 70);
	//debugText_->Printf("translation:%f,%f,%f", worldTransform_.translation_);

	//TRANSLATION XYZ
	std::string strDebugTTX =
	  std::string("translation:(") + std::to_string(worldTransform_.translation_.x);
	debugText_->Print(strDebugTTX, 20, 90, 1.0f);

	std::string strDebugTTY =
	  std::string(",") + std::to_string(worldTransform_.translation_.y);
	debugText_->Print(strDebugTTY, 220, 90, 1.0f);

	std::string strDebugTTZ =
	  std::string(",") + std::to_string(worldTransform_.translation_.z);
	debugText_->Print(strDebugTTZ, 310, 90, 1.0f);

	debugText_->SetPos(400, 90);
    debugText_->Printf(")");

	//ROTATION XYZ
	std::string strDebugTRX =
	  std::string("rotation:(") + std::to_string(worldTransform_.rotation_.x);
	debugText_->Print(strDebugTRX, 20, 110, 1.0f);

	std::string strDebugTRY = std::string(",") + std::to_string(worldTransform_.rotation_.y);
	debugText_->Print(strDebugTRY, 185, 110, 1.0f);

	std::string strDebugTRZ = std::string(",") + std::to_string(worldTransform_.rotation_.z);
	debugText_->Print(strDebugTRZ, 275, 110, 1.0f);

	debugText_->SetPos(360, 110);
	debugText_->Printf(")");

	// SCALE XYZ
	std::string strDebugTSX =
	  std::string("scale:(") + std::to_string(worldTransform_.scale_.x);
	debugText_->Print(strDebugTSX, 20, 130, 1.0f);

	std::string strDebugTSY = std::string(",") + std::to_string(worldTransform_.scale_.y);
	debugText_->Print(strDebugTSY, 160, 130, 1.0f);

	std::string strDebugTSZ = std::string(",") + std::to_string(worldTransform_.scale_.z);
	debugText_->Print(strDebugTSZ, 250, 130, 1.0f);

	debugText_->SetPos(330, 130);
	debugText_->Printf(")");


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
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

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
