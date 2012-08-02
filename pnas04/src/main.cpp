#include "population.h"
#include <ctime>
#include "consts.h"

int main (void) {

    srand(1);
    //  initialization
    ustc::Population myPop(POPULATION, TOTAL_EVO);
    myPop.init ();
	// ask users if they can input more information about the cell
	void askInformation(ustc::Population* targetPop);
	askInformation(&myPop);
	
    int i = 1;
    int sum = 1;
    //  evolution
    while (!myPop.isTerminate ()) {
        if ((TOTAL_EVO + 1 - myPop.getEvolution()) % (50 * sum) == 0) {
            //sort
            myPop.sort();
            myPop.output();
            myPop.mutation();
            i++;
            sum += i;
        }
        myPop.mut_parameters_simAnneal();
        std::cout << "Finished Evolution: " << TOTAL_EVO - myPop.getEvolution() << std::endl;
    }

    
    //  output
    myPop.sort();
    myPop.output();
    myPop.genSBMLFormat();

    return 0;
}


void askInformation(ustc::Population* targetPop){
	ustc::reaction_type rType;
    getchar();
	std::cout << "Do you know more information about the reaction? <y/n>" << std::endl;
	while(getchar() == 'y'){
		getchar();
		
		//print the types of reaction
		std::cout<< "1. inducer and protein binding" << std::endl;
		std::cout<< "2. protein and protein binding" << std::endl;
		std::cout<< "3. protein and gene binding" << std::endl;
		std::cout<< "4. protein modification" << std::endl;
		
		// print all nodes a cell contains
		std::cout<< "Species in the cell are:" << std::endl;
		ustc::Cell* aCell = (targetPop -> getCells())[0];
		//size of nodes
		int size = (int)((aCell->getNodesVector()) -> size());
		// print the Nstrings of nodes
		for(int i = 0; i < size; i++){
			std::stringstream num;
			num << (i + 1);
			std::cout<< num.str() << (*(aCell -> getNodesVector()))[i] -> getNstring() << std::endl;
		}
		// ask the reaction type
		std::cout<< "please type in the type of reaction(1-4): ";
		int tempType;
		std::cin >> tempType;
		if(tempType == 4){
			std::cout<< "please type in the index of reactans";
			int tempIndex = 0;
			std::cin >> tempIndex;
			int index = (tempIndex - 1);
			rType = ustc::MODIFICATION;
			for(int i = 0; i < POPULATION; i ++){
			((targetPop -> getCells())[i]) -> addReaction(rType,index);
			}
		}
		else{
			if(tempType == 1){
				rType = ustc::INDU_PROT_BINDING;
			}
			else{
				if(tempType == 2){
					rType = ustc::DIMERIZATION;
				}
				else{
					if(tempType == 3){
						rType = ustc::BINDING;
					}
				}
			}
			int index1 = 0;
			int index2 = 0;
			int tempIndex1 = 0;
			int tempIndex2 = 0;
			std::cout<< "please type in the index of reactans";
			std::cin>> tempIndex1;
			index1 = (tempIndex1 - 1);
			std::cout<< "please type in the index of another reactans";
			std::cin>> tempIndex2;
			index2 = (tempIndex2 - 1);
			for(int i = 0; i < POPULATION; i ++){
			((targetPop -> getCells())[i]) -> addReaction(rType,index1,index2);
			}
		}
        
        getchar();
		std::cout << "Do you know more information about the reaction? <y/n>" << std::endl;
	}
}


