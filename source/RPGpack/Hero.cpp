#include "Hero.h"

void CHero::Create(CScene * scene)
{
	glm::vec3 normalized_size(0, 1.8f, 0);
	unsigned int model_id;
	int x = -1;
	for (const auto& model_file : m_Models)
	{
		x++;
		if (model_file.empty())
			continue;
		try
		{
			model_id = scene->GetLoader().LoadMesh(model_file, true);
		}
		catch (std::exception& e)
		{
			cout << e.what();
			throw e;
		}
		scene->GetLoader().m_Models[model_id]->m_UseFakeLight = true;
		scene->GetLoader().m_Models[model_id]->m_BoneUpdate = true;
		m_CurrentAnimationFrame = rand() % 30;
		scene->GetLoader().m_Models[model_id]->AddCurrentFramePtr(&m_CurrentAnimationFrame);

		m_ModelsIds[x] = model_id;
		//AddModel(model_id, model_file);
	}
	glm::mat4 nm = scene->GetLoader().m_Models[model_id]->CalculateNormalizedMatrix(normalized_size.x, normalized_size.y, normalized_size.z);
	SetNormalizedMatrix(nm);
	SetNormalizedSize(normalized_size);
	//m_IsSpecial - enity created in code not from file
	m_IsSpecial = true;
	m_RigidBody = CRigidbody(SSphere(glm::vec3(0), 0.5));
	m_RigidBody.m_Static = false;
	scene->m_PhysicsScene.AddRigidBody(&m_RigidBody);
	scene->GetPhysicsEntities().push_back(this);
}

void CHero::Move(const float& delta_time)
{	
	IncreaseRotation(0, m_CharacterStats.m_CurrentTurnSpeed * delta_time, 0);
	float distance = m_CharacterStats.m_CurrentMoveSpeed;
	float dx = static_cast<float>(distance * sin(Utils::ToRadians(this->GetRotation().y)));
	float dz = static_cast<float>(distance * cos(Utils::ToRadians(this->GetRotation().y)));
	m_RigidBody.m_AngularVelocity.x = dx;
	m_RigidBody.m_AngularVelocity.z = dz;
	m_RigidBody.m_Restitution = 0;

	//action = static_cast<CharacterActions::Type>(3);

	//if (m_CharacterStats.m_CurrentMoveSpeed <= 0 && m_Action < CharacterActions::RUN)
	//	SetAction(CharacterActions::IDLE);
	//else if (m_CharacterStats.m_CurrentMoveSpeed > 0 && m_Action < CharacterActions::WALK)
	//	SetAction(CharacterActions::RUN);
//	else if (action != CharacterActions::ATTACK_1)
//		action = CharacterActions::ATTACK_1;
	
	SetModelId(m_Action);
}

void CHero::Jump()
{
	if (m_RigidBody.m_IsGrounded)
	{
		m_RigidBody.m_AngularVelocity.y = m_CharacterStats.m_JumpPower;
		m_RigidBody.m_IsGrounded = false;
	}
}

void CHero::SetAction(CharacterActions::Type action)
{
	if (action != m_LastAction)
	{
		m_LastAction = m_Action;
		m_Action = action;
		m_CurrentAnimationFrame = 0;
	}
	else
	{
		m_Action = action;
		m_LastAction = action;
	}
}

const int CHero::GetAttackDamage()
{
	return 2;
}

const int CHero::GetMagickDamage()
{
	return 0;
}

const int CHero::GetArmor()
{
	return 1;
}

const int CHero::GetMegicResist()
{
	return 0;
}

void CHero::SetModelId(CharacterActions::Type type)
{
	int model_id = m_ModelsIds[type];
	if (model_id >= 0)
		m_ModelID = model_id;
}
