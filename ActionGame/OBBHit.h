#pragma once
#include <array>
#include <DirectXMath.h>
#include "SimpleMath.h"

using Vector2 = DirectX::SimpleMath::Vector2;


inline std::array<Vector2, 4> ComputeOBBVertices(const Vector2& pos, float width, float height, float angle)
{
    std::array<Vector2, 4> verts;
    float halfW = width / 2.0f;
    float halfH = height / 2.0f;
    float cosA = cosf(angle);
    float sinA = sinf(angle);

    verts[0] = pos + Vector2(-halfW * cosA - halfH * sinA, -halfW * sinA + halfH * cosA);
    verts[1] = pos + Vector2(halfW * cosA - halfH * sinA, halfW * sinA + halfH * cosA);
    verts[2] = pos + Vector2(halfW * cosA + halfH * sinA, halfW * sinA - halfH * cosA);
    verts[3] = pos + Vector2(-halfW * cosA + halfH * sinA, -halfW * sinA - halfH * cosA);

    return verts;
}
