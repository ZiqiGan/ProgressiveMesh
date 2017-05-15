#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis)
{

	float radian = degrees*pi / 180.0;

	//normalize the vector
	glm::vec3 axisNorm = glm::normalize(axis);

	//compute the component of A
	glm::mat3 Acomponent;
	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j<3; j++)
		{
			Acomponent[i][j] = axisNorm[i] * axisNorm[j];
		}
	}

	//define A star
	glm::mat3 Astar = glm::mat3(0, axisNorm[2], -axisNorm[1],
		-axisNorm[2], 0, axisNorm[0],
		axisNorm[1], -axisNorm[0], 0);
	glm::mat3 I(1.0);

	//calculate the rotation matrix
	I = ((float)cos(radian))*I;
	Acomponent = ((float)(1 - cos(radian)))*Acomponent;
	Astar = ((float)sin(radian))*Astar;
	return I + Acomponent + Astar;
}

void Transform::left(float degrees, vec3& eye, vec3& up)
{
	mat3 rotationMx = Transform::rotate(degrees, up);
	eye = rotationMx*eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up)
{
	vec3 axis = glm::cross(eye, up);
	up = Transform::rotate(degrees, axis)*up;
	eye = Transform::rotate(degrees, axis)*eye;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
{
	vec3 w = glm::normalize(eye);
	vec3 u = glm::normalize(glm::cross(glm::normalize(up), w));
	vec3 v = glm::cross(w, u);
	mat4 rotation = mat4(u.x, u.y, u.z, 0, v.x, v.y, v.z, 0, w.x, w.y, w.z, 0, 0, 0, 0, 1);
	rotation = glm::transpose(rotation);
	mat4 translation = mat4(1, 0, 0, -eye.x, 0, 1, 0, -eye.y, 0, 0, 1, -eye.z, 0, 0, 0, 1);
	translation = glm::transpose(translation);
	return rotation*translation;

}

/*calculate perspective*/
mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
	float theta = (fovy / 2)*pi / 180;
	float d = 1 / tan(theta);
	float A = -(zFar + zNear) / (zFar - zNear);
	float B = (-2 * zFar*zNear) / (zFar - zNear);
	glm::mat4 perspective = mat4(d / aspect, 0, 0, 0,
		0, d, 0, 0,
		0, 0, A, B,
		0, 0, -1, 0);
	perspective = glm::transpose(perspective);
	return perspective;
}

/*calculate scaling*/
mat4 Transform::scale(const float &sx, const float &sy, const float &sz)
{
	glm::mat4 scaling = mat4(sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, 0, 0, 0, 0, 1);
	scaling = glm::transpose(scaling);
	return scaling;
}

/*Calculate the translation*/
mat4 Transform::translate(const float &tx, const float &ty, const float &tz)
{
	glm::mat4 translation = mat4(1, 0, 0, tx, 0, 1, 0, ty, 0, 0, 1, tz, 0, 0, 0, 1);
	translation = glm::transpose(translation);
	return translation;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec)
{
	vec3 x = glm::cross(up, zvec);
	vec3 y = glm::cross(zvec, x);
	vec3 ret = glm::normalize(y);
	return ret;
}


Transform::Transform()
{

}

Transform::~Transform()
{

}