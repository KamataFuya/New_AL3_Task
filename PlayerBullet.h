#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

/// <summary>
/// ���L�����̒e
/// </summary>
class PlayerBullet {
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model_"></param>
	/// <param name="position"></param>
	void Initialize(Model* model_, const Vector3& position, const Vector3& velocity);

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
	/// isDead_�̃Q�b�^�[�֐�
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f���̃|�C���^(�؂�Ă�����)
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���x
	Vector3 velocity_;
	//����
	static const int32_t kLifeTime = 60 * 5;
	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;
};