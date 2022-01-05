#ifndef STUDENTAI_H
#define STUDENTAI_H
#include "AI.h"
#include "Board.h"
#include "MCNode.h"

#pragma once

//The following part should be completed by students.
//Students can modify anything except the class name and exisiting functions and varibles.
class StudentAI :public AI
{
public:
    Board board;
	StudentAI(int col, int row, int p);
	virtual Move GetMove(Move board);
    

	// DO NOT CHANGE ABOVE CODE
public:
    MCNode* root;
private:
    Move MCTS_main_loop (const Board& current_board, const time_t startTime, int timeLimit_sec, Move m);
    MCNode* get_best_child(MCNode* node);
    MCNode* traverse(MCNode* node); 
    MCNode* rollout_policy(MCNode* node);

    bool resources_left(const time_t startTime, int timeLimit_sec);
    bool is_terminal(MCNode* node);
    bool rollout(MCNode* node);
  
    void backpropagate(MCNode* node, bool simulation_result);
	
};

#endif //STUDENTAI_H
