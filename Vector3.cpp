#include "Vector3.h"
#include <cmath>

//Vector2 クラスに属さない関数群
// 二項演算子
//ベクトルの足し算ができるようにする
const Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp += v2;
}

//ベクトルの引き算ができるようにする
const Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp -= v2;
}

//ベクトルのs倍を計算できるようにする
const Vector3 operator*(const Vector3& v, float s) {
	Vector3 temp(v);
	return temp *= s;
}

//ベクトルのs倍を計算できるようにする(上とはオペランドの順序が逆)
const Vector3 operator*(float s, const Vector3& v) {
	return v * s;
}

//ベクトルの1/s倍を計算できるようにする
const Vector3 operator/(const Vector3& v, float s) {
	Vector3 temp(v);
	return temp /= s;
}