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
    OnNodeInit();
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
    OnNodeInit();
    if(realnode){
        CheckEndings();
    }
}

Node::Node(std::string fen){
    std::cout << '\n' << '\n';
    _board = new Board(this);
    fen.erase(0, 1);
    std::cout << "full fen    #" << fen << '#' << '\n';
    int ind = fen.find(' ');
    std::cout << ind << '\n';
    std::string fentemp = fen.substr(0, ind);
    fen.erase(0, ind + 1);
    std::cout << "first cut   #" << fentemp << '#' << '\n';
    std::cout << "Board const 1\n";
    
    unsigned int index = 0;
    for(int i = 7; i >= 0; i--){
        int j = 0;
        while(fentemp[index] != '/' && index < fentemp.length()){
            switch(fentemp[index]){
                case 'K':
                    _board->_squares[mailbox[8 * i + j]] = WHITE_KING;
                    _board->_whiteKingPos = mailbox[8 * i + j];
                    break;
                case 'k':
                    _board->_squares[mailbox[8 * i + j]] = BLACK_KING;
                    _board->_blackKingPos = 8 * i + j;
                    break;
                case 'Q':
                    _board->_squares[mailbox[8 * i + j]] = WHITE_QUEEN;
                    break;
                case 'q':
                    _board->_squares[mailbox[8 * i + j]] = BLACK_QUEEN;
                    break;
                case 'R':
                    _board->_squares[mailbox[8 * i + j]] = WHITE_ROOK;
                    break;
                case 'r':
                    _board->_squares[mailbox[8 * i + j]] = BLACK_ROOK;
                    break;
                case 'B':
                    _board->_squares[mailbox[8 * i + j]] = WHITE_BISHOP;
                    break;
                case 'b':
                    _board->_squares[mailbox[8 * i + j]] = BLACK_BISHOP;
                    break;
                case 'N':
                    _board->_squares[mailbox[8 * i + j]] = WHITE_KNIGHT;
                    break;
                case 'n':
                    _board->_squares[mailbox[8 * i + j]] = BLACK_KNIGHT;
                    break;
                case 'P':
                    _board->_squares[mailbox[8 * i + j]] = WHITE_PAWN;
                    break;
                case 'p':
                    _board->_squares[mailbox[8 * i + j]] = BLACK_PAWN;
                    break;
                default:;
            }
            if(isdigit(fentemp[index])){
                int x = (int)(fentemp[index]) - '0';
                for(; x > 0; x--){
                    _board->_squares[mailbox[8 * i + j]] = EMPTY_SQUARE;
                    j++;
                }
            }
            else{
                j++;
            }
            index++;
        }
        index++;
    }

    std::cout << "Board const 2\n";
    std::cout << fen << '\n';
    if(fen[0] == 'w'){
         _toMove = WHITE;
    }
    else{
        _toMove = BLACK;
    } 
    fen.erase(0, 2);
    std::cout << fen << "!\n";
    fentemp = fen.substr(0, fen.find(' '));
    _whcstl = 0;
    _blcstl = 0;
    if(fentemp.find('K') != std::string::npos) _whcstl |= SHORT_CASTLE_MOVE;
    if(fentemp.find('Q') == std::string::npos) _whcstl |= LONG_CASTLE_MOVE;
    if(fentemp.find('k') == std::string::npos) _blcstl |= SHORT_CASTLE_MOVE;
    if(fentemp.find('q') == std::string::npos) _blcstl |= LONG_CASTLE_MOVE;
    fen.erase(0, fentemp.length() + 1);
    std::cout << fen << "!\n";
    if(fen[0] == '-'){
        _enPassant = -1;
    }
    else{
        _enPassant = Not2Ind(fen.substr(0, 2));
    }
    fen.erase(0,fen.find(' ') + 1);
    std::cout << fen << "!\n";
    if(fen[1] != ' '){
        halfMoveClock = 10 * ((int)(fen[0]) - '0') + (int)(fen[1]) - '0';
    }
    else{
        halfMoveClock = (int)(fen[0]) - '0';
    }
    fen.erase(0, fen.find(' ') + 1);
    std::cout << fen << "!\n";
    fullMoveCounter = 1;
    for(uint i = 0; i < fen.length(); i++){
        fullMoveCounter *= 10;
        fullMoveCounter += (int)(fen[i]) - '0';
    }
    OnNodeInit();
    std::cout << "fen reading finished" << std::endl;
}

void Node::OnNodeInit(){
    CalculatePositionHash();
    searchBackRepetitions = false;
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