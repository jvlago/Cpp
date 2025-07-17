# Cache Simulator

## Descrição:
Um simulador funcional de caches em C++.\
Este simulador é parametrizável quanto ao número de conjuntos, tamanho do bloco, nível de associatividade e política de substituição.

## Especificação:
	nsets: número de conjuntos
	bsize: tamanho do bloco
	assoc: associatividade
	subst: política de substituição (R, L, F)
    flagOut: flag para saída (0 ou 1)
    arquivoEntrada: caminho do arquivo de entrada
    Input genérico: ./cache_simulator <nsets> <bsize> <assoc> <subst> <flagOut> <arquivoEntrada>
  	Exemplo de uso: ./cache_simulator 16 64 4 r 0 arquivo.bin
    Output genérico:
    1. Para flagOut == 0:
      Misses/Hits: <value>
      Total de acessos: <value>
      Hits: <value> <rate>
      Misses: <value> <rate>
      Compulsórios: <value> <rate>
      Capacidade: <value> <rate>
      Conflito: <value> <rate>
    2. Para flagOut == 1:
      <addressesAmount> <hitRate> <missRate> <compRate> <capRate> <conflRate>

## Autores:
  João Vitor Montanher Lago\
  E-mail: jvmlago@inf.ufpel.edu.br
  
  Matheus Borges Goulart\
  E-mail: mbgoulart@inf.ufpel.edu.br