#include "Enemy.h"

float Enemy::ToRadian(float d) {
	const float pi = 3.14f;
	d = d * (pi / 180);
	return d;
}

float Enemy::ToDegree(float r) {
	const float pi = 3.14f;
	r = r * 180.0f / pi;
	return r;
}

float Enemy::MinNum(float num, float num2) {
	if (num < num2) {
		return num;
	}
	return num2;
}

float Enemy::MaxNum(float num, float num2) {
	if (num > num2) {
		return num;
	}
	return num2;
}

Matrix4 Enemy::CreditMatrix(Matrix4 matIdentity) {
	//単位行列の設定
	matIdentity.m[0][0] = 1.0f;
	matIdentity.m[1][1] = 1.0f;
	matIdentity.m[2][2] = 1.0f;
	matIdentity.m[3][3] = 1.0f;
	//設定した行列を返す
	return matIdentity;
}

Matrix4 Enemy::CreateMatScale(Vector3 scale) {
	Matrix4 matScale;
	//スケーリング行列の設定
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1.0f;
	//設定した行列を返す
	return matScale;
}

Matrix4 Enemy::CreateMatRotation(Vector3 rotation) {
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

Matrix4 Enemy::CreateMatTranslation(Vector3 translation) {
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	//平行移動行列の設定
	matTrans.m[3][0] = translation.x;
	matTrans.m[3][1] = translation.y;
	matTrans.m[3][2] = translation.z;
	//設定した行列を返す
	return matTrans;
}

void Enemy::MatrixUpdate(WorldTransform& worldtransform) {
	//行列更新
	//単位行列の生成
	Matrix4 matIdentity;
	matIdentity = CreditMatrix(matIdentity);
	//ワールド行列に単位行列を代入
	worldTransform_.matWorld_ = matIdentity;
	//スケーリング行列の生成
	Matrix4 matScale = CreateMatScale(worldTransform_.scale_);
	//回転行列の生成
	Matrix4 matRot = CreateMatRotation(worldTransform_.rotation_);
	//平行移動行列の生成
	Matrix4 matTrans = CreateMatTranslation(worldTransform_.translation_);
	//スケーリング・回転・平行移動を合成した行列を計算してmatWorldに代入
	worldTransform_.matWorld_ *= matScale;
	worldTransform_.matWorld_ *= matRot;
	worldTransform_.matWorld_ *= matTrans;
	//wordlTransform_のワールド行列を転送
	worldTransform_.TransferMatrix();
}

void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	//ヌルポインタチェック
	assert(model);
	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void Enemy::Update() {
	//異動処理
	//座標に速度を加算して移動する
	worldTransform_.translation_ += velocity_;
	//座標をもとに行列の更新を行う
	//行列の更新、転送
	MatrixUpdate(worldTransform_);
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	//ワールドトランスフォーム、ビュープロジェクション、テクスチャハンドルを渡して3Dモデルを描画する
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}