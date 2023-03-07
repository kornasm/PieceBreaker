#include "src/move.h"
#include "src/movegenerators.h"
#include "src/node.h"
#include "src/search.h"
#include "src/evaluate.h"
#include "src/uci.h"
#include "src/logger.h"
#include "src/input_provider.h"
#include "src/movecheck.h"

#include <string>
#include <optional>

#include <boost/program_options.hpp>

extern Logger logger;

namespace PieceBreaker{
    void Init(int argc, char **argv){

        namespace po = boost::program_options;
        po::options_description desc("Available options");
        desc.add_options()
            ("help,h", "Show this help message")
            ("input-source,i", po::value<std::string>(), "specify path to input source (used for testing)")
            ("show-analysis,a", "show additional analysis data")
            ("show-debug,d", "show additional data for debugging")
            ("quiet,q", "don't log any data except those explicitly called (it will overrite -a and -d options");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            logger << LogDest(LOG_ALWAYS) << desc << "\n";
            exit(0);
        }

        std::optional<std::string> file = std::nullopt;
        if (vm.count("input-source"))
            file = vm["input-source"].as<std::string>();
        InputProvider::Init(file);

        if(vm.count("show-analysis"))
            logger.AddLevel(LOG_ANALYSIS);
        if(vm.count("show-debug"))
            logger.AddLevel(LOG_DEBUG);
        if(vm.count("quiet"))
            logger.SetLevel(LOG_QUIET);

        MoveCheckHandler::Init();
        MoveGeneratorHandler::Init();
    }

    void Cleanup(){
        MoveGeneratorHandler::Cleanup();
        MoveCheckHandler::Cleanup();

        InputProvider *ip = InputProvider::GetInstance();
        delete ip;
    }
}

int main(int argc, char **argv){
    PieceBreaker::Init(argc, argv);
    Uci::loop();
    PieceBreaker::Cleanup();
}