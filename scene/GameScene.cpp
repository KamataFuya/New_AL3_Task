#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>

float GameScene::ToRadian(float d) {
	const float pi = 3.14;
	d = d * (pi / 180);
	return d;
}

float GameScene::ToDegree(float r) {
	const float pi = 3.14;
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
	////親(0番)
	//worldTransforms_[0].Initialize();
	////子(1番)
	//worldTransforms_[1].Initialize();
	//worldTransforms_[1].translation_ = { 0,4.5f,0 };//ローカル座標
	//worldTransforms_[1].parent_ = &worldTransforms_[0];

	//キャラクターの大元
	worldTransforms_[PartId::kRoot].Initialize();
	//脊椎
	worldTransforms_[PartId::kSpine].Initialize();
	worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];
	worldTransforms_[PartId::kSpine].translation_ = { 0,4.5f,0 };
	//上半身
	//胸
	worldTransforms_[PartId::kChest].Initialize();//Chestの初期化
	worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];//親をSpineに指定
	worldTransforms_[PartId::kChest].translation_ = { 0,0,0 };//Spineからのローカル座標を設定
	//頭
	worldTransforms_[PartId::kHead].Initialize();//Headの初期化
	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];//親をChestに指定
	worldTransforms_[PartId::kHead].translation_ = { 0,3.5f,0 };//Chestからのローカル座標を設定
	//左腕
	worldTransforms_[PartId::kArmL].Initialize();//ArmLの初期化
	worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];//親をChestに指定
	worldTransforms_[PartId::kArmL].translation_ = { 3.5f,0,0 };//Chestからのローカル座標を設定
	//右腕
	worldTransforms_[PartId::kArmR].Initialize();//ArmRの初期化
	worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];//親をChestに指定
	worldTransforms_[PartId::kArmR].translation_ = { -3.5f,0,0 };//Chestからのローカル座標を設定
	//下半身
	//尻
	worldTransforms_[PartId::kHip].Initialize();//Hipの初期化
	worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];//親をSpineに指定
	worldTransforms_[PartId::kHip].translation_ = { 0,-3.5f,0 };//Spineからのローカル座標を設定
	//左足
	worldTransforms_[PartId::kLegL].Initialize();//LegLの初期化
	worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];//親をHipに指定
	worldTransforms_[PartId::kLegL].translation_ = { 3.0f,-3.5f,0 };//Hipからのローカル座標を設定
	//右足
	worldTransforms_[PartId::kLegR].Initialize();//LegRの初期化
	worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];//親をHipに指定
	worldTransforms_[PartId::kLegR].translation_ = { -3.0f,-3.5f,0 };//Hipからのローカル座標を設定

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	//デバッグカメラの更新
	debugCamera_->Update();
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
		//ワールドトランスフォームの初期化
		worldTransforms_[PartId::kRoot].Initialize();
		//worldTransforms_[PartId::kRoot]のtranslationにmoveを加算する(移動)
		worldTransforms_[PartId::kRoot].translation_.x += move.x;
		//大元から順に更新していく
		for (int i = 0; i < kNumPartId; i++) {
			//worldTransforms_[i]のスケーリング・回転・平行移動を合成した行列を計算する
			//worldTransforms_[0]のワールド行列を計算する
			//単位行列の生成
			Matrix4 matIdentity;
			matIdentity = CreditMatrix(matIdentity);
			//ワールド行列に単位行列を代入
			worldTransforms_[i].matWorld_ = matIdentity;
			//スケーリング行列の生成
			Matrix4 matScale = CreateMatScale(worldTransforms_[i].scale_);
			//回転行列の生成
			Matrix4 matRot = CreateMatRotation(worldTransforms_[i].rotation_);
			//平行移動行列の生成
			Matrix4 matTrans = CreateMatTranslation(worldTransforms_[i].translation_);
			//スケーリング・回転・平行移動を合成した行列を計算してmatWorldに代入
			worldTransforms_[i].matWorld_ *= matScale;
			worldTransforms_[i].matWorld_ *= matRot;
			worldTransforms_[i].matWorld_ *= matTrans;
			//worldTransforms_[i]にparent_のワールド行列を掛け算代入する
			if (worldTransforms_[i].parent_) {//ヌルチェック
				worldTransforms_[i].matWorld_ *= worldTransforms_[i].parent_->matWorld_;
			}
			//wordlTransforms[i]のワールド行列を転送
			worldTransforms_[i].TransferMatrix();
		}
		////worldTransforms_[0]のワールド行列を計算する
		////単位行列の生成
		//Matrix4 matIdentity;
		//matIdentity = CreditMatrix(matIdentity);
		////ワールド行列に単位行列を代入
		//worldTransforms_[0].matWorld_ = matIdentity;
		////スケーリング行列の生成
		//Matrix4 matScale = CreateMatScale(worldTransforms_[0].scale_);
		////回転行列の生成
		//Matrix4 matRot = CreateMatRotation(worldTransforms_[0].rotation_, matIdentity);
		////平行移動行列の生成
		//Matrix4 matTrans = CreateMatTranslation(worldTransforms_[0].translation_);
		////スケーリング・回転・平行移動を合成した行列を計算してmatWorldに代入
		//worldTransforms_[0].matWorld_ *= matScale;
		//worldTransforms_[0].matWorld_ *= matRot;
		//worldTransforms_[0].matWorld_ *= matTrans;
		////worldTransforms_[0]のワールド行列を転送
		//worldTransforms_[0].TransferMatrix();
		//worldTransforms_[PartId::kRoot]のワールドtranslationをデバッグ表示する
		debugText_->SetPos(50, 150);
		debugText_->Printf(
			"Root : (%f,%f,%f)", worldTransforms_[PartId::kRoot].translation_.x,
			worldTransforms_[PartId::kRoot].translation_.y,
			worldTransforms_[PartId::kRoot].translation_.z);
	}
	//子の更新
	//{
	//	//worldTransforms_[1]のスケーリング・回転・平行移動を合成した行列を計算する
	//	//単位行列の生成
	//	Matrix4 matIdentity;
	//	matIdentity = CreditMatrix(matIdentity);
	//	//ワールド行列に単位行列を代入
	//	worldTransforms_[1].matWorld_ = matIdentity;
	//	//スケーリング行列の生成
	//	Matrix4 matScale = CreateMatScale(worldTransforms_[1].scale_);
	//	//回転行列の生成
	//	Matrix4 matRot = CreateMatRotation(worldTransforms_[1].rotation_);
	//	//平行移動行列の生成
	//	Matrix4 matTrans = CreateMatTranslation(worldTransforms_[1].translation_);
	//	//スケーリング・回転・平行移動を合成した行列を計算してmatWorldに代入
	//	worldTransforms_[1].matWorld_ *= matScale;
	//	worldTransforms_[1].matWorld_ *= matRot;
	//	worldTransforms_[1].matWorld_ *= matTrans;
	//	//worldTransforms_[1]にworldTransforms_[0](parent_)のワールド行列を掛け算代入する
	//	worldTransforms_[1].matWorld_ *= worldTransforms_[0].matWorld_;
	//	//worldTransforms_[1]のワールド行列を転送
	//	worldTransforms_[1].TransferMatrix();
	//}

	//上半身回転処理
	{
		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_U)) {
			//胸パーツのY軸周りの角度を減少
			worldTransforms_[PartId::kChest].rotation_.y -= kChestRotSpeed;
		}
		else if (input_->PushKey(DIK_I)) {
			//胸パーツのY軸周りの角度を増加
			worldTransforms_[PartId::kChest].rotation_.y += kChestRotSpeed;
		}
	}

	//下半身回転処理
	{
		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_J)) {
			//尻パーツのY軸周りの角度を減少
			worldTransforms_[PartId::kHip].rotation_.y -= kHipRotSpeed;
		}
		else if (input_->PushKey(DIK_K)) {
			//尻パーツのY軸周りの角度を増加
			worldTransforms_[PartId::kHip].rotation_.y += kHipRotSpeed;
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
	//大元から順に描画していく
	for (int i = 0; i < kNumPartId; i++) {
		if (i < 2) {
			continue;
		}
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
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
