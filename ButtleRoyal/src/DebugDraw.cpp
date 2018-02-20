#include "stdafx.h"
#include "DebugDraw.h"
#include "VectorAlgebra2D.h"
#include "InputManager.h"


DebugDraw::DebugDraw() :
	m_enabled(true)
{
	if (!m_font.loadFromFile("../assets/consolab.ttf"))
	{
		err() << "Could not load font\n";
	}
};

void DebugDraw::DrawLine(const Vector2f& p1, const Vector2f& p2, const Color& color, const float thickness)
{
	if (!m_enabled)
	{
		return;
	}

	Vector2f dir = p2 - p1;
	const float length = MathUtil::length(dir);

	if (length >= 1.0f)
	{
		dir = (dir / length) * thickness * 0.5f;
		Vector2f right(dir.y, -dir.x);

		std::shared_ptr<ConvexShape> shape(new ConvexShape());
		shape->setPointCount(4);
		shape->setPoint(0, p1 - right);
		shape->setPoint(1, p1 + right);
		shape->setPoint(2, p2 + right);
		shape->setPoint(3, p2 - right);
		shape->setFillColor(color);

		m_shapes.push_back(shape);
	}
}

void DebugDraw::DrawArrow(const Vector2f& origin, const Vector2f& direction, const Color& color,
                          const float thickness)
{
	if (!m_enabled)
	{
		return;
	}

	const float length = MathUtil::length(direction);

	if (length >= 1.0f)
	{
		Vector2f unitDirection = direction / length;
		Vector2f right(unitDirection.y, -unitDirection.x);

		const float HEAD_SIZE = 5.0f;
		float head = length > (HEAD_SIZE * 2) ? length - HEAD_SIZE : length * 0.5f;

		Vector2f p1 = origin;
		Vector2f p2 = origin + unitDirection * head;
		Vector2f p3 = origin + direction;

		std::shared_ptr<ConvexShape> shape(new ConvexShape());
		shape->setPointCount(4);

		shape->setPoint(0, p1 - right * thickness * 0.5f);
		shape->setPoint(1, p1 + right * thickness * 0.5f);
		shape->setPoint(2, p2 + right * thickness * 0.5f);
		shape->setPoint(3, p2 - right * thickness * 0.5f);
		shape->setFillColor(color);

		m_shapes.push_back(shape);

		std::shared_ptr<ConvexShape> headShape(new ConvexShape());
		headShape->setPointCount(3);

		headShape->setPoint(0, p2 - right * HEAD_SIZE);
		headShape->setPoint(1, p2 + right * HEAD_SIZE);
		headShape->setPoint(2, p3);
		headShape->setFillColor(color);

		m_shapes.push_back(headShape);
	}
}

void DebugDraw::DrawCircle(const Vector2f& center, float radius, const Color& outlineColor, const Color& fillColor)
{
	if (!m_enabled)
	{
		return;
	}

	std::shared_ptr<CircleShape> shape(new CircleShape(radius));
	shape->setOutlineThickness(1.0f);
	shape->setOutlineColor(outlineColor);
	shape->setFillColor(fillColor);
	shape->setPosition(center);
	shape->setOrigin(Vector2f(radius, radius));

	m_shapes.push_back(shape);
}

void DebugDraw::DrawRectangle(const Vector2f& position, const Vector2f& size, const Color& outlineColor,
                              const Color& fillColor)
{
	if (!m_enabled)
	{
		return;
	}

	std::shared_ptr<RectangleShape> shape(new RectangleShape(size));
	shape->setOutlineThickness(1.0f);
	shape->setOutlineColor(outlineColor);
	shape->setFillColor(fillColor);
	shape->setPosition(position);
	shape->setOrigin(size * 0.5f);

	m_shapes.push_back(shape);
}

void DebugDraw::DrawText(const std::string& str, const Vector2f& center, const Color& color)
{
	std::shared_ptr<Text> text(new Text());
	text->setPosition(center);
	text->setString(str);
	text->setFillColor(color);
	text->setFont(m_font);
	text->setCharacterSize(14);
	auto bounds = text->getLocalBounds();
	text->setOrigin(
		bounds.width * 0.5f,
		bounds.height * 0.5f + bounds.top);

	m_shapes.push_back(text);
}

void DebugDraw::Update(float fDeltaTime)
{
	if (InputManager::getInstance().isKeyPressed("debug", 2))
	{
		SetEnabled(!IsEnabled());
	}
}

void DebugDraw::Draw(RenderWindow& renderWindow)
{
	if (m_enabled)
	{
		for (const auto& shape : m_shapes)
		{
			renderWindow.draw(*shape);
		}
	}

	m_shapes.clear();
}
