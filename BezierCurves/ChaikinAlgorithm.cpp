#include "ChaikinAlgorithm.h"
#include "Curves.h"

ChaikinAlgorithm::ChaikinAlgorithm(Curves* app) : CurveAlgorithm(app) {}

void ChaikinAlgorithm::OnCreateVertex(float x, float y)
{
    // muda a cor dos novos vértices para amarelo caso existam mais de 14
    app->vertices[app->index] = { XMFLOAT3(x, y, 0.f), (app->count >= 15 ? XMFLOAT4(Colors::Yellow) : XMFLOAT4(Colors::White)) };
    app->index = (app->index + 1) % app->MaxSize;

    if (app->count < app->MaxSize)
        ++app->count;

    // muda a cor dos 15 primeiros vértices para amarelo
    if (app->count == 15)
        for (uint i = 0; i < 15; ++i)
            app->vertices[i].Color = XMFLOAT4(Colors::Yellow);

    // copia vértices para o buffer da GPU usando o buffer de Upload
    app->graphics->PrepareGpu(app->pipelineState);
    app->vbuffer->Copy(app->vertices, app->count);
    app->graphics->SendToGpu();

    // atualiza o desenho
    Display();
}

void ChaikinAlgorithm::OnDelete()
{
    app->index = app->count = 0;
    Display();
}

void ChaikinAlgorithm::OnIterate()
{
    if (2 * app->count - 2 < app->MaxSize)
    {
        app->index = 0;
        for (uint i = 1; i < app->count; ++i)
        {
            Vertex p0 = app->vertices[i - 1];
            Vertex p1 = app->vertices[i];

            // calcula coordenadas do vetor distância (p1 - p0)
            float dy = p1.Pos.y - p0.Pos.y;
            float dx = p1.Pos.x - p0.Pos.x;

            // cria dois novos vértices para essa iteração e os adiciona ao vetor curve
            app->curve[app->index++] = { XMFLOAT3(p0.Pos.x + 0.25f * dx, p0.Pos.y + 0.25f * dy, 0.f), XMFLOAT4(Colors::Green) };
            app->curve[app->index++] = { XMFLOAT3(p0.Pos.x + 0.75f * dx, p0.Pos.y + 0.75f * dy, 0.f), XMFLOAT4(Colors::Green) };
        }

        // copia os vértices de curve para o vetor vertices
        for (uint i = 0; i < app->index; ++i)
            app->vertices[i] = app->curve[i];

        app->count = app->index;

        // copia vértices para o buffer da GPU usando o buffer de Upload
        app->graphics->PrepareGpu(app->pipelineState);
        app->vbuffer->Copy(app->vertices, app->count);
        app->graphics->SendToGpu();

        // atualiza o desenho
        Display();
    }
    else
    {
        // muda a cor dos vértices de volta para branco
        for (Vertex& v : app->vertices)
            v.Color = XMFLOAT4(Colors::White);

        // copia vértices para o buffer da GPU usando o buffer de Upload
        app->graphics->PrepareGpu(app->pipelineState);
        app->vbuffer->Copy(app->vertices, app->count);
        app->graphics->SendToGpu();

        // atualiza o desenho
        Display();
    }
}

void ChaikinAlgorithm::Display()
{
    // limpa backbuffer
    app->graphics->Clear(app->pipelineState);

    // submete comandos de configuração do pipeline
    app->graphics->CommandList()->SetGraphicsRootSignature(app->rootSignature);
    app->graphics->CommandList()->SetPipelineState(app->pipelineState);
    app->graphics->CommandList()->IASetVertexBuffers(0, 1, app->vbuffer->View());
    app->graphics->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

    // submete comandos de desenho
    app->graphics->CommandList()->DrawInstanced(app->count, 1, 0, 0);

    // apresenta backbuffer
    app->graphics->Present();
}

void ChaikinAlgorithm::Save()
{

}

void ChaikinAlgorithm::Load()
{

}
