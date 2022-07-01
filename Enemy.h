#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include "EnemyBullet.h"
#include <memory>
#include <list>
/// <summary>
/// �G
/// </summary>
class Enemy {
	//�����o�֐�
public:

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	/// <param name="velocity"></param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// ���W�A���ɕϊ�����֐�
	/// </summary>
	float ToRadian(float d);

	/// <summary>
	/// �x���ɕϊ�����֐�
	/// </summary>
	float ToDegree(float r);

	//clamp�֐�
	/// <summary>
	/// �ŏ��l
	/// </summary>
	float MinNum(float num, float num2);

	/// <summary>
	/// �ő�l
	/// </summary>
	float MaxNum(float num, float num2);

	/// <summary>
	/// �P�ʍs������֐�
	/// </summary>
	Matrix4 CreditMatrix(Matrix4 matIdentity);

	/// <summary>
	/// �X�P�[�����O�s������֐�
	/// </summary>
	Matrix4 CreateMatScale(Vector3 scale);

	/// <summary>
	/// ��]�s������֐�
	/// </summary>
	Matrix4 CreateMatRotation(Vector3 rotation);

	/// <summary>
	/// ���s�ړ��s������֐�
	/// </summary>
	Matrix4 CreateMatTranslation(Vector3 translation);

	/// <summary>
	/// �s����X�V����֐�
	/// </summary>
	/// <param name="worldTransform_"></param>
	void MatrixUpdate(WorldTransform& worldtransform);

	/// <summary>
	/// �x�N�g���ƍs��̊|���Z
	/// </summary>
	Vector3 VectorCrossMatrix(Vector3 velocity, WorldTransform& worldTransform);

	/// <summary>
	/// �U��
	/// </summary>
	void Fire();

	//�����o�ϐ�
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f���̃|�C���^
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���x
	Vector3 velocity_;
	//�G�e
	std::list < std::unique_ptr<EnemyBullet>> enemyBullets_;
	//���˃^�C�}�[
	int32_t kEnemyBulletTimer = 0;
	
public:
	//���ˎ���
	static const int kLifeInterval = 60;
	//�ڋ߃t�F�[�Y������
	void accessPhaseInitializing();
};