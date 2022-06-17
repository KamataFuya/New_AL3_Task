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
/// ���L����
/// </summary>
class Player {
public://�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name = "model">���f��</param>
	/// <param name = "textureHandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name = "viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
	void Draw(ViewProjection& viewProjection_);

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

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
	void MatrixUpdate(WorldTransform& worldTransform_);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���͏������邽��
	Input* input_ = nullptr;
	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;
	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
};