#include "stdafx.h"
#include "Enemy.h"

#include "Game.h"
#include "FireBall.h"
#include "Player.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

#include <time.h>
#include <stdlib.h>

//CollisionObject���g�p���������߁A�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"


Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

bool Enemy::Start()
{
	//�A�j���[�V������ǂݍ��ށB
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/enemy/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/enemy/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/enemy/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/enemy/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_MagicAttack].Load("Assets/animData/enemy/magicattack.tka");
	m_animationClips[enAnimationClip_MagicAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/enemy/receivedamage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/enemy/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	//���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/enemy/enemy.tkm", m_animationClips, enAnimationClip_Num);
	
	//���W��ݒ肷��B
	Vector3 position = m_position;
	position.y += 2.5f;
	m_modelRender.SetPosition(position);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);
	//�傫����ݒ肷��B
	m_modelRender.SetScale(m_scale);

	//�L�����N�^�[�R���g���[���[���������B
	m_charaCon.Init(
		20.0f,			//���a�B
		100.0f,			//�����B
		m_position		//���W�B
	);

	
	//����ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/magic.wav");
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/slash.wav");
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/hit.wav");

	m_player = FindGO<Player>("player");
	//�������������B
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	return true;
}

void Enemy::Update()
{
	//�ǐՏ����B
	Chase();
	//��]�����B
	Rotation();
	//�����蔻��B
	Collision();
	//�U�������B
	Attack();
	//�A�j���[�V�����̍Đ��B
	PlayAnimation();
	//�X�e�[�g�̑J�ڏ����B
	ManageState();
	
	//���f���̍X�V�B
	m_modelRender.Update();
}

void Enemy::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
		return;
	}
	//atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐��B
	//m_moveSpeed.x / m_moveSpeed.z�̌��ʂ�tan�ƂɂȂ�B
	//atan2���g�p���āA�p�x�����߂Ă���B
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
	//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
	m_rotation.SetRotationY(-angle);

	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);

	//�v���C���[�̑O�x�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Enemy::Chase()
{
	//�ǐՃX�e�[�g�łȂ��Ȃ�A�ǐՏ����͂��Ȃ��B
	if (m_enemyState != enEnemyState_Chase)
	{
		return;
	}

}

void Enemy::Collision()
{
	//��_���[�W�A���邢�̓_�E���X�e�[�g�̎��́B
	//�����蔻�菈���͂��Ȃ��B
	if (m_enemyState == enEnemyState_ReceiveDamage ||
		m_enemyState == enEnemyState_Down)
	{
		return;
	}

	//�v���C���[�̍U���p�̃R���W�������擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon))
		{
			//HP��1���炷�B
			m_hp -= 1;

			//����HP��0����Ȃ�B
			if (m_hp > 0)
			{
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_enemyState = enEnemyState_ReceiveDamage;
			}
			//HP��0�Ȃ�B
			else if (m_hp == 0)
			{
				//�_�E���X�e�[�g�ɑJ�ڂ���B
				m_enemyState = enEnemyState_Down;
			}

			//���ʉ����Đ�����B
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(4);
			se->Play(false);
			se->SetVolume(0.4f);
			return;
		}	
	}

	//�v���C���[�̃t�@�C���[�{�[���p�̃R���W�������擾����B
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_fireball");
	//for���Ŕz����񂷁B
	for (auto collision : collisions2)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon))
		{
			//HP��1���炷�B
			m_hp -= 1;
			//HP��0�ɂȂ�����B
			if (m_hp == 0)
			{
				//�_�E���X�e�[�g�ɑJ�ڂ���B
				m_enemyState = enEnemyState_Down;
			}
			else {
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_enemyState = enEnemyState_ReceiveDamage;
			}
			//���ʉ����Đ�����B
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(4);
			se->Play(false);
			se->SetVolume(0.6f);
			return;
		}
	}
}

void Enemy::Attack()
{
	//�U���X�e�[�g�łȂ��Ȃ珈�������Ȃ��B
	if (m_enemyState != enEnemyState_Attack)
	{
		return;
	}

	//�U�����ł���΁B
	if (m_isUnderAttack == true)
	{
		//�U���p�̃R���W�������쐬����B
		MakeAttackCollision();
	}
}

const bool Enemy::SearchPlayer() const
{
	return false;
}

void Enemy::MakeAttackCollision()
{
	
}

void Enemy::MakeFireBall()
{
	
}

void Enemy::ProcessCommonStateTransition()
{
	//�e�^�C�}�[���������B
	//�ҋ@���ԂƒǐՎ��Ԃ𐧌����邽�߁B
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//�v���C���[����������B
	if (SearchPlayer() == true)
	{
		//�����ɂ���āA�ǐՂ����邩���@�U�������邩���肷��B	
		int ram = rand() % 100;
		//������40�ȏ�Ȃ�B
		if (ram > 40)
		{
			//�ǐՃX�e�[�g�ɑJ�ڂ���B
			m_enemyState = enEnemyState_Chase;
			return;
		}
		//������40�����Ȃ�B
		else {
			//���@�U���X�e�[�g�ɑJ�ڂ���B
			m_enemyState = enEnemyState_MagicAttack;
			return;
		}
	}
}

void Enemy::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//�ҋ@���Ԃ�������x�o�߂�����B
	if (m_idleTimer >= 0.9f)
	{
		//���̃X�e�[�g�֑J�ڂ���B
		ProcessCommonStateTransition();
	}
	
}

void Enemy::ProcessWalkStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���B
	ProcessCommonStateTransition();
}

void Enemy::ProcessRunStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���B
	ProcessCommonStateTransition();
}

void Enemy::ProcessChaseStateTransition()
{
	//�U���ł��鋗���Ȃ�B
	if (IsCanAttack() == true)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
		return;
	}
	m_chaseTimer += g_gameTime->GetFrameDeltaTime();
	//�ǐՎ��Ԃ�������x�o�߂�����B
	if (m_chaseTimer >= 0.8f)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Enemy::ProcessAttackStateTransition()
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Enemy::ProcessMagicAttackStateTransition()
{
	//���@�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Enemy::ProcessReceiveDamageStateTransition()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U�����ꂽ�狗���֌W�����ɁA��芸�����ǐՂ�����B
		m_enemyState = enEnemyState_Chase;
		Vector3 diff = m_player->GetPosition() - m_position;
		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_moveSpeed = diff * 250.0f;
	}
}

void Enemy::ProcessDownStateTransition()
{
	//�_�E���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		Game* game = FindGO<Game>("game");
		//�|���ꂽ�G�l�~�[�̐���+1����B
		game->AddDefeatedEnemyNumber();
		//���g���폜����B
		DeleteGO(this);
	}
}

void Enemy::ManageState()
{
	switch (m_enemyState)
	{
	//�ҋ@�X�e�[�g�̎��B
	case enEnemyState_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessIdleStateTransition();
		break;
	//�ǐՃX�e�[�g�̎��B
	case enEnemyState_Chase:
		//�ǐՃX�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessChaseStateTransition();
		break;
	//�U���X�e�[�g�̎��B
	case enEnemyState_Attack:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessAttackStateTransition();
		break;
	//���@�U���X�e�[�g�̎��B
	case enEnemyState_MagicAttack:
		//���@�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessMagicAttackStateTransition();
		break;
	//��_���[�W�X�e�[�g�̎��B
	case enEnemyState_ReceiveDamage:
		//��_���[�W�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessReceiveDamageStateTransition();
		break;
	//�_�E���X�e�[�g�̎��B
	case enEnemyState_Down:
		//�_�E���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessDownStateTransition();
		break;
	}
}

void Enemy::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_enemyState)
	{
	//�ҋ@�X�e�[�g�̎��B
	case enEnemyState_Idle:
		//�ҋ@�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
	//�ǐՃX�e�[�g�̎��B
	case enEnemyState_Chase:
		m_modelRender.SetAnimationSpeed(1.2f);
		//����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.1f);
		break;
	//�U���X�e�[�g�̎��B
	case enEnemyState_Attack:
		m_modelRender.SetAnimationSpeed(1.6f);
		//�U���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
	//���@�U���X�e�[�g�̎��B
	case enEnemyState_MagicAttack:
		m_modelRender.SetAnimationSpeed(1.2f);
		//���@�U���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_MagicAttack, 0.1f);
		break;
	//��_���[�W�X�e�[�g�̎��B
	case enEnemyState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.3f);
		//��_���[�W�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
	//�_�E���X�e�[�g�̎��B
	case enEnemyState_Down:
		//�_�E���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	default:
		break;
	}
}

const bool Enemy::IsCanAttack() const
{

	return false;
}

void Enemy::Render(RenderContext& rc)
{
	//���f����`�悷��B
	m_modelRender.Draw(rc);
}