#include "CurveAlgorithm.h"

class Curves : public App
{
    ID3D12RootSignature* rootSignature{};

    CurveAlgorithm* algorithm{};      // ponteiro para o algoritmo atual
    CurveAlgorithm** algorithms{};    // vetor de algoritmos

    float mouseX{}, mouseY{};
    float lastX{}, lastY{};

public:
    void Init();
    void Update();
    void Display();
    void Finalize();

    void BuildRootSignature();
    void BuildPipelineState();
};
