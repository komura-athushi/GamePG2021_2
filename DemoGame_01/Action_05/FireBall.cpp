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
	return true;
}

void FireBall::Update()
{
	


	//タイマーを加算する。
	m_timer += g_gameTime->GetFrameDeltaTime();

	//タイマーが0.7f以上だったら。
	if (m_timer >= 0.7f)
	{
		//自身を削除する。
		DeleteGO(this);
	}
}