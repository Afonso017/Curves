#pragma once
#include "DXUT.h"

struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};

// representa um algoritmo de desenho de curvas para a aplicação
class CurveAlgorithm
{
public:
    // cada algoritmo possui pso e topologia diferente
    ID3D12PipelineState* pipelineState{};
    D3D_PRIMITIVE_TOPOLOGY topology{};

    // vertex buffer e vetor de vértices
    static const uint MaxSize{ 1024 };
    VertexBuffer<Vertex>* vbuffer{};
    Vertex vertices[MaxSize]{};
    uint count{};
    uint index{};

    virtual ~CurveAlgorithm() = default;

    virtual void OnMouseMove(float x, float y) = 0;
    virtual void OnClick(float x, float y) = 0;
    virtual void OnDelete() = 0;
    virtual void OnIterate() = 0;

    // copia vértices para o buffer da GPU
    void Copy() 
    {
        vbuffer->Copy(vertices, count);
    };

    virtual void Save() = 0;    // salva estado do algoritmo
    virtual void Load() = 0;    // carrega último estado salvo
};
