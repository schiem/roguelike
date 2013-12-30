#include <gui.h>

void GUI::OnLoop() {
    perform_action_cont();
    if(game.is_initialized()) {
        game.refresh();
        game.run_spawners();
        game.run_enemies();
    }
}
