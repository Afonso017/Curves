#pragma once
#include "DXUT.h"

struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};

// representa um algoritmo de desenho de curvas para a aplica��o
class CurveAlgorithm
{
public:
    D3D_PRIMITIVE_TOPOLOGY topology{};

    // vertex buffer e vetor de v�rtices
    static const uint MaxSize{ 2040 };
    VertexBuffer<Vertex>* vbuffer{};
    Vertex vertices[MaxSize]{};
    uint count{};
    uint index{};

    virtual ~CurveAlgorithm() = default;

    virtual void OnMouseMove(float x, float y) = 0;
    virtual void OnClick(float x, float y) = 0;
    virtual void OnDelete() = 0;
    virtual void OnIterate() = 0;

    // copia v�rtices para o buffer da GPU
    void Copy() 
    {
        vbuffer->Copy(vertices, count);
    };

    virtual bool Save() = 0;    // salva estado do algoritmo
    virtual bool Load() = 0;    // carrega �ltimo estado salvo
};
