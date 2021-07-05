#include "stdafx.h"
#include "Lever.h"

#include "Door.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

Lever::Lever()
{

}

Lever::~Lever()
{

}

bool Lever::Start()
{
	//�A�j���[�V������ǂݍ��ށB
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/lever/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Push].Load("Assets/animData/lever/push.tka");
	m_animationClips[enAnimationClip_Push].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Pull].Load("Assets/animData/lever/pull.tka");
	m_animationClips[enAnimationClip_Pull].SetLoopFlag(false);
	//���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/lever/lever.tkm", m_animationClips, enAnimationClip_Num);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale(m_scale);
	m_modelRender.SetRotation(m_rotation);

	//����ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/lever.wav");
	return true;
}

void Lever::Update()
{
	//�A�j���[�V�������Đ�����B
	PlayAnimation();
	//�X�e�[�g�Ǘ��B
	ManageState();

	//���f���̍X�V�B
	m_modelRender.Update();
}

void Lever::ProcessTransitionPushState()
{
	
}

void Lever::ProcessTransitionPullState()
{
	
}

void Lever::PlayAnimation()
{
	switch (m_leverState)
	{
	//�ҋ@�X�e�[�g�Ȃ�B
	case enLeverState_Idle:
		//�ҋ@�A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		break;
	//�����X�e�[�g�̎��B
	case enLeverState_Push:
		//�����A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimationClip_Push);
		break;
	//�����X�e�[�g�̂Ƃ��B
	case enLeverState_Pull:
		//�����A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimationClip_Pull);
		break;
	default:
		break;
	}
}

void Lever::ProcessTransitionPushIdleState()
{
	//�����A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�h�A�̃I�u�W�F�N�g�̔z����擾����B
		auto doors = FindGOs<Door>("door");
		//for���Ŕz����񂷁B
		for (auto door : doors)
		{
		
		}
	}
}

void Lever::ProcessTransitionPullIdleState()
{
	//�����A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�h�A�̃I�u�W�F�N�g�̔z����擾����B
		auto doors = FindGOs<Door>("door");
		//for���Ŕz����񂷁B
		for (auto door : doors)
		{
			
		}
	}
}

void Lever::ProcessIdleStateTransition()
{
	//�ҋ@��Ԃ��牟����Ԃւ̑J�ځB
	ProcessTransitionPushState();
}

void Lever::ProcessPushStateTransition()
{
	//������Ԃ���ҋ@��Ԃւ̑J�ځB
	ProcessTransitionPushIdleState();
}

void Lever::ProcessPushIdleStateTransition()
{
	//�ҋ@��Ԃ��������Ԃւ̑J�ځB
	ProcessTransitionPullState();
}

void Lever::ProcessPullStateTransition()
{
	//������Ԃ���ҋ@��Ԃւ̑J�ځB
	ProcessTransitionPullIdleState();
}

void Lever::ProcessPullIdleStateTransition()
{
	//�ҋ@��Ԃ��牟����Ԃւ̑J�ځB
	ProcessTransitionPushState();
}

void Lever::ManageState()
{
	switch (m_leverState)
	{
	//�ҋ@�X�e�[�g�̎��B
	case enLeverState_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessIdleStateTransition();
		break;
	//�����X�e�[�g�̎��B
	case enLeverState_Push:
		//�����X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessPushStateTransition();
		break;
	//�����I������X�e�[�g�̎��B
	case enLeverState_Push_Idle:
		//�����I������X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessPushIdleStateTransition();
		break;
	//�����X�e�[�g�̎��B
	case enLeverState_Pull:
		//�����X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessPullStateTransition();
		break;
	//�����I������X�e�[�g�̎��B
	case enLeverState_Pull_Idle:
		//�����I������X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessPullIdleStateTransition();
		break;
	default:
		break;
	}
}

void Lever::Render(RenderContext& rc)
{
	//���f����`�悷��B
	m_modelRender.Draw(rc);
}