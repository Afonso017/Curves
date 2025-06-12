# Curvas Gráficas Interativas
Aplicação gráfica interativa em Direct3D 12 para construção e desenho de curvas de Bézier e Chaikin.  
A aplicação oferece funcionalidades para criar, editar, salvar e carregar curvas, além de alternar entre os algoritmos.

# Funcionalidades  
Desenho Interativo: Crie pontos de controle para as curvas com cliques do mouse.  
Algoritmo de Chaikin: Suaviza uma polilinha através de um processo iterativo.  
Algoritmo de Bézier: Gera curvas suaves a partir de 4 pontos de controle usando polinômios cúbicos de Bernstein.  
Alternância de Algoritmo: Mude entre os algoritmos de Chaikin e Bézier em tempo real.    
Iterações de Chaikin: Execute iterações do algoritmo de Chaikin para observar a suavização progressiva da curva.
Manipulação de Curvas: Mova pontos de controle existentes para ajustar a forma da curva.  
Salvar e Carregar Curvas: Persista o estado atual da curva em um arquivo e carregue-o posteriormente.  
Limpar Tela: Apague a curva atual para começar um novo desenho.

# Controles
C: Alterna para o algoritmo de Chaikin.  
B: Alterna para o algoritmo de Bézier.  
S: Salva o estado atual da curva.  
L: Carrega o último estado salvo da curva para o algoritmo selecionado.  
DELETE: Apaga a curva atual.  
ENTER: Executa uma iteração do algoritmo de Chaikin (apenas quando o algoritmo de Chaikin está ativo).  
ESC: Fecha a aplicação.  
Clique Esquerdo do Mouse: Adiciona um ponto de controle na posição do mouse.  
Movimento do Mouse: Ajusta a posição de pontos de controle ou pré-visualiza a adição de novos pontos.  

# Estrutura do Projeto
O projeto é organizado em módulos que encapsulam a lógica de cada componente.  
A aplicação utiliza de um modelo baseado em eventos, cada interação do usuário gera um evento correspondente ao algoritmo atualmente selecionado.  

CurveAlgorithm.h: Classe abstrata que fornece os métodos baseados em eventos que serão chamados pela aplicação.   
Curves.h / Curves.cpp: Contém a lógica principal da aplicação, gerenciando os algoritmos de curva, a interação do usuário e o pipeline gráfico.  
ChaikinAlgorithm.h / ChaikinAlgorithm.cpp: Implementa CurveAlgorithm e o comportamento para o algoritmo iterativo de Chaikin.  
BezierAlgorithm.h / BezierAlgorithm.cpp: Implementa CurveAlgorithm e o comportamento para o algoritmo de Bézier para geração de curvas.
