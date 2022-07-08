#include "Vector3.h"
#include <cmath>

//Vector2 �N���X�ɑ����Ȃ��֐��Q
// �񍀉��Z�q
//�x�N�g���̑����Z���ł���悤�ɂ���
const Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp += v2;
}

//�x�N�g���̈����Z���ł���悤�ɂ���
const Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp -= v2;
}

//�x�N�g����s�{���v�Z�ł���悤�ɂ���
const Vector3 operator*(const Vector3& v, float s) {
	Vector3 temp(v);
	return temp *= s;
}

//�x�N�g����s�{���v�Z�ł���悤�ɂ���(��Ƃ̓I�y�����h�̏������t)
const Vector3 operator*(float s, const Vector3& v) {
	return v * s;
}

//�x�N�g����1/s�{���v�Z�ł���悤�ɂ���
const Vector3 operator/(const Vector3& v, float s) {
	Vector3 temp(v);
	return temp /= s;
}