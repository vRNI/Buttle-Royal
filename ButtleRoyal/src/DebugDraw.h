#pragma once

/// Simple debug draw functionality.
/// Use the relevant Draw* methode to render debug stuff in the current frame
class DebugDraw
{
public:
	static DebugDraw& getInstance()
	{
		static DebugDraw instance;
		return instance;
	}

	void DrawLine(const Vector2f& p1, const Vector2f& p2, const Color& color, float thickness = 1.0f);
	void DrawArrow(const Vector2f& origin, const Vector2f& direction, const Color& color, float thickness = 1.0f);
	void DrawCircle(const Vector2f& center, float radius, const Color& outlineColor, const Color& fillColor = Color::Transparent);
	void DrawText(const std::string& text, const Vector2f& center, const Color& color);
	void DrawRectangle(const Vector2f& p1, const Vector2f& p2, const Color& outlineColor, const Color& fillColor = Color::Transparent);


	bool IsEnabled() const
	{
		return m_enabled;
	}

	void SetEnabled(const bool enabled)
	{
		m_enabled = enabled;
	}

	void Update(float fDeltaTime);

	void Draw(RenderWindow& renderWindow);

private:
	DebugDraw();

	DebugDraw(DebugDraw const&): m_enabled(true){} ;
	void operator=(DebugDraw const&) const {};
	~DebugDraw() {};

	bool m_enabled;
	std::vector<std::shared_ptr<Drawable>> m_shapes;
	Font m_font;
};