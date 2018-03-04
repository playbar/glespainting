
#ifndef _MAT4_H_
#define _MAT4_H_ 1

#include "vec3.h"
#include "vec4.h"

class Mat4  //column-major memory
{
  public:
    // Stores the columns of this 4x4 matrix.
    union {
      
      double m[16];
     
      struct {
        double m00, m10, m20, m30;
        double m01, m11, m21, m31;
        double m02, m12, m22, m32;
        double m03, m13, m23, m33;
      };
    };

    Mat4();

    Mat4(double m11, double m12, double m13, double m14,
        double m21, double m22, double m23, double m24,
        double m31, double m32, double m33, double m34, 
        double m41, double m42, double m43, double m44);

    // Creates a matrix initialized to the specified column-major array.
    Mat4(const double* mat);

    Mat4(const Mat4& copy);
    ~Mat4();

    void set(double m11, double m12, double m13, double m14, double m21, double m22, double m23, double m24,
        double m31, double m32, double m33, double m34, double m41, double m42, double m43, double m44);

    /**
     * Sets the values of this matrix to those in the specified column-major array.
     *
     * @param mat An array containing 16 elements in column-major format.
     */
    void set(const double* mat);

    /**
     * Sets the values of this matrix to those of the specified matrix.
     *
     * @param mat The source matrix.
     */
    void set(const Mat4& mat);

    /**
     * Sets this matrix to the identity matrix.
     */
    void setIdentity();

    /**
     * Inverts this matrix.
     *
     * @return true if the matrix can be inverted, false otherwise.
     */
    bool inverse();

    /**
     * Get the inversed matrix.
     */
    Mat4 getInversed() const;

    /**
     * Transposes this matrix.
     */
    void transpose();

    /**
     * Get the Transposed matrix.
     */
    Mat4 getTransposed() const;

    /**
     * Determines if this matrix is equal to the identity matrix.
     *
     * @return true if the matrix is an identity matrix, false otherwise.
     */
    bool isIdentity() const;

    static void createLookAt(const Vec3& eyePosition, const Vec3& targetPosition, const Vec3& up, Mat4* dst);
    static void createLookAt(double eyePositionX, double eyePositionY, double eyePositionZ,
        double targetCenterX, double targetCenterY, double targetCenterZ,
        double upX, double upY, double upZ, Mat4* dst);

  
    /**
     * Creates an orthographic projection matrix.
     *
     * @param width The width of the view.
     * @param height The height of the view.
     * @param zNearPlane The minimum z-value of the view volume.
     * @param zFarPlane The maximum z-value of the view volume.
     * @param dst A matrix to store the result in.
     */
    static void createOrthographic(double width, double height, double zNearPlane, double zFarPlane, Mat4* dst);

    static void createOrthographicOffCenter(double left, double right, double bottom, double top,
        double zNearPlane, double zFarPlane, Mat4* dst);
    
    static void createViewport(double x, double y, double width, double height, Mat4* dst);
    
    /**
     * Creates a scale matrix.
     *
     * @param xScale The amount to scale along the x-axis.
     * @param yScale The amount to scale along the y-axis.
     * @param zScale The amount to scale along the z-axis.
     * @param dst A matrix to store the result in.
     */
    static void createScale(double xScale, double yScale, double zScale, Mat4* dst);

    /**
     * Creates a translation matrix.
     *
     * @param xTranslation The translation on the x-axis.
     * @param yTranslation The translation on the y-axis.
     * @param zTranslation The translation on the z-axis.
     * @param dst A matrix to store the result in.
     */
    static void createTranslation(double xTranslation, double yTranslation, double zTranslation, Mat4* dst);


    /**
     * Multiplies the components of this matrix by the specified scalar.
     *
     * @param scalar The scalar value.
     */
    void multiply(double scalar);

    /**
     * Multiplies the components of this matrix by a scalar and stores the result in dst.
     *
     * @param scalar The scalar value.
     * @param dst A matrix to store the result in.
     */
    void multiply(double scalar, Mat4* dst) const;

    /**
     * Multiplies the components of the specified matrix by a scalar and stores the result in dst.
     *
     * @param mat The matrix.
     * @param scalar The scalar value.
     * @param dst A matrix to store the result in.
     */
    static void multiply(const Mat4& mat, double scalar, Mat4* dst);

    /**
     * Multiplies this matrix by the specified one.
     *
     * @param mat The matrix to multiply.
     */
    void multiply(const Mat4& mat);

    /**
     * Multiplies m1 by m2 and stores the result in dst.
     *
     * @param m1 The first matrix to multiply.
     * @param m2 The second matrix to multiply.
     * @param dst A matrix to store the result in.
     */
    static void multiply(const Mat4& m1, const Mat4& m2, Mat4* dst);

    /**
     * Calculates the matrix product of this matrix with the given matrix.
     * 
     * Note: this does not modify this matrix.
     * 
     * @param mat The matrix to multiply by.
     * @return The matrix product.
     */
    Mat4 operator*(const Mat4& mat) const;

    Vec4 operator*(const Vec4& a) const;

    /**
     * Calculates the negation of this matrix.
     * 
     * Note: this does not modify this matrix.
     * 
     * @return The negation of this matrix.
     */
    inline Mat4 operator-() const;

    inline Mat4 operator*(double k) const;

    inline double operator[](int k) const; 
};

#endif // _MAT4_H_
