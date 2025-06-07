#include "BezierAlgorithm.h"
#include "Curves.h"

// v�rtices da linha auxiliar
Vertex lineAux[2]
{
	{ XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT4(Colors::DarkRed) },
	{ XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT4(Colors::DarkRed) }
};

// v�rtices dos quadrados de apoio
Vertex quadAux[5]
{
	{ XMFLOAT3(-0.015f, -0.02f, 0.f), XMFLOAT4(Colors::DarkRed) },
	{ XMFLOAT3(0.015f, -0.02f, 0.f), XMFLOAT4(Colors::DarkRed) },
	{ XMFLOAT3(0.015f, 0.02f, 0.f), XMFLOAT4(Colors::DarkRed) },
	{ XMFLOAT3(-0.015f, 0.02f, 0.f), XMFLOAT4(Colors::DarkRed) },
	{ XMFLOAT3(-0.015f, -0.02f, 0.f), XMFLOAT4(Colors::DarkRed) }
};

BezierAlgorithm::BezierAlgorithm()
{
  	topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// aloca vertex buffer
	vbuffer = new VertexBuffer<Vertex>(nullptr, MaxSize);
}

BezierAlgorithm::~BezierAlgorithm()
{
	pipelineState->Release();
	delete vbuffer;
}

void BezierAlgorithm::OnCreateVertex(float x, float y)
{
	// verifica quantos cliques at� agora
	switch (numClicks)
	{
	case 0:	// clicou agora, gera dois pontos ligados por uma linha
		// desloca cada v�rtice por x e y
		for (Vertex& v : quadAux)
		{
			v.Pos.x += x;
			v.Pos.y += y;
		}
		
		// desenha quadCopy
		count = 5;
		break;
	}
}

void BezierAlgorithm::OnDelete()
{

}

void BezierAlgorithm::OnIterate()
{

}

void BezierAlgorithm::Save()
{
	//MessageBox(window->Id(), string("Curva salva com sucesso!").c_str(), string("Salvar Curva de B�zier").c_str(), MB_OK);
}

void BezierAlgorithm::Load()
{

}
