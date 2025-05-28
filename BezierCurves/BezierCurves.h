#include "DXUT.h"

struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};

class BezierCurves : public App
{
    ID3D12RootSignature* rootSignature{};
    ID3D12PipelineState* pipelineState{};
    VertexBuffer<Vertex>* vbuffer{};

    static const uint MaxSize{ 1024 };
    Vertex vertices[MaxSize]{};
    uint count{};
    uint index{};

    Vertex curve[MaxSize]{};    // vetor auxiliar para os vértices da curva
	bool chaikin;               // true para Chaikin, false para Bézier

public:
    void Init();
    void Update();
    void Display();
    void Finalize();

    void BuildRootSignature();
    void BuildPipelineState();
};
