#pragma once
#include "CurveAlgorithm.h"
#include <fstream>

class BezierAlgorithm : public CurveAlgorithm
{
    // estrutura para salvar a curva em arquivo
    struct CurveSave
    {
        Vertex save[MaxSize]{};
        uint saveIndex{};
        uint numClicksSave{};
        uint offsetSave{};
    };

    uint numClicks{};
    uint offset{ 0 };           // porção dos vértices de apoio no vetor
	const uint lineSegs{ 50 };  // quantidade de segmentos de linha da curva
    bool showSupport{ true };

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
    bool Save() override;
    bool Load() override;
};
