#include <gui.h>

void GUI::OnLoop() {
    perform_action_cont();
    canvas.refresh();
    SDL_Delay(2);
    canvas.run_spawners();
    canvas.run_enemies();
}
