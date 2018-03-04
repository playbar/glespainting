
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mat4.h"

#define MATH_TOLERANCE 2e-37f
#define MATH_EPSILON 0.000001f

#define MATRIX_SIZE 16*sizeof(double)

static const double s_identity[16] = {1.0,0.0,0.0,0.0, 0.0,1.0,0.0,0.0, 0.0,0.0,1.0,0.0, 0.0,0.0,0.0,1.0};

Mat4::Mat4() {
  memcpy(m,s_identity,16*sizeof(double));
}

Mat4::Mat4(double m11, double m12, double m13, double m14, 
           double m21, double m22, double m23, double m24,
           double m31, double m32, double m33, double m34, 
           double m41, double m42, double m43, double m44) {
  set(m11,m12,m13,m14, m21,m22,m23,m24, m31,m32,m33,m34, m41,m42,m43,m44);
}

Mat4::Mat4(const double* mat) { set(mat); }

Mat4::Mat4(const Mat4& copy) { memcpy(m, copy.m, 16*sizeof(double)); }

Mat4::~Mat4() { } 



void Mat4::set(double a00, double a01, double a02, double a03, 
    double a10, double a11, double a12, double a13,
    double a20, double a21, double a22, double a23,
    double a30, double a31, double a32, double a33) {
  m00 = a00; m01 = a01; m02 = a02; m03 = a03;
  m10 = a10; m11 = a11; m12 = a12; m13 = a13; 
  m20 = a20; m21 = a21; m22 = a22; m23 = a23;
  m30 = a30; m31 = a31; m32 = a32; m33 = a33;
}
void Mat4::set(const double* mat) {
  memcpy(this->m, mat, 16*sizeof(double));
}
void Mat4::set(const Mat4& mat) {
  memcpy(this->m, mat.m, MATRIX_SIZE);
}

void Mat4::setIdentity() {
  memcpy(m,s_identity,MATRIX_SIZE);
}

void Mat4::transpose() {
  double tmp;

  tmp = m01; m01 = m10; m10 = tmp; 
  tmp = m02; m02 = m20; m20 = tmp; 
  tmp = m03; m03 = m30; m30 = tmp; 

  tmp = m12; m12 = m21; m21 = tmp; 
  tmp = m13; m13 = m31; m31 = tmp; 

  tmp = m23; m23 = m32; m32 = tmp; 
}
Mat4 Mat4::getTransposed() const {
  Mat4 mat(*this);
  mat.transpose();
  return mat;
}

bool Mat4::inverse() {
  double a0 = m[0] * m[5] - m[1] * m[4];
  double a1 = m[0] * m[6] - m[2] * m[4];
  double a2 = m[0] * m[7] - m[3] * m[4];
  double a3 = m[1] * m[6] - m[2] * m[5];
  double a4 = m[1] * m[7] - m[3] * m[5];
  double a5 = m[2] * m[7] - m[3] * m[6];
  double b0 = m[8] * m[13] - m[9] * m[12];
  double b1 = m[8] * m[14] - m[10] * m[12];
  double b2 = m[8] * m[15] - m[11] * m[12];
  double b3 = m[9] * m[14] - m[10] * m[13];
  double b4 = m[9] * m[15] - m[11] * m[13];
  double b5 = m[10] * m[15] - m[11] * m[14];

  // Calculate the determinant.
  double det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

  // Close to zero, can't invert.
  if (fabs(det) <= MATH_TOLERANCE)
    return false;

  // Support the case where m == dst.
  Mat4 inverse;
  inverse.m[0]  = m[5] * b5 - m[6] * b4 + m[7] * b3;
  inverse.m[1]  = -m[1] * b5 + m[2] * b4 - m[3] * b3;
  inverse.m[2]  = m[13] * a5 - m[14] * a4 + m[15] * a3;
  inverse.m[3]  = -m[9] * a5 + m[10] * a4 - m[11] * a3;

  inverse.m[4]  = -m[4] * b5 + m[6] * b2 - m[7] * b1;
  inverse.m[5]  = m[0] * b5 - m[2] * b2 + m[3] * b1;
  inverse.m[6]  = -m[12] * a5 + m[14] * a2 - m[15] * a1;
  inverse.m[7]  = m[8] * a5 - m[10] * a2 + m[11] * a1;

  inverse.m[8]  = m[4] * b4 - m[5] * b2 + m[7] * b0;
  inverse.m[9]  = -m[0] * b4 + m[1] * b2 - m[3] * b0;
  inverse.m[10] = m[12] * a4 - m[13] * a2 + m[15] * a0;
  inverse.m[11] = -m[8] * a4 + m[9] * a2 - m[11] * a0;

  inverse.m[12] = -m[4] * b3 + m[5] * b1 - m[6] * b0;
  inverse.m[13] = m[0] * b3 - m[1] * b1 + m[2] * b0;
  inverse.m[14] = -m[12] * a3 + m[13] * a1 - m[14] * a0;
  inverse.m[15] = m[8] * a3 - m[9] * a1 + m[10] * a0;

  multiply(inverse, 1.0f / det, this);

  return true;
}
Mat4 Mat4::getInversed() const {
  Mat4 mat(*this);
  mat.inverse();
  return mat;
}

bool Mat4::isIdentity() const {
  return (memcmp(m, s_identity, MATRIX_SIZE) == 0);
}

void Mat4::createLookAt(const Vec3& eyePosition, const Vec3& targetPosition, const Vec3& up, Mat4* dst) {
  createLookAt(eyePosition.x, eyePosition.y, eyePosition.z,
      targetPosition.x, targetPosition.y, targetPosition.z,
      up.x, up.y, up.z, dst);
}
void Mat4::createLookAt(double eyePositionX, double eyePositionY, double eyePositionZ,
                        double targetPositionX, double targetPositionY, double targetPositionZ,
                        double upX, double upY, double upZ, Mat4* dst) {
  Vec3 eye(eyePositionX, eyePositionY, eyePositionZ);
  Vec3 target(targetPositionX, targetPositionY, targetPositionZ);

  Vec3 up(upX, upY, upZ);
  up.normalize();

  Vec3 zaxis = eye - target;
  zaxis.normalize();

  Vec3 xaxis = up ^ zaxis;;
  xaxis.normalize();

  Vec3 yaxis = zaxis ^ xaxis;
  yaxis.normalize();

  dst->m[0] = xaxis.x;
  dst->m[1] = yaxis.x;
  dst->m[2] = zaxis.x;
  dst->m[3] = 0.0f;

  dst->m[4] = xaxis.y;
  dst->m[5] = yaxis.y;
  dst->m[6] = zaxis.y;
  dst->m[7] = 0.0f;

  dst->m[8] = xaxis.z;
  dst->m[9] = yaxis.z;
  dst->m[10] = zaxis.z;
  dst->m[11] = 0.0f;

  dst->m[12] = -(xaxis*eye);
  dst->m[13] = -(yaxis*eye);
  dst->m[14] = -(zaxis*eye);
  dst->m[15] = 1.0f;
}

void Mat4::createViewport(double x, double y, double w, double h, Mat4* dst){
  dst->setIdentity();
  dst->m[0] = w*0.5;    dst->m[5] = h*0.5;     dst->m[10] = 0.5;
  dst->m[12] = x+w*0.5; dst->m[13] = y+h*0.5;  dst->m[14] = 0.5;
}

void Mat4::createOrthographic(double width, double height, double zNearPlane, double zFarPlane, Mat4* dst) {
  double halfWidth = width / 2.0f;
  double halfHeight = height / 2.0f;
  createOrthographicOffCenter(-halfWidth, halfWidth, -halfHeight, halfHeight, zNearPlane, zFarPlane, dst);
}
void Mat4::createOrthographicOffCenter(double left, double right, double bottom, double top,
    double zNearPlane, double zFarPlane, Mat4* dst) {
   memset(dst->m, 0, MATRIX_SIZE);

  dst->m[0] = 2 / (right - left);
  dst->m[5] = 2 / (top - bottom);
  dst->m[10] = 2 / (zNearPlane - zFarPlane);

  dst->m[12] = (left + right) / (left - right);
  dst->m[13] = (top + bottom) / (bottom - top);
  dst->m[14] = (zNearPlane + zFarPlane) / (zNearPlane - zFarPlane);
  dst->m[15] = 1;
}
    
void Mat4::createScale(double xScale, double yScale, double zScale, Mat4* dst) {
  memcpy(dst->m,s_identity, MATRIX_SIZE);
  dst->m[0] = xScale;
  dst->m[5] = yScale;
  dst->m[10] = zScale;
}

void Mat4::createTranslation(double xTranslation, double yTranslation, double zTranslation, Mat4* dst) {
  dst->setIdentity();
  dst->m[12] = xTranslation;
  dst->m[13] = yTranslation;
  dst->m[14] = zTranslation;
}

void Mat4::multiply(double scalar) { multiply(scalar, this); }
void Mat4::multiply(double scalar, Mat4* dst) const { multiply(*this, scalar, dst); }
void Mat4::multiply(const Mat4& m, double scalar, Mat4* dst) {
  for(int i=0; i<16; i++)
    dst->m[i] = m[i] * scalar;
}
void Mat4::multiply(const Mat4& mat) {
  multiply(*this, mat, this);
}
void Mat4::multiply(const Mat4& m1, const Mat4& m2, Mat4* dst) {
  dst->m00 = m1.m00*m2.m00 + m1.m01*m2.m10 + m1.m02*m2.m20 + m1.m03*m2.m30;
  dst->m01 = m1.m00*m2.m01 + m1.m01*m2.m11 + m1.m02*m2.m21 + m1.m03*m2.m31;
  dst->m02 = m1.m00*m2.m02 + m1.m01*m2.m12 + m1.m02*m2.m22 + m1.m03*m2.m32;
  dst->m03 = m1.m00*m2.m03 + m1.m01*m2.m13 + m1.m02*m2.m23 + m1.m03*m2.m33;

  dst->m10 = m1.m10*m2.m00 + m1.m11*m2.m10 + m1.m12*m2.m20 + m1.m13*m2.m30;
  dst->m11 = m1.m10*m2.m01 + m1.m11*m2.m11 + m1.m12*m2.m21 + m1.m13*m2.m31;
  dst->m12 = m1.m10*m2.m02 + m1.m11*m2.m12 + m1.m12*m2.m22 + m1.m13*m2.m32;
  dst->m13 = m1.m10*m2.m03 + m1.m11*m2.m13 + m1.m12*m2.m23 + m1.m13*m2.m33;

  dst->m20 = m1.m20*m2.m00 + m1.m21*m2.m10 + m1.m22*m2.m20 + m1.m23*m2.m30;
  dst->m21 = m1.m20*m2.m01 + m1.m21*m2.m11 + m1.m22*m2.m21 + m1.m23*m2.m31;
  dst->m22 = m1.m20*m2.m02 + m1.m21*m2.m12 + m1.m22*m2.m22 + m1.m23*m2.m32;
  dst->m23 = m1.m20*m2.m03 + m1.m21*m2.m13 + m1.m22*m2.m23 + m1.m23*m2.m33;

  dst->m30 = m1.m30*m2.m00 + m1.m31*m2.m10 + m1.m32*m2.m20 + m1.m33*m2.m30;
  dst->m31 = m1.m30*m2.m01 + m1.m31*m2.m11 + m1.m32*m2.m21 + m1.m33*m2.m31;
  dst->m32 = m1.m30*m2.m02 + m1.m31*m2.m12 + m1.m32*m2.m22 + m1.m33*m2.m32;
  dst->m33 = m1.m30*m2.m03 + m1.m31*m2.m13 + m1.m32*m2.m23 + m1.m33*m2.m33;
}

Mat4 Mat4::operator*(const Mat4& b) const {
  Mat4 dst;
  Mat4::multiply(*this,b,&dst);
  return dst;
}
Vec4 Mat4::operator*(const Vec4& a) const {
  double ax = m00*a.x + m01*a.y + m02*a.z + m03*a.w;
  double ay = m10*a.x + m11*a.y + m12*a.z + m13*a.w;
  double az = m20*a.x + m21*a.y + m22*a.z + m23*a.w;
  double aw = m30*a.x + m31*a.y + m32*a.z + m33*a.w;
  return Vec4(ax,ay,az,aw);
}
Mat4 Mat4::operator-() const{
  Mat4 dst;
  for(int i=0; i<16; i++)
    dst.m[i] = -m[i];
  return dst;
}
Mat4 Mat4::operator*(double k) const{
  Mat4 dst;
  for(int i=0; i<16; i++)
    dst.m[i] = m[i]*k;
  return dst;
}
double Mat4::operator[](int k) const{
  return m[k];
}
