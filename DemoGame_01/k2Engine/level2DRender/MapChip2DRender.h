#pragma once

struct Level2DObjectData;
/// <summary>
/// マップチップ2D。
/// </summary>
class MapChip2DRender
{
public:
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="objData">レベル2Dデータ。</param>
	void Init(Level2DObjectData* objData);
	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update()
	{
		m_spriteRender.Update();
	}
	/// <summary>
	/// スプライトを描画する。
	/// </summary>
	/// <param name="renderContext">レンダーコンテキスト。</param>
	void Draw(RenderContext& renderContext)
	{
		m_spriteRender.Draw(renderContext);
	}
private:
	Vector3 m_position;						//座標。
	Quaternion m_rotation;					//回転。
	Vector3 m_scale = g_vec3One;			//大きさ。
	SpriteRender m_spriteRender;			//スプライト。
};

