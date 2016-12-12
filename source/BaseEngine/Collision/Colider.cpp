#include "Colider.h"
#include  "../Utils/Utils.h"
CColider::CColider()
: m_UpdateVector(0)
{
}

CColider::CColider(SSphere sphere)
: m_Sphere(sphere)
, m_ColiderType(ColiderType::SPHERE_COLIDER)
, m_UpdateVector(0)
{
}

CColider::CColider(SAabb aabb)
: m_ColiderType(ColiderType::BOX_COLIDER)
, m_Aabb(aabb)
, m_UpdateVector(0)
{
}

CColider::CColider(std::vector<SFace> faces)
: m_ColiderType(ColiderType::MESH_COLIDER)
, m_Faces(faces)
, m_OrginalFaces(faces)
, m_UpdateVector(0)
{

}

void CColider::TransformFaces(const glm::mat4 & matrix)
{
	m_Faces = m_OrginalFaces;
	for (SFace& face : m_Faces)
	{
		face.vertexes.v1 = Utils::TransformPoint(face.vertexes.v1, matrix);
		face.vertexes.v2 = Utils::TransformPoint(face.vertexes.v2, matrix);
		face.vertexes.v3 = Utils::TransformPoint(face.vertexes.v3, matrix);
		face.CalculateNormal();
	}
}

void CColider::SetPosition(glm::vec3 position)
{
	switch (m_ColiderType)
	{
	case ColiderType::BOX_COLIDER:
		m_Aabb.position = position;
		break;
	case ColiderType::SPHERE_COLIDER:
		m_Sphere.position = position;
		break;
	}
}

const glm::vec3& CColider::GetPosition() const
{
	switch (m_ColiderType)
		{
		case ColiderType::BOX_COLIDER:
			return m_Aabb.position;
			break;
		case ColiderType::SPHERE_COLIDER:
			return m_Sphere.position;
			break;
		}
	return m_Sphere.position;
}

float CColider::GetHeight(const float& x, const float& y, const float& z) const
{
	float height = -1;
	float yy = y + 0.1;
	if (m_ColiderType == ColiderType::MESH_COLIDER)
	{
		for (const SFace& face : m_Faces)
		{			
			glm::vec3 ray_dir(0.f, -1.f, 0.f);			
			
			float h = Utils::IntersectTriangle(face.normal, glm::vec3(x, y, z), ray_dir, face.vertexes);
			//Utils::PrintVector("", glm::vec3(h, 0, z));
			if (h > 0)
				if ((yy - h) > height)
				{
					height = yy - h;
				}				
		}
	}
	
	return height;
}

bool CColider::DetectCollision(CColider& colider, std::list<SCollisionInfo>& colision_info) const
{
	switch (m_ColiderType)
	{
	case ColiderType::BOX_COLIDER:
		switch (colider.m_ColiderType)
		{
			case ColiderType::BOX_COLIDER:
				colision_info.push_back(SCollisionInfo());
				return DettectAABBAABB(m_Aabb, colider.m_Aabb, colision_info.back());
				break;
			case ColiderType::SPHERE_COLIDER:
				//return DetectSphereSphere(m_Sphere, colider.m_Sphere);
				break;

		}
		break;
	case ColiderType::SPHERE_COLIDER:
		switch (colider.m_ColiderType)
		{
		case ColiderType::SPHERE_COLIDER:
			colision_info.push_back(SCollisionInfo());
			return DetectSphereSphere(m_Sphere, colider.m_Sphere, colision_info.back());
			break;
		case ColiderType::MESH_COLIDER:
			return DetectFaceSphere(m_Sphere, colider.m_Faces, colision_info);
			break;
		}
		break;
	case ColiderType::MESH_COLIDER:
		switch (colider.m_ColiderType)
		{
		case ColiderType::SPHERE_COLIDER:
			return DetectFaceSphere(colider.m_Sphere, m_Faces, colision_info);
			break;
		}
		break;
	}
	return false;
}

bool CColider::DetectSphereSphere(const SSphere& a, const SSphere& b, SCollisionInfo& colision_info) const
{
	
	float aa =	(b.position.x - a.position.x)*(b.position.x - a.position.x) +
				(b.position.y - a.position.y)*(b.position.y - a.position.y) +
				(b.position.z - a.position.z)*(b.position.z - a.position.z);
	float r = a.radius + b.radius;
	r *= r;
	
	if (aa <= r)
	{
		colision_info.collision = true;
		glm::vec3 n = a.position - b.position;
		float d = glm::length(n);
		colision_info.t = r - d;
		colision_info.updateVector = n*(r - d);
		colision_info.normal = n / d;
		return true;
	}			
	else

		return false;
}

bool CColider::DetectFaceSphere(const SSphere& a, const std::vector<SFace>& faces, std::list<SCollisionInfo>& colision_info) const
{
	bool is_col = false;
	for (const SFace& face : faces)
	{
		SCollisionInfo info;
		Utils::DetectSphereTriangleCollision(a.position, a.radius, face.vertexes, face.normal, info);
		if (info.collision)
		{
			is_col = true;
			colision_info.push_back(info);
		}
			
	}
	return is_col;
}

bool CColider::DettectAABBAABB(const SAabb & a, const SAabb & b, SCollisionInfo& colision_info) const
{
	if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
	if (a.max.y < b.min.y || a.min.y > b.max.y) return false;
	if (a.max.z < b.min.z || a.min.z > b.max.z) return false;
	return true;
}
