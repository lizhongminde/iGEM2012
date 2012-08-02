#ifndef CELL_H
#define CELL_H

#include "consts.h"
#include "node.h"
#include "reaction.h"
#include "motif.h"
#include "scorefunction.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>

namespace ustc{

class Cell {

    public:

		//	no default constructor, for construction, number of inducers and proteins should be provided
		Cell (const int& _numind, const int& _numprot, const int& _numInputSets);
        Cell (Cell &cell);//copy constructor

		//	deconstructor
		~Cell ();

		//	call for six submutations successively
        void mutation();

        //mutation only changing dynamical parameters
        void mut_parameters();
            
        //mutation only changing dynamical parameters using simulated annealing algorithm
        void mut_parameters_simAnneal();
    
        //mutation only for new nodes or new reactions
        void mut_topology();
    
        //explicitly adding reaction to the cell
        void addReaction(int _rtype, int firstIndex, int secondIndex);
        void addReaction(int _rtype, int index);
    
        //  get score and change its own currScore value 
        void getScore (ScoreFunc&, double*** targetData,int numNodes, int x, bool print);
    
        // return current score generated by getScore
        double getCurrScore();
    
        //description method
        void description(int time);
    
        //cell's equation operator
        bool operator== (Cell& aCell);
    
    
        //generate time courses and put them in currData
        void generateTimeCourses(double*** targetData,int numTargetNodes, int x);
        
    
        //pring time course to a file with the name of a string parameter
        void printCurrDataToAFile(std::string name, int time);
    
    
        //generate regulatory relationships
        void genRegulatoryRelationships();
    
        //find motifs consisting 1, 2 or 3 genes
        void findMotifs();
    
        //find and print single-gene motifs, returning a string containing the motifs' matrice
        std::string findSingleMotifs(int numberOfGenes, int* indiceOfGenes);

        //find and print two-gene motifs, returning a string containing the motifs' matrice
        std::string findDoubleMotifs(int numberOfGenes, int* indiceOfGenes);

        //find and print three-gene motifs, returning a string containing the motifs' matrice
        std::string findTripleMotifs(int numberOfGenes, int* indiceOfGenes);
    
    
    
        //get nodes vector
        std::vector<Node*>* getNodesVector();
    
        //get rlist vector
        std::vector<Reaction*>* getRlistVector();

		//get inputIndice vector
		std::vector<int>* getInputIndiceVector();
        
        //add ranking to rankings vector
        void addRanking(int ranking);
    
    private:

        //  The degradation rate of a protein is modified
        void mut_deg_prot();

		//	A kinetic constant of one action is modified
		void mut_kin_const();

		//	A new gene is created
		void mut_add_gene();

		//	A new interaction between a protein and a gene or gene/protein complex is introduced
		void mut_add_regu();

		//	A post transcriptional modification is added
		void mut_add_postmod();

    private:
    
        //number of inducer
        int numInducer;
    
        //number of input sets
        int numInputSets;
    
        //current data generated by Runge-Kutta method, 3-d means that it contains several sets of time courses
        double*** currData;

        //regulatory relationship matrix
        int** regulatoryMatrix;
    
        //durrent score generated with currData using Score function
        double currScore;

		//	check if a node has been added previously
		bool existsNode (const Node& node);
		
		//	check if a reaction has been added previously
		bool existsReaction (const Reaction& rxn);

        //	Global Storage
        vector<Node*> nodes;//contain all the species within a cell
        vector<Reaction*> rlist;	//	document operations made to develop the network
        std::vector<Motif*> motifs;     //contain all motifs within a cell
    
        vector<int> inputIndice;   //indice of input sequences, including inducers and proteins, for scoring function
        std::vector<int> rankings;  //vector to store every ranking of this cell
};
}   //namespace ustc
#endif
