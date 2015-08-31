#pragma once

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS
#include <list>
#include "Math.h"
#include "glm.hpp"
#include "type_ptr.hpp"
#include "matrix_transform.hpp"
#include "matrix_transform.hpp"
// Vectors


class Vec3
	{
		

		public:
			float x, y, z;

			const Vec3& operator=(const glm::vec3& v)
			  {
				x = v.x;
				y = v.y;
				z = v.z;
				return (const Vec3&)*this;
			  }

			// Constructors
			Vec3();
			Vec3(glm::vec3 _Vector3) { x = _Vector3.x; y = _Vector3.y; z = _Vector3.z; }
			Vec3(const int _X, const int _Y, const int _Z) { x = (float)_X; y = (float)_Y; z = (float)_Z; }
			Vec3(const float _X, const float _Y, const float _Z) { x = _X; y = _Y; z = _Z; }
			Vec3(const double _X, const double _Y, const double _Z) { x = (float)_X; y = (float)_Y; z = (float)_Z; }
			inline Vec3(const class Vec4 &_Vec4);

			// Math functions
			inline float Length()
			{ 
				glm::vec3 Vector(x, y, z);
				float length = glm::length(Vector);
				return length;
			}
			inline Vec3 Normalize() 
			{
				glm::vec3 NormalizedVector;
				NormalizedVector.x = x;
				NormalizedVector.y = y;
				NormalizedVector.z = z;
				NormalizedVector = glm::normalize(NormalizedVector);

				x = NormalizedVector.x;
				y = NormalizedVector.y;
				z = NormalizedVector.z;

				Vec3 ReturnVector(NormalizedVector);
				return ReturnVector;
			}
			inline float Dot(const Vec3 &_b)
			{
				glm::vec3 a;
				a.x = x;
				a.y = y;
				a.z = z;

				glm::vec3 b;
				b.x = _b.x;
				b.y = _b.y;
				b.z = _b.z;
				float DotProduct = glm::dot(a, b); return DotProduct; 
			}
			inline Vec3 Cross(const Vec3 &_b) const
			{
				glm::vec3 a;
				a.x = x;
				a.y = y;
				a.z = z;

				glm::vec3 b;
				b.x = _b.x;
				b.y = _b.y;
				b.z = _b.z;
				Vec3 Out(glm::cross(a, b));
				return Out;
			}
	};

class Vec4
{
	public:
		float x, y, z, w;
		// Constructors
		Vec4();
		Vec4(glm::vec4 _Vector4) {  x = _Vector4.x; y = _Vector4.y; z = _Vector4.z; w = _Vector4.w; }
		Vec4(const int _X, const int _Y, const int _Z, const int _W) { x = (float)_X; y = (float)_Y; z = (float)_Z; w = (float) _W;}
		Vec4(const int _X, const int _Y, const int _Z) { x = (float)_X; y = (float)_Y; z = (float)_Z; w = 1.0f; }
		Vec4(const float _X, const float _Y, const float _Z) { x = _X; y = _Y; z = _Z; w = 1.0f; }
		Vec4(const float _X, const float _Y, const float _Z, const float _W) { x = _X; y = _Y; z = _Z; w = _W; }
		Vec4(const double _X, const double _Y, const double _Z, const double _W) { x = (float)_X; y = (float)_Y; z = (float)_Z; w = _W; }
		//inline Vec4(const Vec3 &_Vector3) 
		
		// Math functions
		inline float Length()
			{ 
				glm::vec4 Vector(x, y, z, w);
				float length = glm::length(Vector);
				return length;
			}
		//inline Vec4* Normalize() { glm::normalize(this); return this; }
		inline Vec4 Normalize()
			{
				glm::vec4 NormalizedVector;
				NormalizedVector.x = x;
				NormalizedVector.y = y;
				NormalizedVector.z = z;
				NormalizedVector.w = 1.0f;
				NormalizedVector = glm::normalize(NormalizedVector);

				x = NormalizedVector.x;
				y = NormalizedVector.y;
				z = NormalizedVector.z;
				w = 1.0f;

				Vec4 ReturnVector(NormalizedVector);
				return ReturnVector;
			}
		inline float Dot(const Vec4 &_b)
		{ 
			glm::vec4 a;
				a.x = x;
				a.y = y;
				a.z = z;
				a.w = w;

				glm::vec4 b;
				b.x = _b.x;
				b.y = _b.y;
				b.z = _b.z;
				b.w = _b.w;
			float DotProduct = glm::dot(a, b); return DotProduct; 
		}
		
};

typedef std::list<Vec3> Vec3List;
typedef std::list<Vec4> Vec4List;




// Quaternions here....


class Quaternion
{
public:
	glm::quat Quat;
public:

	// Constructors
	Quaternion(Vec3& _Vector, float _Radians)
	{
		Quat = glm::quat(_Radians, glm::vec3(_Vector.x, _Vector.y, _Vector.z));
	}
	Quaternion(glm::quat _Quaternion)
	{
		Quat = _Quaternion;
	}

	// Modifiers
	inline void Normalize() {	glm::normalize(Quat);	}
	inline void SLerp(const Quaternion &_Begin, const Quaternion &_End, float _Coefficient)
	{
		// Performs Spherical Linear Interpolation between Begin & End
		// NOTE: Set _Coefficient between 0.0f-1.0f
		Quat = glm::slerp(_Begin.Quat, _End.Quat, _Coefficient);
	}

	// Accessors
	inline void GetAxisAngle(Vec3 &_Axis, float &_Angle) 
	{
		glm::vec3 Axis(_Axis.x, _Axis.y, _Axis.z);
		Quat = glm::angleAxis(_Angle, Axis);
	}

	// Initializers
	void BuildRotYawPitchRoll
		( const float _YawRadians, const float _PitchRadians, const float _RollRadians)
	{
		//Quaternion QuatAroundX = Quaternion( Vec3(1.0,0.0,0.0), _RollRadians );
		//Quaternion QuatAroundY = Quaternion( Vec3(0.0,1.0,0.0), _YawRadians );
		//Quaternion QuatAroundZ = Quaternion( Vec3(0.0,0.0,1.0), _PitchRadians );
		//glm::quat FinalOrientation = QuatAroundX.Quat * QuatAroundY.Quat * QuatAroundZ.Quat;
		//Quat = FinalOrientation;
	}
	void BuildAxisAngle(const Vec3 &_Axis, const float _Radians)
	{
		
	}

	inline void Build(const class Mat4x4 &_Mat);
};

// Matrices


class Mat4x4 
{

public:
	
	double m[4][4];
	/*
	const Mat4x4& operator=(const glm::mat4x4& _Mat)
			  {
				const float *pSource = (const float*)glm::value_ptr(_Mat);
				int i, j;
				int k = 0;
				for(i = 0; i<4; i++)
					{
						for(j=0; j<4; j++)
							{
								m[i][j] = pSource[k];	
								++k;
							}
					}
				return (const Mat4x4&)*this;
			  }
			  */
	inline void Set(const glm::mat4x4& _Mat)
	{
		const float *pSource = (const float*)glm::value_ptr(_Mat);
				int i, j;
				int k = 0;
				for(i = 0; i<4; i++)
					{
						for(j=0; j<4; j++)
							{
								m[i][j] = pSource[k];	
								++k;
							}
					}
	}

private:
	/*
	inline float* MakeOneDimensionalMatrix()
	{
		int i, j, k;
		float OneDimensionalMatrix[16];
 
		for(i = 0; i<4; i++)
		{
			for(j=0; j<4; j++)
				{
					OneDimensionalMatrix[k] = m[i][j];	
					++k;
				}
		}
		return OneDimensionalMatrix;
	}*/

public:
	// Constructors
	Mat4x4(int _Id1, int _Id2, int _Id3, int _Id4,
		int _Id5, int _Id6, int _Id7, int _Id8,
		int _Id9, int _Id10, int _Id11, int _Id12,
		int _Id13, int _Id14, int _Id15, int _Id16)
	{
		m[0][0] = (double)_Id1;
		m[0][1] = (double)_Id2;
		m[0][2] = (double)_Id3;
		m[0][3] = (double)_Id4;
		m[1][0] = (double)_Id5;
		m[1][1] = (double)_Id6;
		m[1][2] = (double)_Id7;
		m[1][3] = (double)_Id8;
		m[2][0] = (double)_Id9;
		m[2][1] = (double)_Id10;
		m[2][2] = (double)_Id11;
		m[2][3] = (double)_Id12;
		m[3][0] = (double)_Id13;
		m[3][1] = (double)_Id14;
		m[3][2] = (double)_Id15;
		m[3][3] = (double)_Id16;
	}
	Mat4x4()
	{
		Set(glm::mat4x4());
	}
	Mat4x4(glm::mat4x4 _Mat)
	{ 
		const float *pSource = (const float*)glm::value_ptr(_Mat);
		int i, j;
		int k = 0;
		for(i = 0; i<4; i++)
			{
				for(j=0; j<4; j++)
					{
						m[i][j] = pSource[k];	
						++k;
					}
			}
	}
	
	// Modifiers
	inline void SetPosition(Vec3 const &_Position);
	inline void SetPosition(Vec4 const &_Position);


	// Accessors and Calculation Methods
	inline Vec3 GetPosition() const
	{
		return Vec3(m[3][0], m[3][1], m[3][2]);
	}

	/*
	inline Vec4 XForm(Vec4 &_v) const
	{
		Vec4 Temp;
		
	}

	inline Vec3 XForm(Vec3 &_v) const
	{
		Vec4 Temp;
		
	}
	*/
	inline Mat4x4 Inverse() const
	{
		
		int i, j;
		int k = 0;
		float OneDimensionalMatrix[16];
 
		for(i = 0; i<4; i++)
		{
			for(j=0; j<4; j++)
				{
					OneDimensionalMatrix[k] = m[i][j];	
					++k;
				}
		}

		glm::mat4x4 Mat = glm::make_mat4x4(OneDimensionalMatrix);
		glm::mat4x4 InversedMatrix = glm::inverse(Mat);
		
		return Mat4x4(InversedMatrix);
	}

	// Initialization Methods
	inline void BuildTranslation(const Vec3 &_Position)
	{
		Set(glm::mat4x4());
		m[3][0] = _Position.x;	m[3][1] = _Position.y;	m[3][2] = _Position.z;	
	}

	inline void BuildTranslation(const float _x, const float _y, const float _z)
	{
		Set(glm::mat4x4());
		//*this = Mat4x4::g_Identity;
		m[3][0] = _x;	m[3][1] = _x;	m[3][2] = _z;	
	}

	inline void BuildRotationX(const float _Radians)
	{
		int Direction = 1; if (_Radians < 0) Direction *= -1;
		float Degrees = RADIANS_TO_DEGREES(_Radians);
		int i, j;
		int k = 0;
		float OneDimensionalMatrix[16];
 
		for(i = 0; i<4; i++)
		{
			for(j=0; j<4; j++)
				{
					OneDimensionalMatrix[k] = m[i][j];	
					++k;
				}
		}
		glm::mat4x4 Mat = glm::make_mat4x4(OneDimensionalMatrix);
		Set(glm::rotate(Mat, Degrees, glm::vec3(Direction,0,0)));
	}

	inline void BuildRotationY(const float _Radians)
	{
		int Direction = 1; if (_Radians < 0) Direction *= -1;
		float Degrees = RADIANS_TO_DEGREES(_Radians);
		int i, j;
		int k = 0;
		float OneDimensionalMatrix[16];
 
		for(i = 0; i<4; i++)
		{
			for(j=0; j<4; j++)
				{
					OneDimensionalMatrix[k] = m[i][j];	
					++k;
				}
		}
		glm::mat4x4 Mat = glm::make_mat4x4(OneDimensionalMatrix);
		Set(glm::rotate(Mat, Degrees, glm::vec3(0,Direction,0)));
	}

	inline void BuildRotationZ(const float _Radians)
	{
		int Direction = 1; if (_Radians < 0) Direction *= -1;
		float Degrees = RADIANS_TO_DEGREES(_Radians);
		int i, j;
		int k = 0;
		float OneDimensionalMatrix[16];
 
		for(i = 0; i<4; i++)
		{
			for(j=0; j<4; j++)
				{
					OneDimensionalMatrix[k] = m[i][j];	
					++k;
				}
		}
		glm::mat4x4 Mat = glm::make_mat4x4(OneDimensionalMatrix);
		Set(glm::rotate(Mat, Degrees, glm::vec3(0,0,Direction)));
	}

	inline void BuildYawPitchRoll
		(const float _YawRadians, const float _PitchRadians, const float _RollRadians)
			{
				// Add Implementation Here!

			}

	inline void BuildRotationQuat(const Quaternion &_Q)
	{
		//glm::quat Q = glm::make_quat(_Q);
	}

	inline void BuildRotationLookAt(const Vec3 &_Eye, const Vec3 &_At, const Vec3 &_Up)
	{
		glm::vec3 Eye = glm::vec3(_Eye.x, _Eye.y, _Eye.z );
		glm::vec3 At = glm::vec3(_At.x, _At.y, _At.z );
		glm::vec3 Up = glm::vec3(_Up.x, _Up.y, _Up.z );
		glm::mat4x4 LookAt = glm::lookAt(Eye, At, Up);
		Set(LookAt);
	}
};

inline void Quaternion::Build(const class Mat4x4 &_Mat)
	{
		int i, j;
		int k = 0;
		float OneDimensionalMatrix[16];
 
		for(i = 0; i<4; i++)
		{
			for(j=0; j<4; j++)
				{
					OneDimensionalMatrix[k] = _Mat.m[i][j];	
					++k;
				}
		}
		glm::mat4x4 Mat = glm::make_mat4x4(OneDimensionalMatrix);
		Quat = glm::quat(Mat);
	}

inline void Mat4x4::SetPosition(Vec3 const &_Position)
	{
		m[3][0] = _Position.x;
		m[3][1] = _Position.y;
		m[3][2] = _Position.z;
		m[3][3] = 1.0f;
	}

inline void Mat4x4::SetPosition(Vec4 const &_Position)
	{
		m[3][0] = _Position.x;
		m[3][1] = _Position.y;
		m[3][2] = _Position.z;
		m[3][3] = _Position.w;
	}

inline Vec3 operator * (const Vec3 &_a, const Vec3 &_b) 
{
	glm::vec3 a = glm::vec3(_a.x, _a.y, _a.z);
	glm::vec3 b = glm::vec3(_b.x, _b.y, _b.z);
	Vec3 out;
	out = a * b;
	return out;
}

inline Vec3 operator + (const Vec3 &_a, const Vec3 &_b) 
{
	glm::vec3 a = glm::vec3(_a.x, _a.y, _a.z);
	glm::vec3 b = glm::vec3(_b.x, _b.y, _b.z);
	Vec3 out;
	out = a + b;
	return out;
}

inline Vec3 operator - (const Vec3 &_a, const Vec3 &_b) 
{
	glm::vec3 a = glm::vec3(_a.x, _a.y, _a.z);
	glm::vec3 b = glm::vec3(_b.x, _b.y, _b.z);

	glm::vec3 Result = a - b;
	//D3DXMatrixMultiply(&out, &a, &b);
	
	return Vec3(Result);
}

inline Vec3 operator / (const Vec3 &_a, const Vec3 &_b) 
{
	glm::vec3 a = glm::vec3(_a.x, _a.y, _a.z);
	glm::vec3 b = glm::vec3(_b.x, _b.y, _b.z);
	Vec3 out;
	out = a / b;
	return out;
}

inline Mat4x4 operator * (const Mat4x4 &a, const Mat4x4 &b) 
{
	Mat4x4 out;
	
	//D3DXMatrixMultiply(&out, &a, &b);
	
	return out;
}



inline Quaternion operator * (const Quaternion &a, const Quaternion &b) 
{
	Quaternion out(a.Quat * b.Quat);
	//D3DXMatrixMultiply(&out, &a, &b);

	return out;
}




////////////////////////////////////////////////////
//
// Plane Definition
// TODO: Init!
//
////////////////////////////////////////////////////

class Plane
{
public:
	float a, b, c, d;

	inline void Normalize();

	inline void Init(const Vec3& _Point1, const Vec3& _Point2, const Vec3& _Point3, const Vec3& _Point4);
	bool IsInside(const Vec3& _Point, const float _Radius) const;
	bool IsInside(const Vec3& _Point) const;
};

inline void Plane::Normalize()
{
	float Magnitude;
	Magnitude = sqrt(a * a + b * b + c * c);
	a = a / Magnitude;
	b = b / Magnitude;
	c = c / Magnitude;
	d = d / Magnitude;
}

inline void Plane::Init(const Vec3& _Point1, const Vec3& _Point2, const Vec3& _Point3, const Vec3& _Point4)
{
	Normalize();
}


////////////////////////////////////////////////////
//
// Frustum Definition
//
////////////////////////////////////////////////////
/*
class Frustum
{
public:
	enum Side { Near, Far, Top, Right, Bottom, Left, NumPlanes };

};*/