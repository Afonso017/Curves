#include "Curves.h"
#include "ChaikinAlgorithm.h"
#include "BezierAlgorithm.h"

void Curves::Init()
{
    // inicializa vetor de ponteiros de algoritmos
    algorithms = new CurveAlgorithm*[2]
    {
        new ChaikinAlgorithm(),
        new BezierAlgorithm()
    };

    // algoritmo padrão é o de chaikin
    algorithm = algorithms[1];

    // configura pipeline
    BuildRootSignature();
    BuildPipelineState();

    // envia comandos para a gpu
    graphics->SendToGpu();
}

void Curves::Update()
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

        algorithm->OnCreateVertex(x, y);
        Display();
    }

    // limpa a tela ao pressionar DELETE
    if (input->KeyPress(VK_DELETE))
    {
        algorithm->OnDelete();

        // atualiza o desenho
        Display();
    }

    // executa uma iteração do algoritmo ao pressionar ENTER
    if (input->KeyPress(VK_RETURN))
    {
        algorithm->OnIterate();

        // atualiza o desenho
        Display();
    }

    // salva curva
    if (input->KeyPress('S'))
        algorithm->Save();

    // carrega última curva salva correspondente ao algoritmo especificado
    if (input->KeyPress('L'))
    {
        algorithm->Load();

        // atualiza o desenho
        Display();
    }

    // alterna entre os algoritmos
    if (input->KeyPress('C'))
        algorithm = algorithms[0];  // chaikin

    if (input->KeyPress('B'))
        algorithm = algorithms[1];  // bezier
}

void Curves::Display()
{
    // copia vértices para o buffer da GPU usando o buffer de Upload
    graphics->PrepareGpu(algorithm->pipelineState);
    algorithm->Copy();
    graphics->SendToGpu();

    // limpa backbuffer
    graphics->Clear(algorithm->pipelineState);

    // submete comandos de configuração do pipeline
    graphics->CommandList()->SetGraphicsRootSignature(rootSignature);
    graphics->CommandList()->SetPipelineState(algorithm->pipelineState);
    graphics->CommandList()->IASetVertexBuffers(0, 1, algorithm->vbuffer->View());
    graphics->CommandList()->IASetPrimitiveTopology(algorithm->topology);

    // submete comandos de desenho
    graphics->CommandList()->DrawInstanced(algorithm->count, 1, 0, 0);

    // apresenta backbuffer
    graphics->Present();
}

void Curves::Finalize()
{
    // espera GPU finalizar comandos pendentes
    graphics->WaitForGpu();

    // libera memória alocada
    rootSignature->Release();

    delete algorithms[1];
    delete algorithms[0];
    delete algorithms;
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
        engine->window->Title("Curvas");
        engine->window->Icon("Icon");
        engine->window->Cursor("Cursor");
        engine->window->LostFocus(Engine::Pause);
        engine->window->InFocus(Engine::Resume);

        // cria e executa a aplicação
        engine->Start(new Curves());

        // finaliza execução
        delete engine;
    }
    catch (Error & e)
    {
        // exibe mensagem em caso de erro
        MessageBox(nullptr, e.ToString().data(), "Curves", MB_OK);
    }

    return 0;
}
