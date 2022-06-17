#include "Player.h"

float Player::ToRadian(float d) {
	const float pi = 3.14f;
	d = d * (pi / 180);
	return d;
}

float Player::ToDegree(float r) {
	const float pi = 3.14f;
	r = r * 180.0f / pi;
	return r;
}

float Player::MinNum(float num, float num2) {
	if (num < num2) {
		return num;
	}
	return num2;
}

float Player::MaxNum(float num, float num2) {
	if (num > num2) {
		return num;
	}
	return num2;
}

Matrix4 Player::CreditMatrix(Matrix4 matIdentity) {
	//�P�ʍs��̐ݒ�
	matIdentity.m[0][0] = 1.0f;
	matIdentity.m[1][1] = 1.0f;
	matIdentity.m[2][2] = 1.0f;
	matIdentity.m[3][3] = 1.0f;
	//�ݒ肵���s���Ԃ�
	return matIdentity;
}

Matrix4 Player::CreateMatScale(Vector3 scale) {
	Matrix4 matScale;
	//�X�P�[�����O�s��̐ݒ�
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1.0f;
	//�ݒ肵���s���Ԃ�
	return matScale;
}

Matrix4 Player::CreateMatRotation(Vector3 rotation) {
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

Matrix4 Player::CreateMatTranslation(Vector3 translation) {
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	//���s�ړ��s��̐ݒ�
	matTrans.m[3][0] = translation.x;
	matTrans.m[3][1] = translation.y;
	matTrans.m[3][2] = translation.z;
	//�ݒ肵���s���Ԃ�
	return matTrans;
}

void Player::MatrixUpdate(WorldTransform& worldTransform_) {
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
	//worldTransform_��parent_�̃��[���h�s����|���Z�������
	if (worldTransform_.parent_) {//�k���`�F�b�N
		worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	}
	//wordlTransform_�̃��[���h�s���]��
	worldTransform_.TransferMatrix();
}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;
	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Player::Update() {
	//�L�����N�^�[���񏈗�
	const float kRotSpeed = 0.05f;
	//�����������ňړ��x�N�g����ύX
	if (input_->PushKey(DIK_U)) {
		//Y������̊p�x������
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_I)) {
		//Y������̊p�x�𑝉�
		worldTransform_.rotation_.y += kRotSpeed;
	}
	//�L�����N�^�[�ړ�����
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };
	//�ړ��x�N�g����ύX���鏈��
	const float kCharactorSpeed = 0.2f;//�L�����N�^�[�̈ړ����x
	if (input_->PushKey(DIK_LEFT)) {
		move = { -kCharactorSpeed,0,0 };
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move = { kCharactorSpeed,0,0 };
	}
	if (input_->PushKey(DIK_UP)) {
		move = { 0,kCharactorSpeed,0 };
	}
	else if (input_->PushKey(DIK_DOWN)) {
		move = { 0,-kCharactorSpeed,0 };
	}
	//���W�ړ�(�x�N�g���̉��Z)
	worldTransform_.translation_ += move;
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
	//�L�����N�^�[�̍��W����ʕ\�����鏈��
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"pos : (%f,%f,%f)", worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

	//�L�[�{�[�h���͂ɂ��ړ�����
	{
		//�ړ����E���W
		const float kMoveLimitX = 35.5f;
		const float kMoveLimitY = 19.5f;
		//�͈͂𒴂��Ȃ�����
		worldTransform_.translation_.x = MaxNum(worldTransform_.translation_.x, -kMoveLimitX);
		worldTransform_.translation_.x = MinNum(worldTransform_.translation_.x, +kMoveLimitX);
		worldTransform_.translation_.y = MaxNum(worldTransform_.translation_.y, -kMoveLimitY);
		worldTransform_.translation_.y = MinNum(worldTransform_.translation_.y, +kMoveLimitY);
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
	//�L�����N�^�[�U������
	Attack();
	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}
}

void Player::Draw(ViewProjection& viewProjection_) {
	//���f����`��
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		//���L�����̍��W���R�s�[
		Vector3 position = worldTransform_.translation_;
		//�e�𐶐����A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, position);
		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}