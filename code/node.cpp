#include "declarations.h"

extern int mailbox[64];
extern MoveGenerator* generators[NO_PIECES + 1];
int drawMaterial[NO_PIECES] = {0, -10, -1, -1, -10, -10, 0, 10, 10, 1, 1, 10, 0};
Node::Node(){
    _board = new Board(this);
    _toMove = WHITE;
    _whcstl = 3;
    _blcstl = 3;
    _enPassant = -1;
    prev = NULL;
    result = ONGOING;
    underCheck = false;
    halfMoveClock = 0;
    fullMoveCounter = 1;
    CalculatePositionHash();
    searchBackRepetitions = false;
}

Node::Node(Node *pr, Board* b, Move *m, bool realnode){
    prev = pr;
    _board = b;
    b->parentnode = this;
    _toMove = prev->_toMove * (-1);
    _whcstl = prev->_whcstl;
    _blcstl = prev->_blcstl;
    if(pr->_toMove == WHITE){
        if(_whcstl % SHORT_CASTLE_MOVE != 0){
            if(m->From() == Not2Ind("e1") || m->From() == Not2Ind("h1")){
                _whcstl -= SHORT_CASTLE_MOVE / 2;
            }
        }
        if(_whcstl % LONG_CASTLE_MOVE != 0){
            if(m->From() == Not2Ind("e1") || m->From() == Not2Ind("a1")){
                _whcstl -= LONG_CASTLE_MOVE / 2;
            }
        }
    }
    else{
        if(_blcstl % SHORT_CASTLE_MOVE != 0){
            if(m->From() == Not2Ind("e8") || m->From() == Not2Ind("h8")){
                _blcstl -= SHORT_CASTLE_MOVE / 2;
            }
        }
        if(_blcstl % LONG_CASTLE_MOVE != 0){
            if(m->From() == Not2Ind("e8") || m->From() == Not2Ind("a8")){
                _blcstl -= LONG_CASTLE_MOVE / 2;
            }
        }
    }
    if(m->Type() == PAWN_DOUBLE_MOVE){
        _enPassant = (m->From() + m->To()) / 2;
    }
    else{
        _enPassant = -1;
    }//*/
    if(m->From() == b->_whiteKingPos){
        b->_whiteKingPos = m->To();
    }
    if(m->From() == b->_blackKingPos){
        b->_blackKingPos = m->To();
    }
    if(_toMove == WHITE){
        fullMoveCounter++;
    }
    if(m->Type() < PAWN_MOVE){ // only regular moves and castles (no captures)
        halfMoveClock++;
    }
    else{
        halfMoveClock = 0;
    }
    CalculatePositionHash();
    searchBackRepetitions = true;
    if(realnode){
        CheckEndings();
    }
}

Node::~Node(){
    delete _board;
    for(auto nd : children){
        delete nd;   
    }
}
void Node::ShowBoard(){
    _board->ShowBoard();
}

Board* Node::GetBoardPtr(){
    return _board;
}


bool Node::CheckMove(Move *checkedmove, bool execute){
    int PieceColor = _board->GetSquareColor(checkedmove->From());
    if(_toMove != PieceColor){
        std::cout << "wrong piece chosen (wrong color)\n";
        return false;
    }
    Move *expectedmove = _board->CheckMove(checkedmove->From(), checkedmove->To());
    if(NULL != expectedmove)
    {
        //std::cout << "expected move found\n\n";
        if(((expectedmove->Type() & PROMOTION_MOVE) == PROMOTION_MOVE) && checkedmove->Promo() == EMPTY_SQUARE){
            std::cout << "No promo char entered\n";
            sleep(1);
            delete expectedmove;
            return false;
        } 
        //std::cout << "creating new board\n";
        Board *newboard = new Board(_board, expectedmove, checkedmove->Promo());
        Node *newnode = new Node(this, newboard, expectedmove, execute);
        if(_toMove == WHITE){
            if(newboard->IsPlaceAttacked(newboard->_whiteKingPos, newnode->_toMove)){
                std::cout << "king attacked after the move\n";
                delete newnode;
                delete expectedmove;
                return false;
            }
        }
        else{
            if(newboard->IsPlaceAttacked(newboard->_blackKingPos, newnode->_toMove)){
                std::cout << "king attacked after the move\n";
                delete newnode;
                delete expectedmove;
                return false;
            }
        }
        if(execute == true){
            children.push_back(newnode);
        }
        else{
            delete newnode;
        }
        delete expectedmove;
        return true;
    }
    return false;
}

void Node::CheckCheck(){
    if(_toMove == WHITE){
        if(_board->IsPlaceAttacked(_board->_whiteKingPos, -_toMove)){
            underCheck = true;
        }
        else{
            underCheck = false;
        }
    }
    else{
        if(_board->IsPlaceAttacked(_board->_blackKingPos, -_toMove)){
            underCheck = true;
        }
        else{
            underCheck = false;
        }
    }
}

std::list<Move>* Node::GenerateAllLegalMoves(){
    std::list<Move>* moves = AllMovesGenerator::GenerateMoves(this);
    std::list<Move>* results = new std::list<Move>();
    auto it = moves->begin();
    while(it != moves->end()){
        if(CheckMove(&(*it), false)){
            results->push_back(*it);
        }
        ++it;
    }
    delete moves;
    return results;
}

void Node::CheckEndings(){
    if(halfMoveClock >= 50){
        result = GameResult::DRAW;
        return;
    }
    result = GameResult::ONGOING;
    int count = 0; // repetition draw
    std::cout << "checking repetition draw  " << positionHash << '\n';
    Node *current = this;
    while(true){
        std::cout << "  " << current->positionHash << '\n';
        if(current->positionHash == this->positionHash){
            count++;
            if(count >= 3){
                result = GameResult::DRAW;
                std::cout << "repetition draw\n";
                return;
            }
        }
        if(current->searchBackRepetitions == false){
            break;
        }
        if(NULL == current->prev){
            break;
        }
        else{
            current = current->prev;
        }
    }
    std::list<Move>* possiblemoves = GenerateAllLegalMoves();
    if(possiblemoves->size() == 0){
        if(underCheck){
            if(_toMove == WHITE){
                result = GameResult::BLACK_WIN;
            }
            else{
                result = GameResult::WHITE_WIN;
            }
        }
        else{
            result = GameResult::DRAW;
        }
    }
    delete possiblemoves;
    if(result != GameResult::ONGOING){
        return;
    }
    int sqmaterial[2] = {0, 0}; // on white and black squares
    int colmaterial[3] = {0, 0, 0};
    for(int i = 0; i < 64; i++){
        int ind = mailbox[i];
        sqmaterial[ind & 1] += std::abs(drawMaterial[_board->_squares[ind] + SYMBOLS_OFFSET]);
        colmaterial[_board->GetSquareColor(ind) + 1] += drawMaterial[_board->_squares[ind] + SYMBOLS_OFFSET];
        //std::cout << "on   " << (ind & 1) << "   added  " << drawMaterial[_board->_squares[ind] + SYMBOLS_OFFSET] << "   by   " << _board->_squares[ind] <<  "   which is    "  <<  PiecesSymbols[_board->_squares[ind] + SYMBOLS_OFFSET] << "   on place   " << Ind2Not(ind) << '\n';
    }
    if(colmaterial[BLACK + 1] + colmaterial[WHITE + 1] < 2){
        result = DRAW;
        return;
    }
    if(colmaterial[BLACK + 1] + colmaterial[WHITE + 1] == 2 && colmaterial[BLACK + 1] == colmaterial[WHITE + 1]){
        if(sqmaterial[0] == sqmaterial[1]){
            bool isknight = false;
            for(int i = 0; i < 64; i++){
                int ind = mailbox[i];
                if(_board->_squares[ind] == WHITE_KNIGHT || _board->_squares[ind] == BLACK_KNIGHT){
                    isknight = true;
                }
            }
            if(isknight == false){
                result = DRAW;
            }
        }
    }
}

void Node::CalculatePositionHash(){
    long long value = _toMove + 313;
    for(int i = 0; i < 64; i++){
        value *= 313;
        value += _board->GetSquareValue(mailbox[i]) + NO_PIECES;
        value %= 1000000009 + 7;
    }
    positionHash = value;
}