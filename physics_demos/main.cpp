// Jopnal.hpp contains all engine functionality.
#include <Jopnal/Jopnal.hpp>

// Standard main() can be used, as long as jopnal-main.lib has been linked.
int main(int argc, char* argv[])
{
    JOP_ENGINE_INIT("MyProject", argc, argv);

    //Change these from 0 to 1 to test them

#if 1
#include "WRBALLSCENE.hpp"
    jop::Engine::createScene<WRBALLSCENE>();
#endif

#if 0
#include "BRIDGESCENE.hpp"
    jop::Engine::createScene<BRIDGESCENE>();
#endif

#if 0
#include "CARTERRAINSCENE.hpp"
    jop::Engine::createScene<CARTERRAINSCENE>();
#endif

    //The demo can be exited by pressing escape
    class EventHandler : public jop::WindowEventHandler
    {
    public:
        void keyPressed(const int key, const int, const int mods) override
        {
            if (key == jop::Keyboard::Escape)
                jop::Engine::Exit();
        }
    };

    // Finally run the main loop.
    return JOP_MAIN_LOOP;
}