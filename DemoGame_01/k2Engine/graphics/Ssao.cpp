#include "k2EnginePreCompile.h"
#include "Ssao.h"
#include "RenderingEngine.h"

void Ssao::Init(RenderTarget& mainRenderTarget)
{
    float color[4] =
    {
        0.0f,0.0f,0.0f,0.0f
    };

    m_ssaoRenderTarget.Create(
        mainRenderTarget.GetWidth(),
        mainRenderTarget.GetHeight(),
        1,
        1,
        DXGI_FORMAT_R32G32B32A32_FLOAT,
        DXGI_FORMAT_UNKNOWN,
        color
    );

    // 最終合成用のスプライトを初期化する
    SpriteInitData spriteInitData;
    spriteInitData.m_textures[0] = &g_renderingEngine->GetZPrepassDepthTexture();
    spriteInitData.m_textures[1] = &g_renderingEngine->GetGBufferNormalTexture();
    // 解像度はmainRenderTargetの幅と高さ
    spriteInitData.m_width = mainRenderTarget.GetWidth();
    spriteInitData.m_height = mainRenderTarget.GetHeight();
    // 2D用のシェーダーを使用する
    spriteInitData.m_fxFilePath = "Assets/shader/ssao.fx";
    spriteInitData.m_vsEntryPointFunc = "VSMain";
    spriteInitData.m_psEntryPoinFunc = "PSMain";

    spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
    spriteInitData.m_colorBufferFormat[0] = m_ssaoRenderTarget.GetColorBufferFormat();

    //解像度をGPUに送るための定数バッファを設定する。
    spriteInitData.m_expandConstantBuffer = (void*)&m_cB;
    spriteInitData.m_expandConstantBufferSize = sizeof(SsaoBuffer) +
        (16 - (sizeof(SsaoBuffer) % 16));
    m_ssaoSprite.Init(spriteInitData);


    spriteInitData.m_textures[0] = &m_ssaoRenderTarget.GetRenderTargetTexture();
    // 2D用のシェーダーを使用する
    spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
    spriteInitData.m_vsEntryPointFunc = "VSMain";
    spriteInitData.m_psEntryPoinFunc = "PSMain";
    spriteInitData.m_alphaBlendMode = AlphaBlendMode_Multiply;
    spriteInitData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();

    m_finalSprite.Init(spriteInitData);
}

void Ssao::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
{
    //ssaoレンダ―ターゲットを。
    //PRESENTからRENDERTARGETへ。
    rc.WaitUntilToPossibleSetRenderTarget(m_ssaoRenderTarget);
    // レンダリングターゲットを設定
    rc.SetRenderTargetAndViewport(m_ssaoRenderTarget);
    
    //定数バッファーを更新。
    //ビュー行列。
    m_cB.view = g_camera3D->GetViewMatrix();
    //プロジェクション行列。
    m_cB.proj = g_camera3D->GetProjectionMatrix();
    //プロジェクション行列の逆行列。
    m_cB.invproj = g_camera3D->GetProjectionMatrixInv();

    //描画。
    m_ssaoSprite.Draw(rc);

    //レンダ―ターゲットを。
    //メインレンダ―ターゲットに設定。
    rc.SetRenderTarget(mainRenderTarget);

    //ssaoレンダ―ターゲットを。
    //RENDERTARGETからPRESENTへ。
    rc.WaitUntilFinishDrawingToRenderTarget(m_ssaoRenderTarget);

    //メインレンダ―ターゲットを。
    //PRESENTからRENDERTARGETへ。
    rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);

    //描画。
    m_finalSprite.Draw(rc);

    rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
}
