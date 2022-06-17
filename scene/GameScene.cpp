#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>

float GameScene::ToRadian(float d) {
	d = d * (pi / 180);
	return d;
}

float GameScene::ToDegree(float r) {
	r = r * 180.0f / pi;
	return r;
}

float GameScene::MinNum(float num, float num2) {
	if (num < num2) {
		return num;
	}
	return num2;
}

float GameScene::MaxNum(float num, float num2) {
	if (num > num2) {
		return num;
	}
	return num2;
}

Matrix4 GameScene::CreditMatrix(Matrix4 matIdentity) {
	//単位行列の設定
	matIdentity.m[0][0] = 1.0f;
	matIdentity.m[1][1] = 1.0f;
	matIdentity.m[2][2] = 1.0f;
	matIdentity.m[3][3] = 1.0f;
	//設定した行列を返す
	return matIdentity;
}

Matrix4 GameScene::CreateMatScale(Vector3 scale) {
	Matrix4 matScale;
	//スケーリング行列の設定
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1.0f;
	//設定した行列を返す
	return matScale;
}

Matrix4 GameScene::CreateMatRotation(Vector3 rotation) {
	Matrix4 matIdentity;
	//回転行列の宣言
	Matrix4 matRot = CreditMatrix(matIdentity);
	//各軸用回転行列を宣言
	Matrix4 matRotX = CreditMatrix(matIdentity),
			matRotY = CreditMatrix(matIdentity),
			matRotZ = CreditMatrix(matIdentity);
	//回転行列の設定
	//Z軸回転行列の各要素を設定する
	matRotZ.m[0][0] = cos(rotation.z);
	matRotZ.m[0][1] = sin(rotation.z);
	matRotZ.m[1][0] = -sin(rotation.z);
	matRotZ.m[1][1] = cos(rotation.z);
	//X軸回転行列の各要素を設定する
	matRotX.m[1][1] = cos(rotation.x);
	matRotX.m[1][2] = sin(rotation.x);
	matRotX.m[2][1] = -sin(rotation.x);
	matRotX.m[2][2] = cos(rotation.x);
	//Y軸回転行列の各要素を設定する
	matRotY.m[0][0] = cos(rotation.y);
	matRotY.m[0][2] = -sin(rotation.y);
	matRotY.m[2][0] = sin(rotation.y);
	matRotY.m[2][2] = cos(rotation.y);
	//各軸の回転行列を合成
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;
	//設定した行列を返す
	return matRot;
}

Matrix4 GameScene::CreateMatTranslation(Vector3 translation) {
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	//平行移動行列の設定
	matTrans.m[3][0] = translation.x;
	matTrans.m[3][1] = translation.y;
	matTrans.m[3][2] = translation.z;
	//設定した行列を返す
	return matTrans;
}

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;
	delete debugCamera_;
	delete player_;//自キャラの開放
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	//3Dモデルの生成
	model_ = Model::Create();
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//乱数範囲の指定(使用例)
	std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_,textureHandle_);
}

void GameScene::Update() {
	//デバッグカメラの更新
	debugCamera_->Update();
	//自キャラの更新
	player_->Update();
	//キャラクター移動処理
	{
		//キャラクターの移動ベクトル
		Vector3 move = { 0.0f,0.0f,0.0f };
		//左か右キーを押していたらmove(移動量)を変化させる
		if (input_->PushKey(DIK_RIGHT)) {
			move = { 0.2f,0.0f,0.0f };
		}
		else if (input_->PushKey(DIK_LEFT)) {
			move = { -0.2f,0.0f,0.0f };
		}
	}
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
	//自キャラの描画
	player_->Draw(viewProjection_);

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
