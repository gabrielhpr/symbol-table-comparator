unit_test.o:
	g++ -o unit_test vetor_desordenado.h vetor_ordenado.h lista_ligada_desordenada.h \
	lista_ligada_ordenada.h arvore_busca_binaria.h treaps.h arvores_23.h \
	arvores_rubro_negras.h tabelas_de_hashing.h unit_test.cpp 
