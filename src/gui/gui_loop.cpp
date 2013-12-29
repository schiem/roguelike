#include <gui.h>

void GUI::OnLoop() {
    perform_action_cont();
    game.refresh();
    game.run_spawners();
    game.run_enemies();
}
