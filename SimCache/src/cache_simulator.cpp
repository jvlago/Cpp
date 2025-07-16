#include <iostream>
#include <math.h>
#include <stdint.h>
#include <fstream>
#include <vector>

// Especificação
	// nsets: número de conjuntos
	// bsize: tamanho do bloco
	// assoc: associatividade
	// subst: política de substituição (R, L, F)
	// flagOut: flag para saída (0 ou 1)
	// arquivoEntrada: caminho do arquivo de entrada
	// Exemplo de uso: ./cache_simulator 16 64 4 r 0 arquivo.bin

struct Cache_t {
	int tag;
	int cycle;
	bool valid;
};

int main(int argc, char *argv[])
{
    if (argc != 7){
        std::cout << "Numero de argumentos incorreto. Utilize: " << std::endl;
        std::cout << "./cache_simulator <nsets> <bsize> <assoc> <substituição> <flag_saida> arquivo_de_entrada" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Declaração das variáveis recebidas como argumentos
    int nsets = atoi(argv[1]);
    int bsize = atoi(argv[2]);
    int assoc = atoi(argv[3]);
    char *subst = argv[4];
    int flagOut = atoi(argv[5]);
    char *arquivoEntrada = argv[6];

    // Declaração de variáveis internas
    int bOffset, bIndice, bins, ends = 0, tag, indice, cval, freePos, hits = 0, compulsorio = 0, capacidade = 0, conflito = 0, misses, usedPos = 0;
	bool hit;
    std::vector<Cache_t> cache(nsets * assoc);
	std::ifstream infile;

    // Verificação das características da cache
    if (nsets <= 0 || bsize <= 0 || assoc <= 0) { //TODO: Adicionar verificação de potência de 2
    	std::cout << "nsets, bsize e assoc precisam ser potências de 2 e maiores que zero" << std::endl;
    	exit(EXIT_FAILURE);
	}
	else if (*subst != 'r' && *subst != 'l' && *subst != 'f') {
		std::cout << "subst precisa ser r = Random, l = LRU e f = FIFO" << std::endl;
		exit(EXIT_FAILURE);
	}
	else if (flagOut != 0 && flagOut != 1) {
		std::cout << "flagOut precisa ser 0 ou 1" << std::endl;
		exit(EXIT_FAILURE);
	}

	infile.open(arquivoEntrada, std::ifstream::binary);

	if (infile.fail()) {
       	std::cout << "Arquivo de entrada não encontrado" << std::endl;
		exit(EXIT_FAILURE);
    }

	// Calculo de offset, índice e tag
	bOffset = log2(bsize);
	bIndice = log2(nsets);
	
	for (auto& line : cache)
        line.valid = false;

	while(infile.read(reinterpret_cast<char*>(&bins), sizeof(int))) {
		bins = __builtin_bswap32(bins); // Converte de little-endian para big-endian
		tag = bins >> (bOffset + bIndice);
    	indice = (bins >> bOffset) & ((1 << bIndice) -1);
		ends++;
		cval = 0;
		hit = false;
		for(int i = 0; i < assoc; i++) {
			if (cache[indice * assoc + i].valid) {
				cval++;
				if (cache[indice * assoc + i].tag == tag) {
					hit = true;
					hits++;
					if(!flagOut)
						std::cout << "Hit: " << bins << std::endl;
					if (*subst == 'l')
						cache[indice * assoc + i].cycle = ends;
					break;
				}
			} else
				freePos = indice * assoc + i;
		}
		if(!hit) {
			if (cval < assoc) {
				compulsorio++;
				cache[freePos].valid = true;
				cache[freePos].tag = tag;
				usedPos++;
				if (*subst == 'l' || *subst == 'f')
					cache[freePos].cycle = ends;
					if(!flagOut)
						std::cout << "Miss compulsório: " << bins << std::endl;
			} else {
				if (usedPos >= nsets * assoc) {
					capacidade++;
					if(!flagOut)
						std::cout << "Miss de capacidade: " << bins << std::endl;
				} else {
					conflito++;
					if(!flagOut)
						std::cout << "Miss de conflito: " << bins << std::endl;
				}
				if (*subst == 'r') {
					int aux = rand() % assoc; // Sem seed
					cache[indice * assoc + aux].tag = tag;
					cache[indice * assoc + aux].valid = true;
				} else if (*subst == 'f' || *subst == 'l') {
					int low = INT32_MAX, aux = 0;
					for(int i = 0; i < assoc; i++) {
						if (cache[indice * assoc + i].cycle < low) {
							low = cache[indice * assoc + i].cycle;
							aux = i;
						}
					}
					cache[indice * assoc + aux].tag = tag;
					cache[indice * assoc + aux].valid = true;
					cache[indice * assoc + aux].cycle = ends;
				} else {
					std::cout << "Política de substituição inválida" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	
	misses = compulsorio + capacidade + conflito;
	
	// Exibe os resultados
	if(!flagOut) {
		std::cout << "Total de acessos: " << ends << std::endl;
		std::cout << "Hits: " << hits << " " << (float)hits/ends << std::endl;
		std::cout << "Misses: " << misses << " "  << (float)misses/ends << std::endl;
		std::cout << "Compulsórios: " << compulsorio << " "  << (float)compulsorio/misses << std::endl;
		std::cout << "Capacidade: " << capacidade  << " " << (float)capacidade/misses << std::endl;
		std::cout << "Conflito: " << conflito << " " << (float)conflito/misses << std::endl;
	} else
		std::cout << ends << " " << (float)hits/ends << " " << (float)misses/ends << " " << (float)compulsorio/misses << " " << (float)capacidade/misses << " " << (float)conflito/misses << std::endl;
	infile.close();
}
