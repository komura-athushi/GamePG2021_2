#include "stdafx.h"
#include "FireBall.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//CollisionObjectを使用したいので、ファイルをインクルードする。
#include "collision/CollisionObject.h"

//EffectEmitterを使用したいので、ファイルをインクルードする。
#include "graphics/effect/EffectEmitter.h"

FireBall::FireBall()
{

}

FireBall::~FireBall()
{
	//コリジョンオブジェクトを削除する。
	DeleteGO(m_collisionObject);
	//エフェクトの再生を停止する。
	m_effectEmitter->Stop();
	//エフェクトを削除する。
	DeleteGO(m_effectEmitter);
}

bool FireBall::Start()
{
	
	//移動速度を計算。
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);
	m_position += m_moveSpeed * 50.0f;
	m_moveSpeed *= 1200.0f;
	m_rotation.AddRotationDegY(360.0f);
	
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/fire_cut_cut.wav");
	//効果音を再生する。
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(1);
	se->Play(false);
	se->SetVolume(1.5f);

	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);
	//球状のコリジョンを作成する。
	m_collisionObject->CreateSphere(
		m_position,                     //座標。
		Quaternion::Identity,           //回転。
		35.0f                           //半径。
	);

	//術者がプレイヤーだったら。
	if (m_enMagician == enMagician_Player)
	{
		//名前をplayer_fireballにする。
		m_collisionObject->SetName("player_fireball");
	}
	//術者がエネミーだったら。
	else if (m_enMagician == enMagician_Enemy)
	{
		//名前をenemy_fireballにする。
		m_collisionObject->SetName("enemy_fireball");
	}

	//コリジョンオブジェクトが自動で削除されないようにする。
	m_collisionObject->SetIsEnableAutoDelete(false);

	//エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/effect/efk/magic_fire.efk");

	//エフェクトのオブジェクトを作成する。
	m_effectEmitter = NewGO<EffectEmitter>(0);
	m_effectEmitter->Init(0);
	//エフェクトの大きさを指定する。
	m_effectEmitter->SetScale(Vector3::One * 20.0f);
	//エフェクトの座標を設定する。
	m_effectEmitter->SetPosition(m_position);
	//エフェクトを再生する。
	m_effectEmitter->Play();
	//エフェクトの回転を設定する。
	m_effectEmitter->SetRotation(m_rotation);
	return true;
}

void FireBall::Update()
{
	//座標を移動させる。
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	//コリジョンオブジェクトに座標を設定する。
	m_collisionObject->SetPosition(m_position);
	//エフェクトに座標を設定する。
	m_effectEmitter->SetPosition(m_position);

	//タイマーを加算する。
	m_timer += g_gameTime->GetFrameDeltaTime();

	//タイマーが0.7f以上だったら。
	if (m_timer >= 0.7f)
	{
		//自身を削除する。
		DeleteGO(this);
	}
}