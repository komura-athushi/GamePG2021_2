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
	//色々なオブジェクトを削除する。
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

	//カメラのオブジェクトを作成する。
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");
	//空のオブジェクトを作成する。
	m_skyCube = NewGO<SkyCube>(0, "skycube");


	//レベルを構築する。
	m_levelRender.Init("Assets/level3D/level.tkl", 
		[&](LevelObjectData& objData) 
		{
		//名前がhumanだったら。
		if (objData.EqualObjectName(L"human") == true) {
			//プレイヤーのオブジェクトを作成する。
			m_player = NewGO<Player>(0, "player");
			//座標を設定する。
			m_player->SetPosition(objData.position);
			//回転を設定する。
			m_player->SetRotation(objData.rotation);
			//trueにすると、レベルの方でモデルが読み込まれない。
			return true;
		}
		//名前がbackgroundだったら。
		else if (objData.EqualObjectName(L"background") == true) {
			//背景オブジェクトを作成する。
			m_background = NewGO<Background>(0, "background");
			//座標を設定する。
			m_background->SetPosition(objData.position);
			//大きさを設定する。
			m_background->SetScale(objData.scale);
			//回転を設定する。
			m_background->SetRotation(objData.rotation);
			return true;
		}
		//名前がenemyだったら。
		else if (objData.EqualObjectName(L"enemy") == true)
		{
			//エネミーのオブジェクトを作成する。
			Enemy* enemy = NewGO<Enemy>(0, "enemy");
			//座標を設定する。
			enemy->SetPosition(objData.position);
			//回転を設定する。
			enemy->SetRotation(objData.rotation);
			//HPを5に設定する。
			enemy->SetHP(5);
			//作成したエネミーの和を数えたいので、+1する。
			m_numEnemy++;
			return true;
		}
		//名前がbossだったら。
		else if (objData.EqualObjectName(L"boss") == true)
		{
			//ボスのオブジェクトを作成する。
			Boss* boss = NewGO<Boss>(0, "boss");
			//座標を設定する。
			boss->SetPosition(objData.position);
			//回転を設定する。
			boss->SetRotation(objData.rotation);
			//ボスのHPは高めに設定する。
			//HPを15に設定する。
			boss->SetHP(15);
			//作成したエネミーの和を数えたいので、+1する。
			m_numEnemy++;
			return true;
		}
		return true;
	});

	//当たり判定を有効化する。
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	//音を読み込む。
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/game.wav");
	//BGMを再生する。
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