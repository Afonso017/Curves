#pragma once
#include "CurveAlgorithm.h"

class ChaikinAlgorithm : public CurveAlgorithm
{
public:
    ChaikinAlgorithm(Curves* app);

    void OnCreateVertex(float x, float y) override;
    void OnDelete() override;
    void OnIterate() override;

    void Display() override;
    void Save() override;
    void Load() override;
};
