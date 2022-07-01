#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
/// <summary>
/// 敵
/// </summary>
class Enemy {
public://メンバ関数

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

	//行動フェーズ
	enum class Phase {
		Initial, //初期フェーズ
		Approach, //接近する
		Leave, //離脱する

		kNumPhase
	};

private://メンバ変数
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデルのポインタ
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_;
	//フェーズ
	Phase phase_ = Phase::Initial;
};