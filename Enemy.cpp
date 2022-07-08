#include "Enemy.h"
#include "PLayer.h"

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

Vector3 Enemy::VectorCrossMatrix(Vector3 velocity, WorldTransform& worldTransform) {
	//ベクトルと行列の各要素の掛け算
	velocity.x = velocity.x * worldTransform.matWorld_.m[0][0]
		+ velocity.y * worldTransform.matWorld_.m[1][0]
		+ velocity.z * worldTransform.matWorld_.m[2][0];
	velocity.y = velocity.x * worldTransform.matWorld_.m[0][1]
		+ velocity.y * worldTransform.matWorld_.m[1][1]
		+ velocity.z * worldTransform.matWorld_.m[2][1];
	velocity.z = velocity.x * worldTransform.matWorld_.m[0][2]
		+ velocity.y * worldTransform.matWorld_.m[1][2]
		+ velocity.z * worldTransform.matWorld_.m[2][2];
	//ベクトルを返す
	return velocity;
}

float Enemy::length(Vector3& a, Vector3& b) {
	return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y) + (b.z - a.z) * (b.z - a.z));
}

Vector3 Enemy::normalize(Vector3& a, Vector3& b) {
	return (b - a) / length(a,b);
}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);
	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;
	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("kuppa.png");
	//ワールド変換の初期化
	worldTransform_.Initialize();
	//初期座標の設定
	worldTransform_.translation_ = { 20.0f,2.0f,20.0f };
	//接近フェーズ初期化
	accessPhaseInitializing();

}

void Enemy::Update() {
	//デスフラグの立った弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& enemyBullet) {
		return enemyBullet->IsDead();
		});
	//移動処理
	// 敵の速度
	const float kEnemySpeed = -0.1f;
	Vector3 velocity(0, 0, kEnemySpeed);
	//座標に速度を加算して移動する
	worldTransform_.translation_ += velocity;
	//座標をもとに行列の更新を行う
	//行列の更新、転送
	MatrixUpdate(worldTransform_);
	//発射タイマーをカウントダウン
	kEnemyBulletTimer--;
	//指定時間に達した
	if (kEnemyBulletTimer <= 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		kEnemyBulletTimer = kLifeInterval;
	}
	//EnemyBullet更新
	for (std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets_) {
		enemyBullet->Update();
	}
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	//ワールドトランスフォーム、ビュープロジェクション、テクスチャハンドルを渡して3Dモデルを描画する
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//弾描画
	for (std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets_) {
		enemyBullet->Draw(viewProjection);
	}
}

void Enemy::Fire() {
	//NULLポインタチェック
	assert(player_);
	//弾の速度
	const float kBulletSpeed = 1.0f;
	//自キャラのワールド座標を取得する
	Vector3 playerPos = player_->GetWorldPosition();
	//敵キャラのワールド座標を取得する
	Vector3 enemyPos = GetWorldPosition();
	//敵キャラ->自キャラの差分ベクトルを求める
	Vector3 differencialVector = playerPos - enemyPos;
	//ベクトルの正規化
	if (length(enemyPos, playerPos) != 0) {
		differencialVector = normalize(enemyPos, playerPos);
	}
	//ベクトルの長さを速さに合わせる
	Vector3 velocity = differencialVector * kBulletSpeed;
	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newEnemyBullet = std::make_unique<EnemyBullet>();
	newEnemyBullet->Initialize(model_, worldTransform_.translation_, velocity);
	//弾を登録する
	enemyBullets_.push_back(std::move(newEnemyBullet));
}

void Enemy::accessPhaseInitializing() {
	//発射タイマーを初期化
	kEnemyBulletTimer = kLifeInterval;
}

Vector3 Enemy::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}