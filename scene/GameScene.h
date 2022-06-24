#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Player.h"
#include "Enemy.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//3Dモデル
	Model* model_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//カメラ上方向の角度
	float viewAngle = 0.0f;
	//自キャラ
	Player* player_ = nullptr;
	//敵
	Enemy* enemy_ = nullptr;

public:
	//π
	const float pi = 3.1415f;
	//パーツID
	enum PartId {
		kRoot, //大元
		kSpine, //脊椎
		kChest, //胸
		kHead, //頭
		kArmL, //左腕
		kArmR, //右腕
		kHip, //尻
		kLegL, //左足
		kLegR, //右足

		kNumPartId
	};
	//回転
	const float kChestRotSpeed = 0.05f;//胸
	const float kHipRotSpeed = 0.05f;//尻

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
