#include "k2EnginePreCompile.h"
#include "SkyCube.h"

SkyCube::SkyCube()
{

}

SkyCube::~SkyCube()
{

}

bool SkyCube::Start()
{
	ModelInitData initData;
	//tkmファイルのファイルパスを指定する。
	initData.m_tkmFilePath = "Assets/modelData/preset/sky.tkm";
	//シェーダーファイルのファイルパスを指定する。
	initData.m_fxFilePath = "Assets/shader/SkyCubeMap.fx";
	initData.m_vsEntryPointFunc = "VSMain";
	initData.m_psEntryPointFunc = "PSMain";
	m_texture.InitFromDDSFile(L"Assets/modelData/preset/skyCubeMap.dds");


	initData.m_expandShaderResoruceView[0] = &m_texture;
	m_modelRender.InitForwardRendering(initData);
	m_modelRender.SetShadowCasterFlag(false);
	m_modelRender.SetTRS(m_position, g_quatIdentity, m_scale);
	m_modelRender.Update();
	return true;
}

void SkyCube::Update()
{
	if (m_isDirty)
	{
		m_modelRender.SetTRS(m_position, g_quatIdentity, m_scale);
		m_modelRender.Update();
		m_isDirty = false;
	}
}

void SkyCube::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}