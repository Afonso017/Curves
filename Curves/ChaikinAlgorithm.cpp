#include "ChaikinAlgorithm.h"
#include "Curves.h"

ChaikinAlgorithm::ChaikinAlgorithm()
{
    topology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;

    // aloca vertex buffer
    vbuffer = new VertexBuffer<Vertex>(nullptr, MaxSize);
}

ChaikinAlgorithm::~ChaikinAlgorithm()
{
    pipelineState->Release();
    delete vbuffer;
}

void ChaikinAlgorithm::OnClick(float x, float y)
{
    // muda a cor dos novos vértices para amarelo caso existam mais de 14
    vertices[index] = { XMFLOAT3(x, y, 0.f), (count >= 15 ? XMFLOAT4(Colors::Yellow) : XMFLOAT4(Colors::White)) };
    index = (index + 1) % MaxSize;

    if (count < MaxSize)
        ++count;

    // muda a cor dos 15 primeiros vértices para amarelo
    if (count == 15)
        for (uint i = 0; i < 15; ++i)
            vertices[i].Color = XMFLOAT4(Colors::Yellow);
}

void ChaikinAlgorithm::OnDelete()
{
    index = count = 0;
}

void ChaikinAlgorithm::OnIterate()
{
    // verifica se a iteração vai gerar um número de vértices menor que o limite
    if (2 * count - 2 < MaxSize)
    {
        index = 0;
        for (uint i = 1; i < count; ++i)
        {
            Vertex p0 = vertices[i - 1];
            Vertex p1 = vertices[i];

            // calcula coordenadas do vetor distância (p1 - p0)
            float dy = p1.Pos.y - p0.Pos.y;
            float dx = p1.Pos.x - p0.Pos.x;

            // cria dois novos vértices para essa iteração e os adiciona ao vetor curve
            aux[index++] = { XMFLOAT3(p0.Pos.x + 0.25f * dx, p0.Pos.y + 0.25f * dy, 0.f), XMFLOAT4(Colors::Green) };
            aux[index++] = { XMFLOAT3(p0.Pos.x + 0.75f * dx, p0.Pos.y + 0.75f * dy, 0.f), XMFLOAT4(Colors::Green) };
        }

        // copia os vértices de aux para o vetor vertices
        for (uint i = 0; i < index; ++i)
            vertices[i] = aux[i];

        count = index;
    }
    else
    {
        // muda a cor dos vértices de volta para branco
        for (Vertex& v : vertices)
            v.Color = XMFLOAT4(Colors::White);
    }
}

void ChaikinAlgorithm::Save()
{

}

void ChaikinAlgorithm::Load()
{

}
