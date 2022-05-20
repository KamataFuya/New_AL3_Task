#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

//ラジアンに変換する関数
float ToRadian(float d) {
	const float pi = 3.14;
	d = d * (pi / 180);
	return d;
}

//単位行列を作る関数
Matrix4 CreditMatrix(Matrix4 matIdentity) {
	matIdentity.m[0][0] = 1.0f;
	matIdentity.m[1][1] = 1.0f;
	matIdentity.m[2][2] = 1.0f;
	matIdentity.m[3][3] = 1.0f;
	return matIdentity;
}

//スケーリング行列を作る関数
Matrix4 CreateMatScale(Vector3 scale) {
	Matrix4 matScale;
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1.0f;
	return matScale;
}

//回転行列を作る関数
//Matrix4 RotationMatrix(Vector3 rotation) {
//
//}

//平行移動行列を作る関数
Matrix4 TranslationMatrix(Vector3 translation, float Tx, float Ty, float Tz) {
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	matTrans.m[3][0] = Tx;
	matTrans.m[3][1] = Ty;
	matTrans.m[3][2] = Tz;
	return matTrans;
}

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;
	delete debugCamera_;

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
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//単位行列(4行4列)
	Matrix4 matIdentity;
	//単位行列
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;
	
	//X,Y,Z方向のスケーリングを設定
	worldTransform_.scale_ = { 5,1,1 };
	//スケーリング行列の宣言
	Matrix4 matScale;
	//スケーリング行列の設定
	matScale.m[0][0] = worldTransform_.scale_.x;//Sx
	matScale.m[1][1] = worldTransform_.scale_.y;//Sy
	matScale.m[2][2] = worldTransform_.scale_.z;//Sz
	matScale.m[3][3] = 1;
	//X,Y,Z軸周りの回転角を設定
	worldTransform_.rotation_ = { ToRadian(45.0f),ToRadian(45.0f),ToRadian(45.0f) };
	//回転行列の宣言
	Matrix4 matRot = matIdentity;
	//各軸用回転行列を宣言
	Matrix4 matRotX = matIdentity, matRotY = matIdentity, matRotZ = matIdentity;
	//回転行列の設定
	//Z軸回転行列の各要素を設定する
	matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);
	//X軸回転行列の各要素を設定する
	matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform_.rotation_.x);
	//Y軸回転行列の各要素を設定する
	matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
	matRotY.m[0][2] = -sin(worldTransform_.rotation_.y);
	matRotY.m[2][0] = sin(worldTransform_.rotation_.y);
	matRotY.m[2][2] = cos(worldTransform_.rotation_.y);
	//各軸の回転行列を合成
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;
	//平行移動行列の宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	//移動量を行列に設定する
	matTrans.m[3][0] = 0;
	matTrans.m[3][1] = 10;
	matTrans.m[3][2] = 0;

	//行列の合成
	worldTransform_.matWorld_ = matIdentity;//単位行列を代入
	worldTransform_.matWorld_ *= matScale;
	worldTransform_.matWorld_ *= matRot;
	worldTransform_.matWorld_ *= matTrans;

	//行列の転送
	worldTransform_.TransferMatrix();

}

void GameScene::Update() {
	//デバッグカメラの更新
	debugCamera_->Update();

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
	//3Dモデル描画
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

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
