#include "emfdtd/emfdtd.h"

int main() {

    io::init_spdlog(spdlog::level::debug);

    emfdtd sim({100, 100, 100});
    sim.update(10);
}
