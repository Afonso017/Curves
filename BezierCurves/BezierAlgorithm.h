#pragma once
#include "CurveAlgorithm.h"

class BezierAlgorithm : public CurveAlgorithm
{
public:
    BezierAlgorithm(Curves* app);

    void OnCreateVertex(float x, float y) override;
    void OnDelete() override;
    void OnIterate() override;

    void Display() override;
    void Save() override;
    void Load() override;
};
