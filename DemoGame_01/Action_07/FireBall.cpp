#include "stdafx.h"
#include "FireBall.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//CollisionObject���g�p�������̂ŁA�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"

FireBall::FireBall()
{

}

FireBall::~FireBall()
{
	//�R���W�����I�u�W�F�N�g���폜����B
	DeleteGO(m_collisionObject);
}

bool FireBall::Start()
{
	
	//�ړ����x���v�Z�B
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);
	m_position += m_moveSpeed * 50.0f;
	m_moveSpeed *= 1200.0f;
	m_rotation.AddRotationDegY(360.0f);
	
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/fire_cut_cut.wav");
	//���ʉ����Đ�����B
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(1);
	se->Play(false);
	se->SetVolume(1.5f);

	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);
	//����̃R���W�������쐬����B
	m_collisionObject->CreateSphere(
		m_position,                     //���W�B
		Quaternion::Identity,           //��]�B
		35.0f                           //���a�B
	);

	//�p�҂��v���C���[��������B
	if (m_enMagician == enMagician_Player)
	{
		//���O��player_fireball�ɂ���B
		m_collisionObject->SetName("player_fireball");
	}
	//�p�҂��G�l�~�[��������B
	else if (m_enMagician == enMagician_Enemy)
	{
		//���O��enemy_fireball�ɂ���B
		m_collisionObject->SetName("enemy_fireball");
	}

	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	m_collisionObject->SetIsEnableAutoDelete(false);
	return true;
}

void FireBall::Update()
{
	//���W���ړ�������B
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	//�R���W�����I�u�W�F�N�g�ɍ��W��ݒ肷��B
	m_collisionObject->SetPosition(m_position);


	//�^�C�}�[�����Z����B
	m_timer += g_gameTime->GetFrameDeltaTime();

	//�^�C�}�[��0.7f�ȏゾ������B
	if (m_timer >= 0.7f)
	{
		//���g���폜����B
		DeleteGO(this);
	}
}