#include "Enemy.h"

float Enemy::ToRadian(float d) {
	const float pi = 3.14f;
	d = d * (pi / 180);
	return d;
}

float Enemy::ToDegree(float r) {
	const float pi = 3.14f;
	r = r * 180.0f / pi;
	return r;
}

float Enemy::MinNum(float num, float num2) {
	if (num < num2) {
		return num;
	}
	return num2;
}

float Enemy::MaxNum(float num, float num2) {
	if (num > num2) {
		return num;
	}
	return num2;
}

Matrix4 Enemy::CreditMatrix(Matrix4 matIdentity) {
	//�P�ʍs��̐ݒ�
	matIdentity.m[0][0] = 1.0f;
	matIdentity.m[1][1] = 1.0f;
	matIdentity.m[2][2] = 1.0f;
	matIdentity.m[3][3] = 1.0f;
	//�ݒ肵���s���Ԃ�
	return matIdentity;
}

Matrix4 Enemy::CreateMatScale(Vector3 scale) {
	Matrix4 matScale;
	//�X�P�[�����O�s��̐ݒ�
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1.0f;
	//�ݒ肵���s���Ԃ�
	return matScale;
}

Matrix4 Enemy::CreateMatRotation(Vector3 rotation) {
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

Matrix4 Enemy::CreateMatTranslation(Vector3 translation) {
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	//���s�ړ��s��̐ݒ�
	matTrans.m[3][0] = translation.x;
	matTrans.m[3][1] = translation.y;
	matTrans.m[3][2] = translation.z;
	//�ݒ肵���s���Ԃ�
	return matTrans;
}

void Enemy::MatrixUpdate(WorldTransform& worldtransform) {
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

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;
	//�t�@�C�������w�肵�ăe�N�X�`����ǂݍ���
	textureHandle_ = TextureManager::Load("kuppa.png");
	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	//�������W�̐ݒ�
	worldTransform_.translation_ = { 0.0f,2.0f,20.0f };
}

void Enemy::Update() {
	//�ړ�����
	// �G�̑��x
	const float kEnemySpeed = -0.3f;
	Vector3 velocity(0, 0, kEnemySpeed);
	//���W�ɑ��x�����Z���Ĉړ�����
	worldTransform_.translation_ += velocity;
	//���W�����Ƃɍs��̍X�V���s��
	//�s��̍X�V�A�]��
	MatrixUpdate(worldTransform_);
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	//���[���h�g�����X�t�H�[���A�r���[�v���W�F�N�V�����A�e�N�X�`���n���h����n����3D���f����`�悷��
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}