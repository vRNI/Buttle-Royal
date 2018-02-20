#include "stdafx.h"

#include "TransformAnimationConstantMotion.h"

void
TransformAnimationConstantMotion::VUpdate(float fDeltaTime, 
										  Transform& transform)
{
	transform.translate(m_direction * m_fSpeed * fDeltaTime);
}