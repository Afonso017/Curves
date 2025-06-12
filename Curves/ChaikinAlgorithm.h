#pragma once
#include "CurveAlgorithm.h"
#include <fstream>

class ChaikinAlgorithm : public CurveAlgorithm
{
    // estrutura para salvar a curva em arquivo
    struct CurveSave
    {
        Vertex save[MaxSize]{};
        uint saveIndex{};
    };

    Vertex aux[MaxSize]{};

public:
    ChaikinAlgorithm();
    ~ChaikinAlgorithm();

    void OnMouseMove(float x, float y) override {};
    void OnClick(float x, float y) override;
    void OnDelete() override;
    void OnIterate() override;

    bool Save() override;
    bool Load() override;
};
