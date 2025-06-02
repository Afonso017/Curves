#include "DXUT.h"
#include "CurveAlgorithm.h"

class ChaikinAlgorithm;
class BezierAlgorithm;

struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};

class Curves : public App
{
    ID3D12RootSignature* rootSignature{};
    ID3D12PipelineState* pipelineState{};
    VertexBuffer<Vertex>* vbuffer{};

    static const uint MaxSize{ 1024 };
    Vertex vertices[MaxSize]{};
    uint count{};
    uint index{};

    Vertex curve[MaxSize]{};          // vetor auxiliar para os vértices da curva
    CurveAlgorithm* algorithm{};      // ponteiro para o algoritmo atual
    CurveAlgorithm** algorithms{};    // vetor de algoritmos

public:
    void Init();
    void Update();
    void Display();
    void Finalize();

    void BuildRootSignature();
    void BuildPipelineState();

    // compartilha dados da aplicação com os algoritmos
    friend class ChaikinAlgorithm;
    friend class BezierAlgorithm;
};
