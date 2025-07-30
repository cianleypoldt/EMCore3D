#include "app/simulation.hpp"
#include "common/spdlog_init.hpp"
#include "io/file_system.hpp"

int main() {
    io::init_spdlog(spdlog::level::debug);

    Simulation sim({ 100, 100, 100 });

    auto o_file = io::fs::start_raw_writer(&sim.grid, "gird.bin", 1);

    sim.update(1, [&](int i) {
        if (i % 5 == 4) {
        }
    });
}
