#pragma once
#include "CurveAlgorithm.h"

class BezierAlgorithm : public CurveAlgorithm
{
    uint numClicks{};

public:
    BezierAlgorithm();
    ~BezierAlgorithm();

    void OnCreateVertex(float x, float y) override;
    void OnDelete() override;
    void OnIterate() override;

    void Save() override;
    void Load() override;
};
