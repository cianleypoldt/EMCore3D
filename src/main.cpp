#include "Simulation/emfdtd.h"

int main() {

    ems::initSPDlog(spdlog::level::debug);

    emfdtd sim({100, 100, 100});
    sim.update(10);
}
