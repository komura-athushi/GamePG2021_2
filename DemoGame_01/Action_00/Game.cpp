#include "stdafx.h"
#include "Game.h"

#include "Player.h"
#include "GameCamera.h"
#include "Background.h"
#include "Enemy.h"
#include "Lever.h"
#include "Door.h"
#include "Boss.h"
#include "Fade.h"
#include "Title.h"

#include "nature/SkyCube.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

Game::Game()
{

}

Game::~Game()
{
	//�F�X�ȃI�u�W�F�N�g���폜����B
	DeleteGO(m_gameCamera);
	DeleteGO(m_skyCube);
	DeleteGO(m_player);
	DeleteGO(m_background);
	DeleteGO(m_bgm);
}

bool Game::Start()
{
	g_camera3D->SetPosition({ 0.0f, 100.0f, -600.0f });
	g_camera3D->SetFar(40000.0f);

	//�J�����̃I�u�W�F�N�g���쐬����B
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");
	//��̃I�u�W�F�N�g���쐬����B
	m_skyCube = NewGO<SkyCube>(0, "skycube");


	//���x�����\�z����B
	m_levelRender.Init("Assets/level3D/level.tkl", 
		[&](LevelObjectData& objData) 
		{
		//���O��human��������B
		if (objData.EqualObjectName(L"human") == true) {
			//�v���C���[�̃I�u�W�F�N�g���쐬����B
			m_player = NewGO<Player>(0, "player");
			//���W��ݒ肷��B
			m_player->SetPosition(objData.position);
			//��]��ݒ肷��B
			m_player->SetRotation(objData.rotation);
			//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
			return true;
		}
		//���O��background��������B
		else if (objData.EqualObjectName(L"background") == true) {
			//�w�i�I�u�W�F�N�g���쐬����B
			m_background = NewGO<Background>(0, "background");
			//���W��ݒ肷��B
			m_background->SetPosition(objData.position);
			//�傫����ݒ肷��B
			m_background->SetScale(objData.scale);
			//��]��ݒ肷��B
			m_background->SetRotation(objData.rotation);
			return true;
		}
		return true;
	});

	//�����蔻���L��������B
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	//����ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/game.wav");
	//BGM���Đ�����B
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(2);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.2f);

	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();
	return true;
}

void Game::NotifyGameClear()
{
	m_isWaitFadeout = true;
	m_fade->StartFadeOut();
}

void Game::NotifyGameOver()
{
	m_isWaitFadeout = true;
	m_fade->StartFadeOut();
}

void Game::Update()
{
	if (m_isWaitFadeout) {
		if (!m_fade->IsFade()) {
			NewGO<Title>(0, "title");
			DeleteGO(this);
		}
	}
}