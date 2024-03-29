#include "position.h"

#include "board.h"
#include "move.h"
#include "movecheck.h"
#include "movegenerators.h"
#include "logger.h"

#include <cmath>
#include <sstream>

const std::array<int, NO_PIECES> drawMaterial = {0, 10, 1, 1, 10, 10, 0, 10, 10, 1, 1, 10, 0};

extern Logger logger;

Position::Position()
   :toMove(WHITE),
    whcstl(SHORT_CASTLE_MOVE | LONG_CASTLE_MOVE),
    blcstl(SHORT_CASTLE_MOVE | LONG_CASTLE_MOVE),
    enPassantPosition(0),
    underCheck(false),
    prev(nullptr),
    result(ONGOING),
    searchBackRepetitions(false),
    halfMoveClock(0),
    fullMoveCounter(1),
    whiteKingPos(Board::Not2Ind("e1")),
    blackKingPos(Board::Not2Ind("e8"))
{
    positionHash = CalculatePositionHash();
}

Position::Position(Position& pr, Move *m, int promo)
   :prev(&pr)
{
    std::copy(prev->squares.begin(), prev->squares.end(), squares.begin());
    whiteKingPos = pr.whiteKingPos;
    blackKingPos = pr.blackKingPos;
    toMove = prev->toMove * (-1);
    whcstl = prev->whcstl;
    blcstl = prev->blcstl;

    squares[m->To()] = squares[m->From()];
    squares[m->From()] = EMPTY_SQUARE;
    if(m->Type() & EN_PASSANT_MOVE){
        if(Board::column(m->From()) < Board::column(m->To())){
            squares[m->From() + 1] = 0;
        }
        else{
            squares[m->From() - 1] = 0;
        }
    }
    if(m->Type() & SHORT_CASTLE_MOVE){
        squares[m->To() - 1] = squares[m->To() + 1];
        squares[m->To() + 1] = EMPTY_SQUARE;
    }
    if(m->Type() & LONG_CASTLE_MOVE){
        squares[m->To() + 1] = squares[m->To() - 2];
        squares[m->To() - 2] = EMPTY_SQUARE;
    }
    if(m->Type() & PROMOTION_MOVE){
        if(squares[m->To()] < 0){
            squares[m->To()] = -promo;
        }
        else{
            squares[m->To()] = promo;
        }
    }

    if(whcstl & SHORT_CASTLE_MOVE)
        if(m->From() == Board::Not2Ind("e1") || GetSquareValue(Board::Not2Ind("h1")) != WHITE_ROOK)
            whcstl -= SHORT_CASTLE_MOVE;

    if(whcstl & LONG_CASTLE_MOVE)
        if(m->From() == Board::Not2Ind("e1") || GetSquareValue(Board::Not2Ind("a1")) != WHITE_ROOK)
            whcstl -= LONG_CASTLE_MOVE;

    if(blcstl & SHORT_CASTLE_MOVE)
        if(m->From() == Board::Not2Ind("e8") || GetSquareValue(Board::Not2Ind("h8")) != BLACK_ROOK)
            blcstl -= SHORT_CASTLE_MOVE;

    if(blcstl & LONG_CASTLE_MOVE)
        if(m->From() == Board::Not2Ind("e8") || GetSquareValue(Board::Not2Ind("a8")) != BLACK_ROOK)
            blcstl -= LONG_CASTLE_MOVE;

    if(m->Type() & PAWN_DOUBLE_MOVE)
        enPassantPosition = (m->From() + m->To()) / 2;
    else
        enPassantPosition = -1;

    if(m->From() == whiteKingPos)
        whiteKingPos = m->To();
    if(m->From() == blackKingPos)
        blackKingPos = m->To();

    fullMoveCounter = prev->fullMoveCounter;
    if(toMove == WHITE)
        fullMoveCounter++;

    if(m->Type() < PAWN_MOVE){ // only regular moves and castles (no captures)
        halfMoveClock = prev->halfMoveClock + 1;
        searchBackRepetitions = true;
    }
    else{
        halfMoveClock = 0;
        searchBackRepetitions = false;
    }
    underCheck = CheckCheck();
    positionHash = CalculatePositionHash();
    result = CheckEndings();
}

Position::Position(std::stringstream& strFen){
    std::string fen;
    strFen >> fen;

#ifdef DEBUG
    // very basic fen checking for more convinient messages during testing
    long int noSlashes = std::count(fen.begin(), fen.end(), '/');
    assert(noSlashes == 7 && "Invalid fen");
#endif  

    unsigned int index = 0;
    for(int i = 7; i >= 0; i--){
        int j = 0;
        while(fen[index] != '/' && index < fen.length()){
            switch(fen[index]){
                case 'K':
                    squares[mailbox[8 * i + j]] = WHITE_KING;
                    whiteKingPos = mailbox[8 * i + j];
                    break;
                case 'k':
                    squares[mailbox[8 * i + j]] = BLACK_KING;
                    blackKingPos = mailbox[8 * i + j];
                    break;
                case 'Q':
                    squares[mailbox[8 * i + j]] = WHITE_QUEEN;
                    break;
                case 'q':
                    squares[mailbox[8 * i + j]] = BLACK_QUEEN;
                    break;
                case 'R':
                    squares[mailbox[8 * i + j]] = WHITE_ROOK;
                    break;
                case 'r':
                    squares[mailbox[8 * i + j]] = BLACK_ROOK;
                    break;
                case 'B':
                    squares[mailbox[8 * i + j]] = WHITE_BISHOP;
                    break;
                case 'b':
                    squares[mailbox[8 * i + j]] = BLACK_BISHOP;
                    break;
                case 'N':
                    squares[mailbox[8 * i + j]] = WHITE_KNIGHT;
                    break;
                case 'n':
                    squares[mailbox[8 * i + j]] = BLACK_KNIGHT;
                    break;
                case 'P':
                    squares[mailbox[8 * i + j]] = WHITE_PAWN;
                    break;
                case 'p':
                    squares[mailbox[8 * i + j]] = BLACK_PAWN;
                    break;
                default:;
            }
            if(isdigit(fen[index])){
                int x = (int)(fen[index]) - '0';
                for(; x> 0; x--){
                    squares[mailbox[8 * i + j]] = EMPTY_SQUARE;
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

    // color to move
    strFen >> fen;
    fen[0] == 'w'? toMove = WHITE : toMove = BLACK;

    // castles
    strFen >> fen;
    whcstl = 0;
    blcstl = 0;
    if(fen.find('K') != std::string::npos)
        whcstl |= SHORT_CASTLE_MOVE;
    if(fen.find('Q') != std::string::npos)
        whcstl |= LONG_CASTLE_MOVE;
    if(fen.find('k') != std::string::npos)
        blcstl |= SHORT_CASTLE_MOVE;
    if(fen.find('q') != std::string::npos)
        blcstl |= LONG_CASTLE_MOVE;

    // en passant position
    strFen >> fen;
    fen[0] == '-' ? enPassantPosition = -1 : enPassantPosition = Board::Not2Ind(fen.substr(0, 2));

    // half move clock
    strFen >> fen;
    halfMoveClock = std::stoi(fen);
    fullMoveCounter = 1;

    // full move counter
    strFen >> fen;
    fullMoveCounter = std::stoi(fen);
    searchBackRepetitions = false;
    prev = nullptr;
    positionHash = CalculatePositionHash();
    underCheck = CheckCheck();
    result = CheckEndings();
}

std::string Position::GetFen() const{
    std::stringstream fen;

    unsigned int index = 0;
    for(int i = 8; i >= 1; i--){
        int emptys = 0;
        for(int j = 1; j <= 8; j++){
            index = Board::ColRow2Ind(j, i);
            if(squares[index] == EMPTY_SQUARE){
                emptys++;
            }
            else{
                if(emptys != 0){
                    fen << emptys;
                    emptys = 0;
                }
                switch(squares[index]){
                    case WHITE_KING:
                        fen << "K";
                        break;
                    case BLACK_KING:
                        fen << "k";
                        break;
                    case WHITE_QUEEN:
                        fen << "Q";
                        break;
                    case BLACK_QUEEN:
                        fen << "q";
                        break;
                    case WHITE_ROOK:
                        fen << "R";
                        break;
                    case BLACK_ROOK:
                        fen << "r";
                        break;
                    case WHITE_BISHOP:
                        fen << "B";
                        break;
                    case BLACK_BISHOP:
                        fen << "b";
                        break;
                    case WHITE_KNIGHT:
                        fen << "N";
                        break;
                    case BLACK_KNIGHT:
                        fen << "n";
                        break;
                    case WHITE_PAWN:
                        fen << "P";
                        break;
                    case BLACK_PAWN:
                        fen << "p";
                        break;
                    default:;
                }
            }
        }
        if(emptys != 0){
            fen << emptys;
        }
        if(i != 1){
            fen << "/";
        }
    }

    // color to move
    fen << " ";
    toMove == WHITE ? fen << "w" : fen << "b";

    // castles
    fen << " ";
    if(whcstl + blcstl == 0){
        fen << "-";
    }
    else{
        if(whcstl | SHORT_CASTLE_MOVE)
            fen << "K";
        if(whcstl | LONG_CASTLE_MOVE)
            fen << "Q";
        if(blcstl | SHORT_CASTLE_MOVE)
            fen << "k";
        if(blcstl | LONG_CASTLE_MOVE)
            fen << "q";
    }
    // en passant position
    fen << " ";
    enPassantPosition == -1 ? fen << "-" : fen << Board::Ind2Not(enPassantPosition);

    // half move clock
    fen << " ";
    fen << halfMoveClock;

    // full move counter
    fen << " ";
    fen << fullMoveCounter;

    return std::string(fen.str());
}

Position::~Position(){}

void Position::ShowBoard(std::ostream& out) const{
    out << std::endl;
    out << "  +---+---+---+---+---+---+---+---+\n";
    for(int row = 8; row > 0; row--){
        out << row << " ";
        for(int col = 1; col <= 8; col++){
            out << "| " << GetPiece(col, row) << " ";
        }
        out << "|";
        out << "\n  +---+---+---+---+---+---+---+---+\n";
    }
    out << "    a   b   c   d   e   f   g   h\n\n";
    return;
}
std::ostream& Position::ShowTinyBoard(std::ostream& out) const{
    for(int row = 8; row > 0; row--){
        out << " ";
        for(int col = 1; col <= 8; col++){
            out << GetPiece(col, row);
        }
        out << '\n';
    }
    return out;
}

Position* Position::MakeMove(Move *toExecute){
    Move *expectedmove = CheckIfMoveFullLegal(toExecute);
    if(nullptr != expectedmove)
    {
        Position *newposition = new Position(*this, expectedmove, toExecute->Promo());
        delete expectedmove;
        return newposition;
    }
    return nullptr;
}

int Position::MakeSoftMove(Move *toExecute){
    int takenPiece = squares[toExecute->To()];
    squares[toExecute->To()] = squares[toExecute->From()];
    squares[toExecute->From()] = EMPTY_SQUARE;
    if(toExecute->Type() & EN_PASSANT_MOVE){
        if(toMove == WHITE){
            squares[enPassantPosition - 10] = EMPTY_SQUARE;
        }
        else{
            squares[enPassantPosition + 10] = EMPTY_SQUARE;
        }
    }
    if(toExecute->From() == whiteKingPos){
        whiteKingPos = toExecute->To();
    }
    if(toExecute->From() == blackKingPos){
        blackKingPos = toExecute->To();
    }
    return takenPiece;
}

void Position::MakeSoftBack(Move *toExecute, int takenPiece){
    squares[toExecute->From()] = squares[toExecute->To()];
    squares[toExecute->To()] = takenPiece;
    if(toExecute->Type() & EN_PASSANT_MOVE){
        if(toMove == WHITE){
            squares[enPassantPosition - 10] = BLACK_PAWN;
        }
        else{
            squares[enPassantPosition + 10] = WHITE_PAWN;
        }
    }
    if(toExecute->To() == whiteKingPos){
        whiteKingPos = toExecute->From();
    }
    if(toExecute->To() == blackKingPos){
        blackKingPos = toExecute->From();
    }
}

Move* Position::CheckIfMoveFullLegal(Move* checkedmove, bool pseudoLegalWarranty){
    int PieceColor = GetSquareColor(checkedmove->From());
    if(toMove != PieceColor){
        logger << LogDest(LOG_ERROR) << "wrong piece chosen (wrong color)\n";
        return nullptr;
    }
    Move *expectedmove = nullptr;
    if(pseudoLegalWarranty){
        expectedmove = new Move(*checkedmove);
    }
    else{
        expectedmove = CheckIfMovePseudoLegal(checkedmove->From(), checkedmove->To());
        if(expectedmove == nullptr){
            return nullptr;
        }
    }
    if((expectedmove->Type()) & PROMOTION_MOVE){
        if(checkedmove->Promo() == 0){
            delete expectedmove;
            logger << LogDest(LOG_BAD_INPUT) << "no promo char entered\n";
            return nullptr;
        }
    }
    if(nullptr != expectedmove)
    {
        int takenPiece = MakeSoftMove(expectedmove);
        int kingPos = 0;
        toMove == WHITE ? kingPos = whiteKingPos : kingPos = blackKingPos;
        bool ownCheckAfter = false;
        /*if(InBetweenEmpty(*this, kingPos, expectedmove->From(), true, true) || Board::row(kingPos) == Board::row(expectedmove->From())){
            ownCheckAfter = IsPlaceAttacked(kingPos, -toMove);
        }//*/
        ownCheckAfter = IsPlaceAttacked(kingPos, -toMove);
        MakeSoftBack(expectedmove, takenPiece);
        if(ownCheckAfter){
            delete expectedmove;
            return nullptr;
        }
        return expectedmove;
    }
    return nullptr;
}

Move* Position::CheckIfMovePseudoLegal(int from, int to) const{
    Move *expected = MoveCheckHandler::CheckMove(*this, from, to);
    return expected;
}

bool Position::CheckCheck(){
    if(toMove == WHITE){
        // when white player is moving the white king safety need to be checked
        if(IsPlaceAttacked(whiteKingPos, -toMove)){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        if(IsPlaceAttacked(blackKingPos, -toMove)){
            return true;
        }
        else{
            return false;
        }
    }
}

std::list<Move> Position::GenerateAllLegalMoves(bool searchAtLeastOne){
    std::list<Move> moves = AllMovesGenerator::GenerateMoves(*this);
    std::list<Move> results;
    Move *temp;
    auto it = moves.begin();
    bool found = false;
    while(it != moves.end()){
        if((temp = CheckIfMoveFullLegal(&(*it), false))){
            found = true;
            delete temp;
            results.push_back(*it);
        }
        if(found && searchAtLeastOne){
            return results;
        }
        ++it;
    }
    return results;
}

GameResult Position::CheckEndings(){
    // 50-move rule
    if(halfMoveClock >= 100){
        return GameResult::DRAW;
    }


    // Repetition draw
    int count = 0;
    Position *current = this;
    while(true){
        if(current->positionHash == this->positionHash){
            count++;
            if(count>= 3){
                logger << LogDest(LOG_ANALYSIS) << "draw (repetition)\n";
                return GameResult::DRAW;
            }
        }
        if(current->searchBackRepetitions == false){
            break;
        }
        if(nullptr == current->prev){
            break;
        }
        else{
            current = current->prev;
        }
    }
    std::list<Move> possiblemoves = GenerateAllLegalMoves(true);
    if(possiblemoves.size() == 0){
        if(underCheck){ // Checkmate
            if(toMove == WHITE){
                logger << LogDest(LOG_ANALYSIS) << "black win (checkmate)\n";
                return GameResult::BLACK_WIN;
            }
            else{
                logger << LogDest(LOG_ANALYSIS) << "white win (checkmate)\n";
                return GameResult::WHITE_WIN;
            }
        }
        else{ // Stalemate
            logger << LogDest(LOG_ANALYSIS) << "draw (stalemate)\n";
            return GameResult::DRAW;
        }
    }

    int sqmaterial[2] = {0, 0}; // on white and black squares
    int colmaterial[3] = {0, 0, 0};
    int col = 0;
    for(int i = 0; i < 64; i++){
        if(i % 8 != 0){
            col = 1 - col;
        }
        int ind = mailbox[i];
        sqmaterial[col & 1] += drawMaterial[LookUpTableIndex(squares[ind])];
        colmaterial[GetSquareColor(ind) + 1] += drawMaterial[LookUpTableIndex(squares[ind])];
    }
    if(colmaterial[BLACK + 1] + colmaterial[WHITE + 1] < 2){
        logger << LogDest(LOG_ANALYSIS) << "draw (material)\n";
        return DRAW;
    }
    if(colmaterial[BLACK + 1] + colmaterial[WHITE + 1] == 2 && colmaterial[BLACK + 1] == colmaterial[WHITE + 1]){
        if(sqmaterial[0] != sqmaterial[1]){
            bool isknight = false;
            for(int i = 0; i < 64; i++){
                int ind = mailbox[i];
                if(squares[ind] == WHITE_KNIGHT || squares[ind] == BLACK_KNIGHT){
                    isknight = true;
                }
            }
            if(isknight == false){
                return DRAW;
                logger << LogDest(LOG_ANALYSIS) << "draw (material)\n";
            }
        }
    }
    return GameResult::ONGOING;
} 

long long Position::CalculatePositionHash(){
    long long value = toMove + 313;
    for(int i = 0; i < 64; i++){
        value *= 313;
        value += GetSquareValue(mailbox[i]) + NO_PIECES;
        value %= 1000000009 + 7;
    }
    return value;
}

char Position::GetPiece(int column, int row) const{
    //logger << LogDest(LOG_DEBUG) << "Get piece     " << GetSquareValue(column, row) << "   " << column << "  " << row << "\n";
    return Board::GetPieceSymbol(GetSquareValue(column, row));
}

int Position::GetSquareValue(int column, int row) const{
    if(column < 1 || column > 8 || row < 1 || row > 8){
        logger << "function    Position::GetSquareValue error    " << column << " " << row << "\n";
        exit(1);
    }
    //logger << LogDest(LOG_DEBUG) << "Get Square value    from   "  << 11 + 10 * row + column << "     " << squares[11 + 10 * row + column] << std::endl;
    return squares[11 + 10 * row + column];
}

int Position::GetSquareColor(int column, int row) const{
    int val = GetSquareValue(column, row);
    if(val == OUTSIDE_BOARD){
        return OUTSIDE_BOARD;
    }
    else{
        return sgn(val);
    }
}

char Position::GetPiece(int index) const{
    return Board::GetPieceSymbol(squares[index]);
}

int Position::GetSquareValue(int index) const{
    return squares[index];
}

int Position::GetSquareColor(int index) const{
    int val = GetSquareValue(index);
    if(val == OUTSIDE_BOARD){
        return OUTSIDE_BOARD;
    }
    else{
        return sgn(squares[index]);
    }
}

bool Position::IsPlaceAttacked(int attackedplace, int attackingcolor) const{
    bool answer = false;
    for(int i = 0; i < 64 && answer == false; i++){
        int ind = mailbox[i];
        if(GetSquareColor(ind) == attackingcolor){
            Move *m = MoveCheckHandler::CheckMove(*this, ind, attackedplace);
            if(m != nullptr){
                delete m;
                return true;
            }
        }
    }
    return answer;
}