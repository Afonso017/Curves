#include "BezierCurves.h"

void BezierCurves::Init()
{
    // cria vertex buffer
    vbuffer = new VertexBuffer<Vertex>(nullptr, MaxSize);

    // configura pipeline
    BuildRootSignature();
    BuildPipelineState();

    // envia comandos para a gpu
    graphics->SendToGpu();
}

void BezierCurves::Update()
{
    // sai com o pressionamento da tecla ESC
    if (input->KeyPress(VK_ESCAPE))
        window->Close();

    // cria vértices com o clique do mouse
    if (input->KeyPress(VK_LBUTTON))
    {
        float cx = float(window->CenterX());
        float cy = float(window->CenterY());
        float mx = float(input->MouseX());
        float my = float(input->MouseY());
        
        // converte as coordenadas da tela para a faixa -1.0 a 1.0
        // cy e my foram invertidos para levar em consideração que
        // o eixo y da tela cresce na direção oposta do cartesiano
        float x = (mx - cx) / cx;
        float y = (cy - my) / cy;

        // muda a cor dos novos vértices para amarelo caso existam mais de 14
        vertices[index] = { XMFLOAT3(x, y, 0.f), (count >= 15 ? XMFLOAT4(Colors::Yellow) : XMFLOAT4(Colors::White)) };
        index = (index + 1) % MaxSize;
        
        if (count < MaxSize)
            ++count;

        // muda a cor dos 15 primeiros vértices para amarelo
        if (count == 15)
            for (uint i = 0; i < 15; ++i)
                vertices[i].Color = XMFLOAT4(Colors::Yellow);

        // copia vértices para o buffer da GPU usando o buffer de Upload
        graphics->PrepareGpu(pipelineState);
        vbuffer->Copy(vertices, count);
        graphics->SendToGpu();

        // atualiza o desenho
        Display();
    }

    // "limpa" o vetor de vértices ao pressionar DELETE
    if (input->KeyPress(VK_DELETE))
    {
        index = count = 0;
        Display();
    }

    // executa uma iteração do algoritmo de chaikin ao pressionar ENTER
    if (input->KeyPress(VK_RETURN))
    {
        if (2 * count - 2 < MaxSize)
        {
            index = 0;
            for (uint i = 1; i < count; ++i)
            {
                Vertex p0 = vertices[i - 1];
                Vertex p1 = vertices[i];

                // calcula coordenadas do vetor distância (p1 - p0)
                float dx = p1.Pos.x - p0.Pos.x;
                float dy = p1.Pos.y - p0.Pos.y;

                // cria dois novos vértices para essa iteração e os adiciona ao vetor curve
                curve[index++] = { XMFLOAT3(p0.Pos.x + 0.25f * dx, p0.Pos.y + 0.25f * dy, 0.f), XMFLOAT4(Colors::Green) };
                curve[index++] = { XMFLOAT3(p0.Pos.x + 0.75f * dx, p0.Pos.y + 0.75f * dy, 0.f), XMFLOAT4(Colors::Green) };
            }

            // copia os vértices de curve para o vetor vertices
            for (uint i = 0; i < index; ++i)
                vertices[i] = curve[i];

            count = index;

            // copia vértices para o buffer da GPU usando o buffer de Upload
            graphics->PrepareGpu(pipelineState);
            vbuffer->Copy(vertices, count);
            graphics->SendToGpu();

            // atualiza o desenho
            Display();
        }
        else
        {
            // muda a cor dos vértices de volta para branco
            for (Vertex& v : vertices)
                v.Color = XMFLOAT4(Colors::White);

            // copia vértices para o buffer da GPU usando o buffer de Upload
            graphics->PrepareGpu(pipelineState);
            vbuffer->Copy(vertices, count);
            graphics->SendToGpu();

            // atualiza o desenho
            Display();
        }
    }
}

void BezierCurves::Display()
{
    // limpa backbuffer
    graphics->Clear(pipelineState);

    // submete comandos de configuração do pipeline
    graphics->CommandList()->SetGraphicsRootSignature(rootSignature);
    graphics->CommandList()->IASetVertexBuffers(0, 1, vbuffer->View());
    graphics->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

    // submete comandos de desenho
    graphics->CommandList()->DrawInstanced(count, 1, 0, 0);

    // apresenta backbuffer
    graphics->Present();
}

void BezierCurves::Finalize()
{
    // espera GPU finalizar comandos pendentes
    graphics->WaitForGpu();

    // libera memória alocada
    rootSignature->Release();
    pipelineState->Release();
    delete vbuffer;
}

// ------------------------------------------------------------------------------
//                                  WinMain                                      
// ------------------------------------------------------------------------------

int APIENTRY WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow
) {
    try
    {
        // cria motor e configura a janela
        Engine* engine = new Engine();
        engine->window->Mode(ASPECTRATIO);
        engine->window->Size(1024, 600);
        engine->window->Color(25, 25, 25);
        engine->window->Title("Curvas de Bézier");
        engine->window->Icon("Icon");
        engine->window->Cursor("Cursor");
        engine->window->LostFocus(Engine::Pause);
        engine->window->InFocus(Engine::Resume);

        // cria e executa a aplicação
        engine->Start(new BezierCurves());

        // finaliza execução
        delete engine;
    }
    catch (Error & e)
    {
        // exibe mensagem em caso de erro
        MessageBox(nullptr, e.ToString().data(), "Bézier Curves", MB_OK);
    }

    return 0;
}
