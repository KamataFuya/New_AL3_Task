#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>

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

//π
const float pi = 3.1415;

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
	//単位行列(4行4列)
	Matrix4 matIdentity;
	//単位行列
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;
	//範囲forですべてのワールドトランスフォームを順に処理する
	for (WorldTransform& worldTransform : worldTransforms_) {
		//ワールドトランスフォームの初期化
		worldTransform.Initialize();
		//X,Y,Z方向のスケーリングを設定
		worldTransform.scale_ = { 1,1,1 };
		//スケーリング行列の宣言
		Matrix4 matScale;
		//スケーリング行列の設定
		matScale.m[0][0] = worldTransform.scale_.x;//Sx
		matScale.m[1][1] = worldTransform.scale_.y;//Sy
		matScale.m[2][2] = worldTransform.scale_.z;//Sz
		matScale.m[3][3] = 1;
		//X,Y,Z軸周りの回転角を乱数で設定
		std::uniform_real_distribution<float> rotationDist(0.0f, 360.0f);
		worldTransform.rotation_ = { ToRadian(rotationDist(engine)),ToRadian(rotationDist(engine)),ToRadian(rotationDist(engine)) };
		//回転行列の宣言
		Matrix4 matRot = matIdentity;
		//各軸用回転行列を宣言
		Matrix4 matRotX = matIdentity, matRotY = matIdentity, matRotZ = matIdentity;
		//回転行列の設定
		//Z軸回転行列の各要素を設定する
		matRotZ.m[0][0] = cos(worldTransform.rotation_.z);
		matRotZ.m[0][1] = sin(worldTransform.rotation_.z);
		matRotZ.m[1][0] = -sin(worldTransform.rotation_.z);
		matRotZ.m[1][1] = cos(worldTransform.rotation_.z);
		//X軸回転行列の各要素を設定する
		matRotX.m[1][1] = cos(worldTransform.rotation_.x);
		matRotX.m[1][2] = sin(worldTransform.rotation_.x);
		matRotX.m[2][1] = -sin(worldTransform.rotation_.x);
		matRotX.m[2][2] = cos(worldTransform.rotation_.x);
		//Y軸回転行列の各要素を設定する
		matRotY.m[0][0] = cos(worldTransform.rotation_.y);
		matRotY.m[0][2] = -sin(worldTransform.rotation_.y);
		matRotY.m[2][0] = sin(worldTransform.rotation_.y);
		matRotY.m[2][2] = cos(worldTransform.rotation_.y);
		//各軸の回転行列を合成
		matRot *= matRotZ;
		matRot *= matRotX;
		matRot *= matRotY;
		//X,Y,Z軸周りの平行移動(座標)を乱数で設定
		std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);
		worldTransform.translation_ = { posDist(engine),posDist(engine),posDist(engine) };
		//平行移動行列の宣言
		Matrix4 matTrans = MathUtility::Matrix4Identity();
		//移動量を行列に設定する
		matTrans.m[3][0] = posDist(engine);
		matTrans.m[3][1] = posDist(engine);
		matTrans.m[3][2] = posDist(engine);
		//スケーリング・回転・平行移動を合成した行列を計算してワールドトランスフォームに代入
		worldTransform.matWorld_ *= matScale;
		worldTransform.matWorld_ *= matRot;
		worldTransform.matWorld_ *= matTrans;
		//行列の転送
		worldTransform.TransferMatrix();
	}
	//カメラ始点座標を設定
	viewProjection_.eye = { 0,0,-10 };
	//カメラ注視点座標を設定
	viewProjection_.target = { 10.0f,0.0f,0.0f };
	//カメラ上方向ベクトルを設定
	viewProjection_.up = { cosf(pi / 4.0f),sinf(pi / 4.0f),0.0f };
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
	//視点移動処理
	{
		//視点の移動ベクトル
		Vector3 move = {0.0f,0.0f,0.0f};//零ベクトル
		//視点の移動速さ
		const float kEyeSpeed = 0.2f;
		//押した方向でベクトルを変更
		if (input_->PushKey(DIK_W)) {
			move.z += kEyeSpeed;
		}
		else if (input_->PushKey(DIK_S)) {
			move.z -= kEyeSpeed;
		}
		//視点移動(ベクトルの加算)
		viewProjection_.eye += move;
		//行列の再計算
		viewProjection_.UpdateMatrix();
		//デバッグ用表示
		debugText_->SetPos(50, 50);
		debugText_->Printf(
			"eye:(%f,%f,%f)",
			viewProjection_.eye.x,
			viewProjection_.eye.y,
			viewProjection_.eye.z);
	}
	//注視点移動処理
	{
		//注視点の移動ベクトル
		Vector3 move = { 0.0f,0.0f,0.0f };//零ベクトル
		//注視点の移動速さ
		const float kTargetSpeed = 0.2f;
		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move.x -= kTargetSpeed;
		}else if (input_->PushKey(DIK_RIGHT)) {
			move.x += kTargetSpeed;
		}
		//注視点移動(ベクトルの加算)
		viewProjection_.target += move;
		//行列の再計算
		viewProjection_.UpdateMatrix();
		//デバッグ用表示
		debugText_->SetPos(50, 70);
		debugText_->Printf(
			"target:(%f,%f,%f)",
			viewProjection_.target.x,
			viewProjection_.target.y,
			viewProjection_.target.z);
	}
	//上方向回転処理
	{
		//上方向の回転速さ[ラジアン/frame]
		const float kUpRotSpeed = 0.05f;
		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_SPACE)) {
			viewAngle += kUpRotSpeed;
			//2πを越えたら0に戻す
			viewAngle = fmodf(viewAngle, pi * 2.0f);
		}
		//上方向ベクトルを計算(半径1の円周上の座標)
		viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };
		//行列の再計算
		viewProjection_.UpdateMatrix();
		//デバッグ用表示
		debugText_->SetPos(50, 90);
		debugText_->Printf(
			"up:(%f,%f,%f)",
			viewProjection_.up.x,
			viewProjection_.up.y,
			viewProjection_.up.z);
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
	//範囲forですべてのワールドトランスフォームを順に処理する
	for (WorldTransform& worldTransform:worldTransforms_) {
		//3Dモデル描画
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
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
