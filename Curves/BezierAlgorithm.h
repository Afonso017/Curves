#pragma once
#include "CurveAlgorithm.h"

class BezierAlgorithm : public CurveAlgorithm
{
    uint numClicks{};
    uint numClicksSave{};
    uint offset{ 0 };           // porção dos vértices de apoio no vetor
	const uint lineSegs{ 50 };  // quantidade de segmentos de linha da curva

public:
    BezierAlgorithm();
    ~BezierAlgorithm();

    void OnMouseMove(float x, float y) override;
    void OnClick(float x, float y) override;
    void OnDelete() override;
    void OnIterate() override;

    void GenerateSupportVertices(float x, float y);
    void MoveSupportVertices(float x, float y);
    void DrawBezier();
    void Save() override;
    void Load() override;
};
