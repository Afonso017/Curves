#pragma once
#include "CurveAlgorithm.h"

class BezierAlgorithm : public CurveAlgorithm
{
    uint numClicks{};
    const uint numSupport{ 20 };    // quantidade de vértices de apoio gerados a cada clique
	const uint lineSegs{ 50 };      // quantidade de segmentos de linha da curva

public:
    BezierAlgorithm();
    ~BezierAlgorithm();

    void OnMouseMove(float x, float y) override;
    void OnClick(float x, float y) override;
    void OnDelete() override;
    void OnIterate() override;

    void GenerateSupportVertices(float x, float y);
    void DrawBezier();
    void Save() override;
    void Load() override;
};
