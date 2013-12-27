#include "gui.h"

void GUI::OnLoop() {
    SDL_Delay(2);
    perform_action_cont();
    canvas.refresh();
    canvas.run_spawners();
    canvas.run_enemies();
}
