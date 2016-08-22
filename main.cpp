


// Jopnal.hpp contains all engine functionality.
#include <Jopnal/Jopnal.hpp>

// Standard main() can be used, as long as jopnal-main.lib has been linked.
int main(int argc, char* argv[])
{
    JOP_ENGINE_INIT("MyProject", argc, argv);

#if 0
#include "WRBALLSCENE.hpp"
    jop::Engine::createScene<WRBALLSCENE>();
#endif

#if 0
    //FAILURE
#include "CONVEYORBELT.hpp"
    jop::Engine::createScene<CONVEYORSCENE>();
#endif

#if 0
#include "BRIDGESCENE.hpp"
    jop::Engine::createScene<BRIDGESCENE>();
#endif

#if 1
#include "CARTERRAINSCENE.hpp"
    jop::Engine::createScene<CARTERRAINSCENE>();
#endif

    // Finally run the main loop. The program can be closed by clicking the window red X button.
    return JOP_MAIN_LOOP;
}