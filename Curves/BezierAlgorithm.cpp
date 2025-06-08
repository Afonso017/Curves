#include "BezierAlgorithm.h"
#include "Curves.h"

// vértices dos quadrados de apoio
const Vertex quadAux[8]
{
	{ XMFLOAT3(-0.01f, -0.015f, 0.f), XMFLOAT4(Colors::DarkRed) },	// v0
	{ XMFLOAT3(0.01f, -0.015f, 0.f), XMFLOAT4(Colors::DarkRed) },	// v1
	{ XMFLOAT3(0.01f, -0.015f, 0.f), XMFLOAT4(Colors::DarkRed) },	// v1
	{ XMFLOAT3(0.01f, 0.015f, 0.f), XMFLOAT4(Colors::DarkRed) },	// v2
	{ XMFLOAT3(0.01f, 0.015f, 0.f), XMFLOAT4(Colors::DarkRed) },	// v2
	{ XMFLOAT3(-0.01f, 0.015f, 0.f), XMFLOAT4(Colors::DarkRed) },	// v3
	{ XMFLOAT3(-0.01f, 0.015f, 0.f), XMFLOAT4(Colors::DarkRed) },	// v3
	{ XMFLOAT3(-0.01f, -0.015f, 0.f), XMFLOAT4(Colors::DarkRed) }	// v0
};

BezierAlgorithm::BezierAlgorithm()
{
  	topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;

	// aloca vertex buffer
	vbuffer = new VertexBuffer<Vertex>(nullptr, MaxSize);
}

BezierAlgorithm::~BezierAlgorithm()
{
	pipelineState->Release();
	delete vbuffer;
}

void BezierAlgorithm::OnMouseMove(float x, float y)
{
	// verifica quantos cliques até agora
	if (numClicks == 1)
	{
		/* calcula vetor distância entre posição do mouse com o ponto do clique e o inverte
		float vx = -(x - vertices[0].Pos.x);
		float vy = -(y - vertices[0].Pos.y);
		vx += vertices[0].Pos.x;	// desloca para a posição correta
		vy += vertices[0].Pos.y;
		*/

		// expressão simplificada
		float vx = 2 * vertices[0].Pos.x - x;
		float vy = 2 * vertices[0].Pos.y - y;

		// ponto invertido
		vertices[1].Pos.x = vx;
		vertices[1].Pos.y = vy;

		// ponto que segue o mouse
		vertices[3].Pos.x = x;
		vertices[3].Pos.y = y;
		vertices[3].Color = XMFLOAT4(0.098f, 0.098f, 0.098f, 1.f);	// ajusta para a cor de fundo

		// quadrado invertido
		for (uint i = 0, index = 4; index < 12; ++index, ++i)
		{
			vertices[index].Pos.x = vx + quadAux[i].Pos.x;
			vertices[index].Pos.y = vy + quadAux[i].Pos.y;
		}
	}
	else if (numClicks == 3)
	{
		float vx = 2 * vertices[0 + numSupport].Pos.x - x;
		float vy = 2 * vertices[0 + numSupport].Pos.y - y;

		// ponto invertido
		vertices[1 + numSupport].Pos.x = vx;
		vertices[1 + numSupport].Pos.y = vy;

		// ponto que segue o mouse
		vertices[3 + numSupport].Pos.x = x;
		vertices[3 + numSupport].Pos.y = y;
		vertices[3 + numSupport].Color = XMFLOAT4(0.098f, 0.098f, 0.098f, 1.f);	// ajusta para a cor de fundo

		// quadrado invertido
		for (uint i = 0, index = 4 + numSupport; index < 12 + numSupport; ++index, ++i)
		{
			vertices[index].Pos.x = vx + quadAux[i].Pos.x;
			vertices[index].Pos.y = vy + quadAux[i].Pos.y;
		}

		// redesenha a curva
		count = 40;
		DrawBezier();
	}
}

void BezierAlgorithm::OnClick(float x, float y)
{
	// verifica quantos cliques até agora
	switch (numClicks)
	{
	case 0:	// primeiro clique, gera dois quadrados e duas linhas
		GenerateSupportVertices(x, y);

		numClicks++;
		break;

	case 1:
		numClicks++;
		break;

	case 2:
		GenerateSupportVertices(x, y);

		// desenha a curva
		DrawBezier();
		numClicks++;
		break;

	//case 3: // no quarto clique, inverte as posições de p3 e p4
	//	
	//	break;
	}
}

void BezierAlgorithm::OnDelete()
{
	count = index = numClicks = 0;
}

void BezierAlgorithm::OnIterate() {}

void BezierAlgorithm::GenerateSupportVertices(float x, float y)
{
	// ordem no vetor de vértices para melhor manipulação:
	// linha invertida
	// linha que segue o mouse
	// quadrado invertido
	// quadrado fixo

	// copia vértices dos quadrados deslocando cada um por x e y
	// gera linhas posicionando dois vértices nas coordenadas do clique
	for (uint i = 0; i < 4; ++i)
	{
		if (i < 2)
		{
			// linha
			vertices[count++] = { XMFLOAT3(x, y, 0.f), XMFLOAT4(Colors::DarkRed) };
			vertices[count++] = { XMFLOAT3(x, y, 0.f), XMFLOAT4(Colors::DarkRed) };
		}
		else
		{
			// quadrado
			for (const Vertex& v : quadAux)
			{
				vertices[count] = v;
				vertices[count].Pos.x += x;
				vertices[count++].Pos.y += y;
			}
		}
	}
}

void BezierAlgorithm::DrawBezier()
{
	for (uint i = 0; i <= lineSegs; ++i)
	{
		float t = 1.0f / lineSegs * i;

		float px = powf(1.0f - t, 3) * vertices[0].Pos.x
			+ 3 * t * powf(1.0f - t, 2) * vertices[1].Pos.x
			+ 3 * t * t * (1.0f - t) * vertices[21].Pos.x
			+ t * t * t * vertices[20].Pos.x;

		float py = powf(1.0f - t, 3) * vertices[0].Pos.y
			+ 3 * t * powf(1.0f - t, 2) * vertices[1].Pos.y
			+ 3 * t * t * (1.0f - t) * vertices[21].Pos.y
			+ t * t * t * vertices[20].Pos.y;

		vertices[count++] = { XMFLOAT3(px, py, 0.0f), XMFLOAT4(Colors::Yellow) };
		if (i != 0) vertices[count++] = { XMFLOAT3(px, py, 0.0f), XMFLOAT4(Colors::Yellow) };
	}
}

void BezierAlgorithm::Save()
{
	//MessageBox(window->Id(), string("Curva salva com sucesso!").c_str(), string("Salvar Curva de Bézier").c_str(), MB_OK);
}

void BezierAlgorithm::Load()
{

}
