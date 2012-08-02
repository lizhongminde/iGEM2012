//
//  plasmid.cpp
//  PNAS04Team
//
//  Created by Jack Kang on 12-8-1.
//
//

#include "plasmid.h"


namespace ustc {


Plasmid::Plasmid(){
    
}


DNAPiece* Plasmid::getDnaPiece(const int& dnaPieceIndex){
    int size = (int)dnaPieces.size();
    if (dnaPieceIndex >= 0 && dnaPieceIndex < size) {
        return dnaPieces[dnaPieceIndex];
    }
    return NULL;
}

void Plasmid::addDnaPiece(DNAPiece* newDNAPiece){
    dnaPieces.push_back(newDNAPiece);
}
    
    
void Plasmid::readMotifs(const int& cellIndex){

	std::stringstream ss;
	ss << "Cell_"<<cellIndex <<"_Motifs.txt";
	std::ifstream infile;
	infile.open(ss.str().c_str());
	if (!infile) {
        std::cerr << "Error: unable to open input file: " << infile << std::endl;
		exit(1);
    }

	infile >> numSingleMotifs >> numDoubleMotifs >> numTripleMotifs;
	if (infile.bad ()) throw std::runtime_error ("IO stream corrupted");
	if (infile.fail ()) throw std::runtime_error ("bad data");
	if (numSingleMotifs + numDoubleMotifs + numTripleMotifs == 0) {
		std::cerr << "Error: empty data" << std::endl;
		exit(1);
	}

	// creat singleMotifMatrice
	singleMotifsMatrice = new int**[numSingleMotifs];
	for(int i = 0; i < numSingleMotifs; i++){
		singleMotifsMatrice[i] = new int*[1];
		singleMotifsMatrice[i][0] = new int[1];
	}
	
	// creat doubleMotifsMatrice
	doubleMotifsMatrice = new int**[numDoubleMotifs];
	for(int i = 0; i < numDoubleMotifs; i++){
		doubleMotifsMatrice[i] = new int*[2];
		for(int j = 0; j < 2; j++){
			doubleMotifsMatrice[i][j]=new int[2];
		}
	}

	// creat tripleMotifsMatrice
	tripleMotifsMatrice = new int**[numTripleMotifs];
	for(int i = 0; i < numTripleMotifs; i++){
		tripleMotifsMatrice[i] = new int*[3];
		for(int j = 0; j < 3; j++){
			tripleMotifsMatrice[i][j] = new int[3];
		}
	}

	// read singleMotifMatrice
	for(int i = 0; i < numSingleMotifs; i++){
		infile >> singleMotifsMatrice[i][0][0];
	}

	// read doubleMotifMatrice
	for(int i = 0; i < numDoubleMotifs; i++){
		infile >> doubleMotifsMatrice[i][0][0] >> doubleMotifsMatrice[i][0][1];
		infile >> doubleMotifsMatrice[i][1][0] >> doubleMotifsMatrice[i][1][1];
	}
	
	// read tripleMotifMatrice
	for(int i = 0;i < numTripleMotifs; i++){
		infile >> tripleMotifsMatrice[i][0][0] >> tripleMotifsMatrice[i][0][1] >> tripleMotifsMatrice[i][0][2];
		infile >> tripleMotifsMatrice[i][1][0] >> tripleMotifsMatrice[i][1][1] >> tripleMotifsMatrice[i][1][2];
		infile >> tripleMotifsMatrice[i][2][0] >> tripleMotifsMatrice[i][2][1] >> tripleMotifsMatrice[i][2][2];
	}
	infile.close();
}

    
void Plasmid::findCandidates(
                             const int& numRow,
                             const int& numColumn,
                             const std::string* namesOfGenes,
                             const std::string* namesOfPromoters,
                             const int** database
                             ){

	//find single motif cadidates
	// j -> gene
	// k -> promoter
    for(int i = 0; i < numSingleMotifs; i ++){
		for(int j = 0; j < numColumn; j ++){
			for(int k = 0; k < numRow; k ++){
				if(database[k][j] == singleMotifsMatrice[i][0][0]){
					MotifCandidate* MC;
					GeneCandidate* GC;
					GC -> name = namesOfGenes[j];
					GC -> itsPromoterStrings.push_back(namesOfPromoters[k]);
					MC -> size = _1X1;
					MC -> geneString.push_back(namesOfGenes[j]);
					MC -> genes.push_back(GC);
					MC -> promoterStrings.push_back(namesOfPromoters[k]);
					candidates.push_back(MC);
				}
			}
		}
	}



	//find double motif cadidats
	//i -> gene1
	//k -> gene2
	//j -> promt1
	//l -> promt2
	for(int n = 0; n < numDoubleMotifs; n ++){
		for(int i = 0; i < numColumn; i ++){
			for(int j = 0; j < numRow; j ++){
				// promt1 satisfy motifmatrice[n][0][0]
				if(database[j][i] != doubleMotifsMatrice[n][0][0]){ 
					continue;
				}
				for(int k = 0; k < numColumn; k ++){ 
					//gene2 != gene1  ||   gene2 satisfy motifmatrice[n][0][1]
					if(k == i || database[j][k] != doubleMotifsMatrice[n][0][1]){
						continue;
					}
					for(int l = 0; l < numRow; l++){
						// this will satisfy all condition
						if(database[l][i] == doubleMotifsMatrice[n][1][0] && database[k][l] == doubleMotifsMatrice[n][1][1]){
							GeneCandidate* GC1;
							GeneCandidate* GC2;
							MotifCandidate* MC;
							GC1 -> name = namesOfGenes[i];
							GC1 -> itsPromoterStrings.push_back(namesOfPromoters[j]);
							GC2 -> name = namesOfGenes[k];
							GC2 -> itsPromoterStrings.push_back(namesOfPromoters[l]);
							MC -> size = _2X2;
							MC -> geneString.push_back(GC1 -> name);
							MC -> geneString.push_back(GC2 -> name);
							MC -> promoterStrings.push_back(namesOfPromoters[j]);
							MC -> promoterStrings.push_back(namesOfPromoters[l]);
							MC -> genes.push_back(GC1);
							MC -> genes.push_back(GC2);
							candidates.push_back(MC);
						}
					}
				}
			}
		}
	}
	


	// find trible motif candidates
	// i -> gene1
	// k -> gene2
	// p -> gene3
	// j -> promt1
	// l -> promt2
	// q -> promt3
	for(int n = 0; n < numTripleMotifs; n ++){
		for(int i = 0; i < numColumn; i ++){
			for(int j = 0; j < numRow; j ++){
				// promt1 satisfy motifmatrice[n][0][0]
				if(database[j][i] != tripleMotifsMatrice[n][0][0]){ 
					continue;
				}
				for(int k = 0; k < numColumn; k ++){ 
					//gene2 != gene1  ||   gene2 satisfy motifmatrice[n][0][1]
					if(k == i || database[j][k] != tripleMotifsMatrice[n][0][1]){
						continue;
					}
					for(int l = 0; l < numRow; l++){
						if(database[l][i] != tripleMotifsMatrice[n][1][0] || database[l][k] != tripleMotifsMatrice[n][1][1]){
							continue;
						}
						for(int p = 0; p < numColumn; p ++){
							if(p ==	k || p == i || database[j][p] != tripleMotifsMatrice[n][0][2] || database[l][p] != tripleMotifsMatrice[n][1][2]){
								continue;
							}
							for(int q  = 0; q < numRow; q ++){
								//satisfy all condition
								if(database[q][i] == tripleMotifsMatrice[n][2][0] && database[q][k] == tripleMotifsMatrice[n][2][1] && database[q][p] == tripleMotifsMatrice[n][2][2]){
									GeneCandidate* GC1;
									GeneCandidate* GC2;
									GeneCandidate* GC3;
									MotifCandidate* MC;
									GC1 -> name = namesOfGenes[i];
									GC1 -> itsPromoterStrings.push_back(namesOfPromoters[j]);
									GC2 -> name = namesOfGenes[k];
									GC2 -> itsPromoterStrings.push_back(namesOfPromoters[l]);
									GC3 -> name = namesOfGenes[p];
									GC3 -> itsPromoterStrings.push_back(namesOfPromoters[q]);
									MC -> size = _3X3;
									MC -> geneString.push_back(GC1 -> name);
									MC -> geneString.push_back(GC2 -> name);
									MC -> geneString.push_back(GC3 -> name);
									MC -> promoterStrings.push_back(namesOfPromoters[j]);
									MC -> promoterStrings.push_back(namesOfPromoters[l]);
									MC -> promoterStrings.push_back(namesOfPromoters[q]);
									MC -> genes.push_back(GC1);
									MC -> genes.push_back(GC2);
									MC -> genes.push_back(GC3);
									candidates.push_back(MC);
								}
							}
						}
					}
				}
			}
		}
	}

}


	
	
    
    
}// namespace ustc