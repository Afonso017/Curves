#include "Curves.h"

// define formato das geometrias utilizadas na aplica��o

// v�rtices da linha auxiliar
Vertex lineAux[2]
{ 
	{ XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT4(Colors::DarkRed) },
	{ XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT4(Colors::DarkRed) }
};

// v�rtices dos quadrados de apoio
Vertex quadAux[6]
{
	{ XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT4(Colors::DarkRed) },
	{ XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT4(Colors::DarkRed) },
	{ XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT4(Colors::DarkRed) },
	{ XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT4(Colors::DarkRed) },
	{ XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT4(Colors::DarkRed) },
	{ XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT4(Colors::DarkRed) }
};
