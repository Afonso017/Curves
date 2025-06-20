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

    // algoritmo padr�o � o de bezier
    algorithm = algorithms[1];

    // configura pipeline
    BuildRootSignature();
    BuildPipelineState();

    // envia comandos para a gpu
    graphics->SendToGpu();

    string str(""
        "C -> alterna para o algoritmo de Chaikin\n"
        "B -> alterna para o algoritmo de B�zier\n"
        "S -> salva estado da curva\n"
        "L -> carrega estado da curva\n"
        "DELETE -> apaga a curva\n"
        "ENTER -> faz uma itera��o de Chaikin"
    "");
    MessageBox(window->Id(), str.c_str(), "Controles da Aplica��o", MB_OK);
}

void Curves::Update()
{
    // sai com o pressionamento da tecla ESC
    if (input->KeyPress(VK_ESCAPE))
        window->Close();

    // atualiza posi��o do mouse
    float cx = float(window->CenterX());
    float cy = float(window->CenterY());
    float mx = float(input->MouseX());
    float my = float(input->MouseY());

    // converte as coordenadas da tela para a faixa -1.0 a 1.0
    // cy e my foram invertidos para levar em considera��o que
    // o eixo y da tela cresce na dire��o oposta do cartesiano
    mouseX = (mx - cx) / cx;
    mouseY = (cy - my) / cy;

    // verifica se o mouse mudou de posi��o
    if (lastX != mouseX || lastY != mouseY)
    {
        algorithm->OnMouseMove(mouseX, mouseY);
        lastX = mouseX;
        lastY = mouseY;

        // atualiza o desenho
        Display();
    }

    // cria v�rtices com o clique do mouse
    if (input->KeyPress(VK_LBUTTON))
    {
        algorithm->OnClick(mouseX, mouseY);
        Display();
    }

    // limpa a tela ao pressionar DELETE
    if (input->KeyPress(VK_DELETE))
    {
        algorithm->OnDelete();
        Display();
    }

    // executa uma itera��o do algoritmo ao pressionar ENTER
    if (input->KeyPress(VK_RETURN))
    {
        algorithm->OnIterate();
        Display();
    }

    // salva curva
    if (input->KeyPress('S'))
    {
        if (algorithm->Save())
        {
            Display();
            MessageBox(window->Id(), "Curva salva com sucesso!", "Salvar Curva", MB_OK);
        }
        else
        {
            MessageBox(window->Id(), "Erro ao salvar curva em arquivo", "Erro", MB_OK);
        }
        // message box bloqueia a libera��o da tecla, ent�o � necess�rio fazer manualmente
        PostMessage(window->Id(), WM_KEYUP, 'S', 0);
    }

    // carrega �ltima curva salva correspondente ao algoritmo especificado
    if (input->KeyPress('L'))
    {
        if (algorithm->Load())
        {
            algorithm->OnMouseMove(mouseX, mouseY);
            Display();
        }
        else
        {
            MessageBox(window->Id(), "Arquivo de save n�o encontrado", "Erro", MB_OK);
            // message box bloqueia a libera��o da tecla, ent�o � necess�rio fazer manualmente
            PostMessage(window->Id(), WM_KEYUP, 'L', 0);
        }
    }

    // alterna entre os algoritmos
    if (input->KeyPress('C'))
    {
        algorithm = algorithms[0];  // chaikin
        Display();
    }

    if (input->KeyPress('B'))
    {
        algorithm = algorithms[1];  // bezier
        Display();
    }
}

void Curves::Display()
{
    // copia v�rtices para o buffer da GPU usando o buffer de Upload
    graphics->PrepareGpu(pipelineState);
    algorithm->Copy();
    graphics->SendToGpu();

    // limpa backbuffer
    graphics->Clear(pipelineState);

    // submete comandos de configura��o do pipeline
    graphics->CommandList()->SetGraphicsRootSignature(rootSignature);
    graphics->CommandList()->SetPipelineState(pipelineState);
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

    // libera mem�ria alocada
    rootSignature->Release();
    pipelineState->Release();

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
        engine->window->Size(1280, 720);
        engine->window->Color(25, 25, 25);
        engine->window->Title("Curvas");
        engine->window->Icon("Icon");
        engine->window->Cursor("Cursor");
        engine->window->LostFocus(Engine::Pause);
        engine->window->InFocus(Engine::Resume);

        // cria e executa a aplica��o
        engine->Start(new Curves());

        // finaliza execu��o
        delete engine;
    }
    catch (Error & e)
    {
        // exibe mensagem em caso de erro
        MessageBox(nullptr, e.ToString().data(), "Curves", MB_OK);
    }

    return 0;
}
