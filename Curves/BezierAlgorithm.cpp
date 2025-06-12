#include "BezierAlgorithm.h"
#include "Curves.h"

// vértices dos quadrados de apoio, considerando LINELIST como topologia
const Vertex quadAux[8]
{
	{ XMFLOAT3(-0.01f, -0.015f, 0.f), XMFLOAT4(Colors::Red) },	// v0
	{ XMFLOAT3(0.01f, -0.015f, 0.f), XMFLOAT4(Colors::Red) },	// v1
	{ XMFLOAT3(0.01f, -0.015f, 0.f), XMFLOAT4(Colors::Red) },	// v1
	{ XMFLOAT3(0.01f, 0.015f, 0.f), XMFLOAT4(Colors::Red) },	// v2
	{ XMFLOAT3(0.01f, 0.015f, 0.f), XMFLOAT4(Colors::Red) },	// v2
	{ XMFLOAT3(-0.01f, 0.015f, 0.f), XMFLOAT4(Colors::Red) },	// v3
	{ XMFLOAT3(-0.01f, 0.015f, 0.f), XMFLOAT4(Colors::Red) },	// v3
	{ XMFLOAT3(-0.01f, -0.015f, 0.f), XMFLOAT4(Colors::Red) }	// v0
};

BezierAlgorithm::BezierAlgorithm()
{
	topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;

	// aloca vertex buffer
	vbuffer = new VertexBuffer<Vertex>(nullptr, MaxSize);
}

BezierAlgorithm::~BezierAlgorithm()
{
	delete vbuffer;
}

void BezierAlgorithm::OnMouseMove(float x, float y)
{
	// verifica quantos cliques até agora
	// manipula primeiro offset dos vértices de apoio somente no primeiro clique
	// após isso, sempre manipula o segundo offset, para facilitar a identificação dos vértices ao chamar DrawBezier
	if (showSupport && numClicks % 2 == 1)
	{
		offset = numClicks == 1 ? 0 : 20;

		MoveSupportVertices(x, y);

		if (numClicks >= 3)
		{
			// índice da próxima curva
			count = index;

			// desenha a curva
			DrawBezier();
		}
	}
}

void BezierAlgorithm::OnClick(float x, float y)
{
	// limite máximo de 20 curvas = 2040, cada curva tem 100 vértices + 40 vértices de apoio
	if (count == MaxSize && numClicks % 2 == 0)
	{
		MessageBox(0, "Limite máximo de curvas atingido: 20", "Erro ao gerar nova curva", MB_OK);
		return;
	}

	if (!showSupport)
	{
		Load();
		showSupport = true;
	}

	// verifica quantos cliques até agora
	switch (numClicks)
	{
	case 0:	// primeiro clique, gera dois quadrados, duas linhas e define P1
		GenerateSupportVertices(x, y);
		break;

	case 1: // segundo clique define P2
		index = 40;		// início da curva
		break;

	case 2: // terceiro clique gera mais dois quadrados e duas linhas e define P4
		GenerateSupportVertices(x, y);
		offset = 20;	// trabalha com o segundo conjunto de vértices de apoio
		MoveSupportVertices(x, y);
		DrawBezier();
		break;

	default: // inverte P3 com seu ponto oposto e reposiciona vértices no vetor para garantir a ordem
		if (numClicks % 2 == 1) index += 100;
		else
		{
			std::copy(vertices + offset, vertices + 20 + offset, vertices);
			std::swap(vertices[1].Pos, vertices[3].Pos);

			// reposiciona quadrado de apoio de P3
			for (uint i = 0, index = 4; index < 12; ++index, ++i)
			{
				vertices[index].Pos.x = vertices[1].Pos.x + quadAux[i].Pos.x;
				vertices[index].Pos.y = vertices[1].Pos.y + quadAux[i].Pos.y;
			}

			index = count;
			count = 20;
			GenerateSupportVertices(x, y);

			count = index;
			DrawBezier();
		}
		break;
	}
	numClicks++;
}

void BezierAlgorithm::OnDelete()
{
	count = numClicks = 0;
	index = 40;
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
			vertices[count++] = { XMFLOAT3(x, y, 0.f), XMFLOAT4(Colors::Red) };
			vertices[count++] = { XMFLOAT3(x, y, 0.f), XMFLOAT4(Colors::Red) };
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

void BezierAlgorithm::MoveSupportVertices(float x, float y)
{
	/* calcula vetor distância entre posição do mouse com o ponto do clique e o inverte
	float vx = -(x - vertices[0].Pos.x);
	float vy = -(y - vertices[0].Pos.y);
	vx += vertices[0].Pos.x;	// desloca para a posição correta
	vy += vertices[0].Pos.y;
	*/

	// expressão simplificada
	float vx = 2 * vertices[0 + offset].Pos.x - x;
	float vy = 2 * vertices[0 + offset].Pos.y - y;

	// ponto invertido
	vertices[1 + offset].Pos.x = vx;
	vertices[1 + offset].Pos.y = vy;

	// ponto que segue o mouse
	vertices[3 + offset].Pos.x = x;
	vertices[3 + offset].Pos.y = y;
	vertices[3 + offset].Color = XMFLOAT4(0.098f, 0.098f, 0.098f, 1.f);	// ajusta para a cor de fundo

	// quadrado invertido
	for (uint i = 0, index = 4 + offset; index < 12 + offset; ++index, ++i)
	{
		vertices[index].Pos.x = vx + quadAux[i].Pos.x;
		vertices[index].Pos.y = vy + quadAux[i].Pos.y;
	}
}

void BezierAlgorithm::DrawBezier()
{
	for (uint i = 0; i <= lineSegs; ++i)
	{
		float t = 1.0f / lineSegs * i;

		float px = powf(1.0f - t, 3) * vertices[0].Pos.x
			+ 3 * t * powf(1.0f - t, 2) * vertices[1].Pos.x
			+ 3 * t * t * (1.0f - t) * vertices[1 + offset].Pos.x
			+ t * t * t * vertices[0 + offset].Pos.x;

		float py = powf(1.0f - t, 3) * vertices[0].Pos.y
			+ 3 * t * powf(1.0f - t, 2) * vertices[1].Pos.y
			+ 3 * t * t * (1.0f - t) * vertices[1 + offset].Pos.y
			+ t * t * t * vertices[0 + offset].Pos.y;

		vertices[count++] = { XMFLOAT3(px, py, 0.0f), XMFLOAT4(Colors::Yellow) };

		// LINELIST precisa de vértices duplicados
		if (i != 0 && i != lineSegs) vertices[count++] = { XMFLOAT3(px, py, 0.0f), XMFLOAT4(Colors::Yellow) };
	}
}

bool BezierAlgorithm::Save()
{
	std::ofstream file("bezier.dat", std::ios::binary);

	if (!file) return false;

	CurveSave* curveSave = new CurveSave();
	curveSave->saveIndex = count;
	curveSave->numClicksSave = numClicks;
	curveSave->offsetSave = offset;
	std::copy(vertices, vertices + count, curveSave->save);

	// fatia os 40 primeiros vértices do vetor para não mostrar as linhas de suporte
	if (count >= 140)
	{
		std::copy(curveSave->save + 40, curveSave->save + count, vertices);
		showSupport = false;
	}

	file.write(reinterpret_cast<const char*>(curveSave), sizeof(CurveSave));
	file.close();

	delete curveSave;
	return true;
}

bool BezierAlgorithm::Load()
{
	std::ifstream file("bezier.dat", std::ios::binary);

	if (!file) return false;

	CurveSave* curveSave = new CurveSave();
	file.read(reinterpret_cast<char*>(curveSave), sizeof(CurveSave));
	file.close();

	std::copy(curveSave->save, curveSave->save + curveSave->saveIndex, vertices);
	count = curveSave->saveIndex;
	index = (count >= 140 ? count - 100 : 40);
	numClicks = curveSave->numClicksSave;
	offset = curveSave->offsetSave;

	delete curveSave;
	return true;
}
