#include "stdafx.h"
#include "FireBall.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

FireBall::FireBall()
{

}

FireBall::~FireBall()
{

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
	return true;
}

void FireBall::Update()
{
	


	//�^�C�}�[�����Z����B
	m_timer += g_gameTime->GetFrameDeltaTime();

	//�^�C�}�[��0.7f�ȏゾ������B
	if (m_timer >= 0.7f)
	{
		//���g���폜����B
		DeleteGO(this);
	}
}