#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include "EnemyBullet.h"
#include <memory>
#include <list>
#include <cmath>
#include "Vector3.h"
//自機クラスの前方宣言
class Player;
/// <summary>
/// 敵
/// </summary>
class Enemy {
	//メンバ関数
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	/// <param name="velocity"></param>
	void Initialize(Model* model, uint32_t textureHandle);

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
	/// ベクトルと行列の掛け算
	/// </summary>
	Vector3 VectorCrossMatrix(Vector3 velocity, WorldTransform& worldTransform);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Fire();

	/// <summary>
	/// プレイヤーのセッター関数
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// Enemyのワールド座標のゲッター関数
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 長さを求める
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	float length(Vector3& a, Vector3& b);

	/// <summary>
	/// ベクトルの正規化
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	Vector3 normalize(Vector3& a, Vector3& b);

	/// <summary>
	/// 衝突を検知したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	/// <summary>
	/// 敵弾リストを取得
	/// </summary>
	/// <returns></returns>
	const std::list<std::unique_ptr<EnemyBullet>>& GetEnemyBullets() { return enemyBullets_; }

	//メンバ変数
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデルのポインタ
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_;
	//敵弾
	std::list < std::unique_ptr<EnemyBullet>> enemyBullets_;
	//発射タイマー
	int32_t kEnemyBulletTimer = 0;
	//自キャラ
	Player* player_ = nullptr;
public:
	//発射時間
	static const int kLifeInterval = 60;
	//接近フェーズ初期化
	void accessPhaseInitializing();
};