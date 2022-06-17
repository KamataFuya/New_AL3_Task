#include "Player.h"

float Player::ToRadian(float d) {
	const float pi = 3.14f;
	d = d * (pi / 180);
	return d;
}

float Player::ToDegree(float r) {
	const float pi = 3.14f;
	r = r * 180.0f / pi;
	return r;
}

float Player::MinNum(float num, float num2) {
	if (num < num2) {
		return num;
	}
	return num2;
}

float Player::MaxNum(float num, float num2) {
	if (num > num2) {
		return num;
	}
	return num2;
}

Matrix4 Player::CreditMatrix(Matrix4 matIdentity) {
	//単位行列の設定
	matIdentity.m[0][0] = 1.0f;
	matIdentity.m[1][1] = 1.0f;
	matIdentity.m[2][2] = 1.0f;
	matIdentity.m[3][3] = 1.0f;
	//設定した行列を返す
	return matIdentity;
}

Matrix4 Player::CreateMatScale(Vector3 scale) {
	Matrix4 matScale;
	//スケーリング行列の設定
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1.0f;
	//設定した行列を返す
	return matScale;
}

Matrix4 Player::CreateMatRotation(Vector3 rotation) {
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

Matrix4 Player::CreateMatTranslation(Vector3 translation) {
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	//平行移動行列の設定
	matTrans.m[3][0] = translation.x;
	matTrans.m[3][1] = translation.y;
	matTrans.m[3][2] = translation.z;
	//設定した行列を返す
	return matTrans;
}

void Player::MatrixUpdate(WorldTransform& worldTransform_) {
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
	//worldTransform_にparent_のワールド行列を掛け算代入する
	if (worldTransform_.parent_) {//ヌルチェック
		worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	}
	//wordlTransform_のワールド行列を転送
	worldTransform_.TransferMatrix();
}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);
	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;
	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	//ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update() {
	//キャラクター旋回処理
	const float kRotSpeed = 0.05f;
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_U)) {
		//Y軸周りの角度を減少
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_I)) {
		//Y軸周りの角度を増加
		worldTransform_.rotation_.y += kRotSpeed;
	}
	//キャラクター移動処理
	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };
	//移動ベクトルを変更する処理
	const float kCharactorSpeed = 0.2f;//キャラクターの移動速度
	if (input_->PushKey(DIK_LEFT)) {
		move = { -kCharactorSpeed,0,0 };
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move = { kCharactorSpeed,0,0 };
	}
	if (input_->PushKey(DIK_UP)) {
		move = { 0,kCharactorSpeed,0 };
	}
	else if (input_->PushKey(DIK_DOWN)) {
		move = { 0,-kCharactorSpeed,0 };
	}
	//座標移動(ベクトルの加算)
	worldTransform_.translation_ += move;
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
	//キャラクターの座標を画面表示する処理
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"pos : (%f,%f,%f)", worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

	//キーボード入力による移動処理
	{
		//移動限界座標
		const float kMoveLimitX = 35.5f;
		const float kMoveLimitY = 19.5f;
		//範囲を超えない処理
		worldTransform_.translation_.x = MaxNum(worldTransform_.translation_.x, -kMoveLimitX);
		worldTransform_.translation_.x = MinNum(worldTransform_.translation_.x, +kMoveLimitX);
		worldTransform_.translation_.y = MaxNum(worldTransform_.translation_.y, -kMoveLimitY);
		worldTransform_.translation_.y = MinNum(worldTransform_.translation_.y, +kMoveLimitY);
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
	//キャラクター攻撃処理
	Attack();
	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}
}

void Player::Draw(ViewProjection& viewProjection_) {
	//モデルを描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		//自キャラの座標をコピー
		Vector3 position = worldTransform_.translation_;
		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, position);
		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
}