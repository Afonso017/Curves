#pragma once
#include "CurveAlgorithm.h"

class ChaikinAlgorithm : public CurveAlgorithm
{
    Vertex aux[MaxSize]{};

public:
    ChaikinAlgorithm();
    ~ChaikinAlgorithm();

    void OnCreateVertex(float x, float y) override;
    void OnDelete() override;
    void OnIterate() override;

    void Save() override;
    void Load() override;
};
