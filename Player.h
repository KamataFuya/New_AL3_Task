#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include "Input.h"
#include "DebugText.h"
#include "PlayerBullet.h"
#include "memory"
#include "list"
/// <summary>
/// 自キャラ
/// </summary>
class Player {
public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name = "model">モデル</param>
	/// <param name = "textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name = "viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& viewProjection_);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

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
	void MatrixUpdate(WorldTransform& worldTransform_);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//入力処理するため
	Input* input_ = nullptr;
	//デバッグテキスト
	DebugText* debugText_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
};