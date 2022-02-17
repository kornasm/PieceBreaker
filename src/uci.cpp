#include "uci.h"
#include "search.h"
#include "functions.h"
#include "evaluate.h"
#include "node.h"
#include "position.h"

#include <iostream>

void Uci::loop(){
    std::string command;
    while(true){
        std::cout << ">";
        std::cin >> command;

        if(command == "uci")
            std::cout << "uci ok\n";
        if(command == "isready"){
            std::cout << "readyok\n";
        }
        if(command == "position"){
            std::string option;
            std::cin >> option;
            if(option == "fen"){
                std::string fen;
                std::cin >> fen;
                SearchTree::Init(fen);
            }
            if(option == "startpos"){
                //not implemanted
            }
        }

        if(command == "go"){
            SearchTree *tree = SearchTree::GetInstance();
            int depth;
            std::cin >> depth;
            if(depth == 0){
                std::cout << "depth must be positive\n";
                continue;
            }
            tree->Search(depth);
        }

        if(command == "ucinewgame"){
            SearchTree::Init();
        }

        if(command == "stop"){
            // nothing yet
        }

        if(command == "ponderhit"){
            // nothing yet
        }

        if(command == "quit"){
            break;
        }


        // not uci

        if(command == "board"){
            SearchTree *tree = SearchTree::GetInstance();
            tree->ShowBoard();
        }
        if(command == "eval"){
            SearchTree *tree = SearchTree::GetInstance();
            tree->GetEntryNode()->Evaluate();
        }
        if(command == "hash"){
            SearchTree *tree = SearchTree::GetInstance();
            std::cout << tree->GetEntryNode()->position->GetPositionHash();
        }
    }
    return;
}

/*Node *newgame = new Node();
    Node *current = newgame;
    int gamestate = 1;
    while(gamestate){
        std::cout << "Enter command:" << std::endl;
        std::string command;
        std::cin >> command;
        std::cout << h(command) << std::endl;
        switch(h(command)){
            case 289517988:{ // "move"
                std::string movestr;
                std::cin >> movestr;
                if(movestr.length() >= 4){
                    Move *insertedmove = Move::String2Move(movestr);
                    if(insertedmove != NULL){
                        if(current->CheckMove(insertedmove)){
                            std::cout << "ok\n";
                            current = current->children.back();
                        }
                        else{
                            std::cout << "not ok\n"; 
                        }
                        delete insertedmove;
                    }
                }
                else{
                    std::cout << "incorrect move syntax\n";
                }
                break;
            }
            case 40093753:{ // moves}
                std::list<Move> *moves = current->position->GenerateAllLegalMoves();
                PrintMoveList(moves);
                delete moves;
                break;
            }
            case 9897063:{ //fen
                std::string fen;
                std::getline(std::cin, fen);
                delete newgame;
                newgame = new Node(fen);
                current = newgame;
                break;
            }
            case 169095377:{ // info
                std::cout << "white king           " << Ind2Not(current->position->whiteKingPos) << '\n';
                std::cout << "black king           " << Ind2Not(current->position->blackKingPos) << '\n';
                std::cout << "white castl          " << current->position->whcstl << '\n';
                std::cout << "black castl          " << current->position->blcstl << '\n';
                std::cout << "en passant           " << Ind2Not(current->position->EnPassantPos() << '\n';
                std::cout << "is check             " << current->position->underCheck << '\n';
                std::cout << "half move clock      " << current->position->halfMoveClock << '\n';
                std::cout << "pos hash             " << current->position->PositionHash() << '\n';
                std::cout << "to move              " << current->position->ToMove() << '\n';
                break;
            }
            case 32144: { // go
                int depth;
                std::cin >> depth;
                //std::unique_ptr<SearchTree> tree = std::make_unique<SearchTree>(SearchTree::GetInstance());
                SearchTree* tree = SearchTree::GetInstance();
                tree->SetEntry(current);
                tree->Search(depth);
                std::cout << "TREE DELETE CALLED\n\n\n";
                delete tree;
                break;
            }
            case 49546663: { // eval
                std::cout << "Eval: " << Evaluator::Evaluate(*(current->position)) << '\n';
            }
        }
    }
    delete newgame;
    cleanup();
    std::cout << Node::count << '\n';//*/