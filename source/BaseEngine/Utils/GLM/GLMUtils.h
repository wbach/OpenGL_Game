#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../../Loader/Face.h"
#include "../Types.h"
#include <vector>
#include <iostream>
#include <string>

namespace Utils
{
	static int s_intmone = -1;
	static glm::vec2 s_vec2_zero(0);
	static glm::vec3 s_vec3_zero(0);
	static glm::mat4 s_mat4_one(1.f);	
	

	static glm::vec3 ColorLerpRGB(const glm::vec3 & c1, const glm::vec3 & c2, const float & blend)
	{
		glm::vec3 color;
		color.x = c1.x + (c2.x - c1.x) * blend;
		color.y = c1.y + (c2.y - c1.y) * blend;
		color.z = c1.z + (c2.z - c1.z) * blend;
		return color;
	}

	static glm::vec3 TransformPoint(const glm::vec3 & point, const glm::mat4 & matrix)
	{
		glm::vec4 n = matrix * glm::vec4(point, 1.0f);
		return glm::vec3(n.x, n.y, n.z);
	}

	static glm::vec3 RGBtoFloat(const glm::vec3 & color)
	{
		return color / 255.f;
	}

	static glm::vec3 RGBtoFloat(const float & r, const float & g, const float & b)
	{
		return RGBtoFloat(glm::vec3(r, g, b));
	}

	static void PrintVector(const std::string & text, const glm::vec3 & v)
	{
		std::cout << text << v.x << " " << v.y << " " << v.z << std::endl;
	}

	static void PrintVector(const std::string & text, const glm::vec2 & v)
	{
		std::cout << text << v.x << " " << v.y << std::endl;
	}

	static void PrintMatrix(const std::string & text, const glm::mat4 & m)
	{
		for (uint y = 0; y < 4; y++)
			std::cout << text << m[y][0] << " " << m[y][1] << " " << m[y][2] << " " << m[y][3] << std::endl;
	}

	static void PrtintMat4(const glm::mat4 & mat)
	{
		printf("\nMat:\n %f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n\n",
			mat[0][0], mat[0][1], mat[0][2], mat[0][3],
			mat[1][0], mat[1][1], mat[1][2], mat[1][3],
			mat[2][0], mat[2][1], mat[2][2], mat[2][3],
			mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
	}

	static float GetMaxFromVector(const glm::vec3 & vector)
	{
		if (vector.x > vector.y && vector.x > vector.z)
			return vector.x;
		else if (vector.y > vector.z)
			return vector.y;
		else
			return vector.z;
	}

	static glm::mat4 CreateTransformationMatrix(const glm::vec3& translation = glm::vec3(.0f), const glm::vec3& rotation = glm::vec3(.0f), const glm::vec3& scale = glm::vec3(1.f))
	{
		glm::mat4  matrix = glm::mat4(1.0);
		matrix *= glm::translate(translation);
		matrix *= glm::rotate(rotation.x, 1.0f, 0.0f, 0.0f);
		matrix *= glm::rotate(rotation.y, 0.0f, 1.0f, 0.0f);
		matrix *= glm::rotate(rotation.z, 0.0f, 0.0f, 1.0f);
		matrix *= glm::scale(scale.x, scale.y, scale.z);
		return matrix;
	}

	static glm::mat4 CreateTransformationMatrix(const glm::vec2 & translation, const glm::vec2 & scale, const float& rotation = 0.f)
	{
		glm::mat4  matrix = glm::mat4(1.0);
		matrix *= glm::translate(glm::vec3(translation, 0.0f));
		matrix *= glm::rotate(rotation, 0.0f, 0.0f, 1.0f);
		matrix *= glm::scale(glm::vec3(scale, 1.0f));
		return matrix;
	}

	static void CalculateBoudnigBox(const std::vector<float>& positions, glm::vec3 & bounding_min, glm::vec3 & bounding_max, glm::vec3 & bounding_size, glm::vec3 & bounding_center)
	{
		if (positions.size() == 0)
			return;

		bounding_min.x = bounding_max.x = positions[0];
		bounding_min.y = bounding_max.y = positions[1];
		bounding_min.z = bounding_max.z = positions[2];
		for (uint i = 0; i < positions.size(); i += 3)
		{
			if (positions[i] < bounding_min.x) bounding_min.x = positions[i];
			if (positions[i] > bounding_max.x) bounding_max.x = positions[i];
			if (positions[i + 1] < bounding_min.y) bounding_min.y = positions[i + 1];
			if (positions[i + 1] > bounding_max.y) bounding_max.y = positions[i + 1];
			if (positions[i + 2] < bounding_min.z) bounding_min.z = positions[i + 2];
			if (positions[i + 2] > bounding_max.z) bounding_max.z = positions[i + 2];
		}
		bounding_size = bounding_max - bounding_min;
		bounding_center = (bounding_max + bounding_min) / 2.f;
	}

	static glm::vec3 CalculateMinimumVector(const glm::vec3 & a, const glm::vec3 & b)
	{
		glm::vec3 result;
		result.x = a.x < b.x ? a.x : b.x;
		result.y = a.y < b.y ? a.y : b.y;
		result.z = a.z < b.z ? a.z : b.z;
		return result;
	}

	static float BarryCentric(const glm::vec3 & p1, const glm::vec3 & p2, const glm::vec3 & p3, const glm::vec2 & pos)
	{
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}

	static glm::vec3 BarryCentricVec3(const glm::vec3 & p1, const glm::vec3 & p2, const glm::vec3 & p3, const glm::vec2 & pos)
	{
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return glm::vec3(l1, l2, l3);
	}

	static glm::mat4 CreateOffset()
	{
		glm::mat4 offset;
		offset *= glm::translate(glm::vec3(0.5f, 0.5f, 0.5f));
		offset *= glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));
		return offset;
	}

	static glm::vec2 toScreenSpace(glm::vec2 & position, glm::vec2 & window_size)
	{
		return glm::vec2((position.x / window_size.x) * 2 - 1, (1 - (position.y / window_size.y)) * 2 - 1);
	}

	static std::string ToString(const glm::vec3 & v)
	{
		return std::to_string(v.x) + " " + std::to_string(v.y) + " " + std::to_string(v.z);
	}

	static std::string ToString(const glm::vec2 & v)
	{
		return std::to_string(v.x) + " " + std::to_string(v.y);
	}

	static std::string ToString(const glm::mat4 & m)
	{
		std::string s;
		for (int y = 0; y < 4; y++)
			s += std::to_string(m[y][0]) + " " + std::to_string(m[y][1]) + " " + std::to_string(m[y][2]) + " " + std::to_string(m[y][3]) + "\n";
		return s;
	}

	static float IntersectTriangle(const glm::vec3 & normal, const glm::vec3 & rayPos, const glm::vec3 & rayDirection, const SFaceVertex & vertexes)
	{
		glm::vec3 v0 = vertexes.v3 - vertexes.v1;
		glm::vec3 v1 = vertexes.v2 - vertexes.v1;
		float d = -1 * (glm::dot(normal, vertexes.v1));
		float t = -(normal.x*rayPos.x + normal.y*rayPos.y + normal.z*rayPos.z + d) / (normal.x*rayDirection.x + normal.y*rayDirection.y + normal.z*rayDirection.z);
		glm::vec3 point = rayPos + t*rayDirection;
		glm::vec3 v2 = point - vertexes.v1;
		float dot00 = glm::dot(v0, v0);
		float dot01 = glm::dot(v0, v1);
		float dot02 = glm::dot(v0, v2);
		float dot11 = glm::dot(v1, v1);
		float dot12 = glm::dot(v1, v2);
		float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
		float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
		if ((u >= 0) && (v >= 0) && (u + v < 1))
		{
			return t;
		}
		else return -1;
	}

	static bool DetectSphereTriangleCollision(const glm::vec3 & sp, const float & r, const SFaceVertex & vertexes, const glm::vec3 & normal, SCollisionInfo & colision_info)
	{
		glm::vec3 n = glm::normalize(normal);

		float dist1 = IntersectTriangle(-n, sp, n, vertexes);
		float dist2 = IntersectTriangle(n, sp, -n, vertexes);

		colision_info.collision = false;
		colision_info.normal = n;
		colision_info.updateVector = glm::vec3(0);

		if (dist2 > 0 && dist2 < r) {
			colision_info.normal = n;
			colision_info.collision = true;
			colision_info.t = dist2;
			colision_info.updateVector = n*(r - dist2);
		}
		else if (dist1 > 0 && dist1 < r) {
			colision_info.normal = -n;
			colision_info.updateVector = -n*(r - dist1);
			colision_info.collision = true;
			colision_info.t = dist1;
			colision_info.collisionY = sp.y + colision_info.updateVector.y;
		}
		return colision_info.collision;
	}

}