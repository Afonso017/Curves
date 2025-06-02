#pragma once

class Curves;

// representa um algoritmo de desenho de curvas para a aplicação
class CurveAlgorithm
{
protected:
    Curves* app;

public:
    CurveAlgorithm(Curves* app) : app(app) {}
    virtual ~CurveAlgorithm() = default;

    virtual void OnCreateVertex(float x, float y) = 0;
    virtual void OnDelete() = 0;
    virtual void OnIterate() = 0;

    virtual void Display() = 0;         // exibe a curva na tela
    virtual void Save() = 0;            // salva estado do algoritmo
    virtual void Load() = 0;            // carrega último estado salvo
};
