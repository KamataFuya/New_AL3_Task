#include "PlayerBullet.h"

float PlayerBullet::ToRadian(float d) {
	const float pi = 3.14f;
	d = d * (pi / 180);
	return d;
}

float PlayerBullet::ToDegree(float r) {
	const float pi = 3.14f;
	r = r * 180.0f / pi;
	return r;
}

float PlayerBullet::MinNum(float num, float num2) {
	if (num < num2) {
		return num;
	}
	return num2;
}

float PlayerBullet::MaxNum(float num, float num2) {
	if (num > num2) {
		return num;
	}
	return num2;
}

Matrix4 PlayerBullet::CreditMatrix(Matrix4 matIdentity) {
	//�P�ʍs��̐ݒ�
	matIdentity.m[0][0] = 1.0f;
	matIdentity.m[1][1] = 1.0f;
	matIdentity.m[2][2] = 1.0f;
	matIdentity.m[3][3] = 1.0f;
	//�ݒ肵���s���Ԃ�
	return matIdentity;
}

Matrix4 PlayerBullet::CreateMatScale(Vector3 scale) {
	Matrix4 matScale;
	//�X�P�[�����O�s��̐ݒ�
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1.0f;
	//�ݒ肵���s���Ԃ�
	return matScale;
}

Matrix4 PlayerBullet::CreateMatRotation(Vector3 rotation) {
	Matrix4 matIdentity;
	//��]�s��̐錾
	Matrix4 matRot = CreditMatrix(matIdentity);
	//�e���p��]�s���錾
	Matrix4 matRotX = CreditMatrix(matIdentity),
		matRotY = CreditMatrix(matIdentity),
		matRotZ = CreditMatrix(matIdentity);
	//��]�s��̐ݒ�
	//Z����]�s��̊e�v�f��ݒ肷��
	matRotZ.m[0][0] = cos(rotation.z);
	matRotZ.m[0][1] = sin(rotation.z);
	matRotZ.m[1][0] = -sin(rotation.z);
	matRotZ.m[1][1] = cos(rotation.z);
	//X����]�s��̊e�v�f��ݒ肷��
	matRotX.m[1][1] = cos(rotation.x);
	matRotX.m[1][2] = sin(rotation.x);
	matRotX.m[2][1] = -sin(rotation.x);
	matRotX.m[2][2] = cos(rotation.x);
	//Y����]�s��̊e�v�f��ݒ肷��
	matRotY.m[0][0] = cos(rotation.y);
	matRotY.m[0][2] = -sin(rotation.y);
	matRotY.m[2][0] = sin(rotation.y);
	matRotY.m[2][2] = cos(rotation.y);
	//�e���̉�]�s�������
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;
	//�ݒ肵���s���Ԃ�
	return matRot;
}

Matrix4 PlayerBullet::CreateMatTranslation(Vector3 translation) {
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	//���s�ړ��s��̐ݒ�
	matTrans.m[3][0] = translation.x;
	matTrans.m[3][1] = translation.y;
	matTrans.m[3][2] = translation.z;
	//�ݒ肵���s���Ԃ�
	return matTrans;
}

void PlayerBullet::MatrixUpdate(WorldTransform& worldtransform) {
	//�s��X�V
	//�P�ʍs��̐���
	Matrix4 matIdentity;
	matIdentity = CreditMatrix(matIdentity);
	//���[���h�s��ɒP�ʍs�����
	worldTransform_.matWorld_ = matIdentity;
	//�X�P�[�����O�s��̐���
	Matrix4 matScale = CreateMatScale(worldTransform_.scale_);
	//��]�s��̐���
	Matrix4 matRot = CreateMatRotation(worldTransform_.rotation_);
	//���s�ړ��s��̐���
	Matrix4 matTrans = CreateMatTranslation(worldTransform_.translation_);
	//�X�P�[�����O�E��]�E���s�ړ������������s����v�Z����matWorld�ɑ��
	worldTransform_.matWorld_ *= matScale;
	worldTransform_.matWorld_ *= matRot;
	worldTransform_.matWorld_ *= matTrans;
	//wordlTransform_�̃��[���h�s���]��
	worldTransform_.TransferMatrix();
}

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	//�k���|�C���^�`�F�b�N
	assert(model);
	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");
	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;
	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;
}

void PlayerBullet::Update() {
	//���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
	worldTransform_.translation_ += velocity_;
	//�s��̍X�V�A�]��
	MatrixUpdate(worldTransform_);
	//���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}