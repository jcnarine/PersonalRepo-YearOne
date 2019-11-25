#include "Vector.h"

vec2::vec2()
{
}

//Initializes a vector 
vec2::vec2(float _x, float _y)
{
	//Sets x and y 
	x = _x;
	y = _y;
}

void vec2::Subtract(vec2 v2)
{
	//Subtracts v2 from THIS and saves here
	this->x -= v2.x;
	this->y -= v2.y;
}

void vec2::Add(vec2 v2)
{
	//Adds v2 to THIS and saves here
	this->x += v2.x;
	this->y += v2.y;
}

void vec2::MultScalar(float s)
{
	//Multiples s into THIS
	this->x *= s;
	this->y *= s;
}

void vec2::DivScalar(float s)
{
	//Divides THIS by s
	this->x /= s;
	this->y /= s;
}

float vec2::Dot(vec2 v2)
{
	//Returns the dot product between THIS and v2
	return float(x* v2.x + y * v2.y);
}

//Grabs the magnitude of the vector 
float vec2::GetMagnitude()
{
	return float(sqrt((x*x) + (y*y)));
}

float vec2::GetMagnitudeSquared()
{
	//Gets the magnitude and multiples it by itself
	float mag = GetMagnitude();
	//return the square
	return mag * mag;
}

vec2 vec2::Normalize()
{
	//Divides this by the magnitude to get a vector in the same direction but a magnitude of 1
	return (*this / GetMagnitude());
}

//Projects THIS onto b 
vec2 vec2::Project(vec2 b)
{
	//For readability 
	vec2 a = *this;

	//Equation
	//     A dot B
	//B * ---------
	//     ||B||^2

	float numerator = a.Dot(b);
	float denominator = b.GetMagnitudeSquared();

	return (b * (numerator / denominator));
}

//Lets us index the variable in i, 0 is x, 1 is y
float vec2::operator[](int i)
{
	//Indexes the variables at i 
	//* 0 = x
	//* 0 = y
	return *hold[i];
}

//Negates the vector, same magnitude with opposite direction 
vec2 vec2::operator-()
{
	return vec2(-x, -y);
}

//Adds this vector to another 
vec2 vec2::operator+(vec2 v1)
{
	return vec2(this->x + v1.x, this->y + v1.y);
}

//Subtracts another vector from this one 
vec2 vec2::operator-(vec2 v1)
{
	return vec2(this->x - v1.x, this->y - v1.y);
}

//Multiplies this vector by a float 
vec2 vec2::operator*(float f)
{
	return vec2(this->x * f, this->y * f);
}

//Divides the 2D vector by a float 
vec2 vec2::operator/(float f)
{
	return vec2(this->x / f, this->y / f);
}

vec3::vec3()
{
}

//Initialize the 3D vector 
vec3::vec3(float _x, float _y, float _z)
{
	//Sets x, y, and z 
	x = _x;
	y = _y;
	z = _z;
}

//Subtracts v2 from THIS and saves here
void vec3::Subtract(vec3 v2)
{
	this->x -= v2.x;
	this->y -= v2.y;
	this->z -= v2.z;
}

//Adds v2 to THIS and saves here
void vec3::Add(vec3 v2)
{
	this->x += v2.x;
	this->y += v2.y;
	this->z += v2.z;
}

//Multiples s into THIS
void vec3::MultScalar(float s)
{
	this->x *= s;
	this->y *= s;
	this->z *= s;
}

//Divides THIS by s 
void vec3::DivScalar(float s)
{
	this->x /= s;
	this->y /= s;
	this->z /= s;
}

//Returns the dot product of THIS and v2 
float vec3::Dot(vec3 v2)
{
	return float((x * v2.x) + (y * v2.y) + (z * v2.z));
}

//Gets the magnitude of the 3D vector 
float vec3::GetMagnitude()
{
	return float(sqrt((x*x) + (y*y) + (z*z)));
}

//Returns the magnitude squared 
float vec3::GetMagnitudeSquared()
{
	float mag = GetMagnitude();
	return mag * mag;
}

//Returns the vector with some direction but a magnitude of 1 
vec3 vec3::Normalize()
{
	return (*this / GetMagnitude());
}

//Projects THIS onto b 
vec3 vec3::Project(vec3 b)
{
	//For readability 
	vec3 a = *this;

	//Equation
	//     A dot B
	//B * ---------
	//     ||B||^2

	float numerator = a.Dot(b);
	float denominator = b.GetMagnitudeSquared();

	return (b * (numerator / denominator));
}

//Allows us to index the vector with square brackets, 0 is x, 1 is y, 2 is z 
float vec3::operator[](int i)
{
	return *hold[i];
}

//Negates the 3D vector, getting same magintude with opposite direction 
vec3 vec3::operator-()
{
	return vec3(-x, -y, -z);
}

//Adds 2 3D vectors together 
vec3 vec3::operator+(vec3 v2)
{
	return vec3(this->x + v2.x, this->y + v2.y, this->z + v2.z);
}

//Subtracts another 3D vector from this one 
vec3 vec3::operator-(vec3 v2)
{
	return vec3(this->x - v2.x, this->y - v2.y, this->z - v2.z);
}

//Multiples the 3D vector by a float 
vec3 vec3::operator*(float f)
{
	return vec3(this->x * f, this->y * f, this->z * f);
}

//Divides the 3D vector by a float 
vec3 vec3::operator/(float f)
{
	return vec3(this->x / f, this->y / f, this->z / f);
}

vec4::vec4()
{
}

//Initializes the 4D vector 
vec4::vec4(float _x, float _y, float _z, float _w)
{
	//Sets x, y, z, and w 
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

//Subtracts v2 from THIS and stores here
void vec4::Subtract(vec4 v2)
{
	this->x -= v2.x;
	this->y -= v2.y;
	this->z -= v2.z;
	this->w -= v2.w;
}

//Adds v2 to THIS and stores here 
void vec4::Add(vec4 v2)
{
	this->x += v2.x;
	this->y += v2.y;
	this->z += v2.z;
	this->w += v2.w;
}

//Multiples s into THIS
void vec4::MultScalar(float s)
{
	this->x *= s;
	this->y *= s;
	this->z *= s;
	this->w *= s;
}

//Divides THIS by s 
void vec4::DivScalar(float s)
{
	this->x /= s;
	this->y /= s;
	this->z /= s;
	this->w /= s;
}

//Returns the dot product of THIS and v2 
float vec4::Dot(vec4 v2)
{
	return float((x * v2.x) + (y * v2.y) + (z * v2.z) + (w * v2.w));
}

//Get the magnitude of the 4D vector 
float vec4::GetMagnitude()
{
	return float(sqrt((x*x) + (y*y) + (z*z) + (w*w)));
}

//Returns the magnitude squared of the 4D vector
float vec4::GetMagnitudeSquared()
{
	float mag = GetMagnitude();
	return mag * mag;
}

//Divide THIS by it's magnitude to get a vector in the same direction but with a magnitude of 1 
vec4 vec4::Normalize()
{
	return (*this / GetMagnitude());
}

//Projects THIS onto vector b 
vec4 vec4::Project(vec4 b)
{
	//For readability 
	vec4 a = *this;

	//Equation
	//     A dot B
	//B * ---------
	//     ||B||^2

	float numerator = a.Dot(b);
	float denominator = b.GetMagnitudeSquared();

	return (b * (numerator / denominator));
}

//Index components of the 4D vector, 0 is x, 1 is y, 2 is z, 3 is w 
float vec4::operator[](int i)
{
	return *hold[i];
}

vec4 vec4::operator-()
{
	return vec4(-x, -y, -z, -w);
}

vec4 vec4::operator+(vec4 v2)
{
	return vec4(this->x + v2.x, this->y + v2.y, this->z + v2.z, this->w + v2.w);
}

vec4 vec4::operator-(vec4 v2)
{
	return vec4(this->x - v2.x, this->y - v2.y, this->z - v2.z, this->w - v2.w);
}

vec4 vec4::operator*(float f)
{
	return vec4(this->x * f, this->y * f, this->z * f, this->w * f);
}

vec4 vec4::operator/(float f)
{
	return vec4(this->x / f, this->y / f, this->z / f, this->w / f);
}
