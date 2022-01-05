#include "StudentAI.h"
#include <random>
#include "math.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>




//The following part should be completed by students.
//The students can modify anything except the class name and exisiting functions and varibles.
StudentAI::StudentAI(int col, int row, int p)
	:AI(col, row, p)
{
    board = Board(col,row,p);
    board.initializeGame();
    player = 2;
}

Move StudentAI::GetMove(Move move)
{
    if (move.seq.empty())
    {
        player = 1;
    } else{
        board.makeMove(move,player == 1?2:1);
    }
    // dont show chosen move
    // return MCTS_main_loop(board, 50, 10, move);
    
    // show chosen move
    time_t startTime = time(NULL);
    Move returnMove = MCTS_main_loop(board, startTime, 10, move);
    cout << "---------------chosen move = " << returnMove.toString() << endl;
    return returnMove;


    // vector<vector<Move> > moves = board.getAllPossibleMoves(player);
    // int i = rand() % (moves.size());
    // vector<Move> checker_moves = moves[i];
    // int j = rand() % (checker_moves.size());
    // Move res = checker_moves[j];
    // board.makeMove(res,player);
    // return res;
}


// DO NOT MODIFY ABOVE CODE ?
// M = number of rows
// N = number of columns
// P = number of rows occupied by pieces in the initial state
// Q = number of unoccupied rows that separate the two sides in the initial state
// ./main {col} {row} {p} m {start_player (0 or 1)}
// ./main 10 10 2 m 0


bool StudentAI::resources_left(const time_t startTime, int timeLimit_sec) {

    // We have 8 minutes for this project.
    // Hence, should set appropriate time limits for each MCTS loop.

    time_t currentTime = time(NULL);
    double time_to_compute = (double)(currentTime - startTime);
    // if there is more time left than it takes to finish, true (go ahead and run)
    if (timeLimit > time_to_compute)
        return true;
    return false;
}

//  ---->SELECT---->EXPAND---->SIMULATE---->BACKUP---
//  |                                                |
//  L--<---------------------------------------------
Move StudentAI::MCTS_main_loop (const Board& current_board, const time_t startTime, int timeLimit_sec, Move m) {
    this->root = new MCNode(current_board, NULL, m, player);
    while (resources_left(startTime, timeLimit_sec)) {
        MCNode* leaf = this->traverse(root);
        // win or lose?
        bool simulation_result = this->rollout(leaf);

        // update nodes along the path with the result
        // (add 1 to times seen & either add 1 to wins if won, or don't if lost)
        this->backpropagate(leaf, simulation_result);
        --time;
    }
    return get_best_child(root)->move;

}


MCNode* StudentAI::traverse(MCNode* node) 
{
    // 1. loop until an unexpanded node is found.
    // 2. Check if it is leaf node
    // 3. Expand node by adding its children.
    // 4. Return the expanded node.

    while(node->is_fully_expanded) {
        node = node->best_uct();
    }
    vector<vector<Move> > PossibleMoves = node->getValue().getAllPossibleMoves(node->player == 1?2:1);

    if(!PossibleMoves.empty()) // Not a terminal node
    {
        for(int i=0; i<PossibleMoves.size(); i++)
        {
            for(int j=0; j<PossibleMoves[i].size(); j++) 
            {
                Board b = node->getValue();
                b.makeMove(PossibleMoves[i][j], node->player == 1?2:1);
                node->addChild(b, PossibleMoves[i][j]);

                //cout << PossibleMoves[i][j].toString() << endl;
                //b.showBoard();
            }
        }
        node->is_fully_expanded = true;
    }
    
    // return the result of traversing or No possible move for player (leaf node)
    return node;
}


bool StudentAI::is_terminal(MCNode* node) {
    // if either color has won, terminal state.
    if(node->getValue().isWin(0) > 0) {
        return true;
    }
    return false;
}


bool StudentAI::rollout(MCNode* node) 
{
    while (!is_terminal(node)) {
        node = rollout_policy(node);
        
        // uncomment to watch the ai:
        // node->getValue().showBoard();
        // std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    // is this move a win for me?
    //cout << "Result: " << 
    return node->getValue().isWin(player);
}


MCNode* StudentAI::rollout_policy(MCNode* node) 
{
    //cout << node->player << " <<< player turn\n";
    vector<vector<Move> > moves = node->getValue().getAllPossibleMoves(node->player == 1?2:1);
    int i = rand() % (moves.size());
    vector<Move> checker_moves = moves[i];
    
    int j = rand() % (checker_moves.size());
    Move res = checker_moves[j];
    Board b = node->getValue();
    b.makeMove(res,node->player == 1?2:1);
    MCNode* result_node = new MCNode(b, node, res, (node->player == 1?2:1));
    return result_node;
}
  

void StudentAI::backpropagate(MCNode* node, bool simulation_result)
{
    node->update_node(simulation_result);
    if(node == root) 
    {
        return;
    }
    else
    {
        backpropagate(node->parent, simulation_result);
    }
}


MCNode* StudentAI::get_best_child(MCNode* node) {
    return node->best_child();
}



 

  

  



