#include <gui.h>

void GUI::OnLoop() {
    perform_action_cont();
    canvas.refresh();
    canvas.run_spawners();
    canvas.run_enemies();
}
