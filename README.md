# Algoritmo árvore PATRICIA
Implementação de um algoritmo para fazer a leitura e computação de um conjunto de palavras, inserindo, buscando e removendo utilizando uma árvore PATRICIA como parte da disciplina de Algoritmo e Estrutura de Dados.
## Implementação
* O programa foi construído para fazer inserção, remoção e busca de árvore trie e inserção e busca de árvore Patricia.  
  
  
**Busca**: A busca por uma string em uma arvore PATRICIA é similar a busca em uma Trie, com a diferença de que ao chegar em um nó,é comparado apenas um caractere, contra a comparação de substrings inteiras que acontece na Trie. No pior caso, a complexidade de tempo é O(|s|), onde s é a string procurada.  

**Inserção**: Inserir uma string em uma árvore Patricia é similar a pesquisar por essa string até o ponto onde a busca é encerrada, pois a string não é encontrada na árvore. Se a busca é encerrada em uma aresta, um novo nó é criado nessa aresta. Esse nó armazena a posição do caractere que distingue a chave destino daquela aresta e a chave que se deseja inserir, e tem como filhos o nó que estava na extremidade seguinte da aresta e um novo nó com a parte restante da nova chave. Se a busca for encerrada em um nó, então um nó filho é criado e o restante da nova chave é usado como rótulo para aresta entre os dois. Ambos os casos tem complexidade de tempo de O(|s| + |E|), onde s é a string que será inserida e E é o alfabeto suportado pela árvore.  

**Remoção**: Remover uma string de uma árvore PATRICIA é o oposto da operação de inserção. Primeiro, localiza-se a folha correspondente a string e remove-se ela da árvore. Como o pai terá apenas um filho, os nós pai e irmão do nó removido são agrupados em um único nó. A complexidade de tempo depende diretamente do tempo para remover 2 nós da árvore, se essa remoção for considerada linear, então a complexidade de tempo da operação é O(|s|), onde s é a string que será removida, se essa remoção tiver complexidade O(N), então a complexidade de tempo da operação é O(|s| + N), onde N é o tamanho total de todas as strings armazenadas na árvore.
