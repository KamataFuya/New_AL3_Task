#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model_"></param>
	/// <param name="position"></param>
	void Initialize(Model* model_, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// ラジアンに変換する関数
	/// </summary>
	float ToRadian(float d);

	/// <summary>
	/// 度数に変換する関数
	/// </summary>
	float ToDegree(float r);

	//clamp関数
	/// <summary>
	/// 最小値
	/// </summary>
	float MinNum(float num, float num2);

	/// <summary>
	/// 最大値
	/// </summary>
	float MaxNum(float num, float num2);

	/// <summary>
	/// 単位行列を作る関数
	/// </summary>
	Matrix4 CreditMatrix(Matrix4 matIdentity);

	/// <summary>
	/// スケーリング行列を作る関数
	/// </summary>
	Matrix4 CreateMatScale(Vector3 scale);

	/// <summary>
	/// 回転行列を作る関数
	/// </summary>
	Matrix4 CreateMatRotation(Vector3 rotation);

	/// <summary>
	/// 平行移動行列を作る関数
	/// </summary>
	Matrix4 CreateMatTranslation(Vector3 translation);

	/// <summary>
	/// 行列を更新する関数
	/// </summary>
	/// <param name="worldTransform_"></param>
	void MatrixUpdate(WorldTransform& worldtransform);

	/// <summary>
	/// isDead_のゲッター関数
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデルのポインタ(借りてくるやつ)
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_;
	//寿命
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};