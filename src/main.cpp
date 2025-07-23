#include "Simulation/emfdtd.h"

int main() {

    ems::initSPDlog(spdlog::level::debug);

    emfdtd sim({170, 170, 170});
    sim.update(500);
}
