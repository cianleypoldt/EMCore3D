#include "app/simulation.hpp"
#include "common/spdlog_init.hpp"
#include "io/file_system.hpp"

int main() {
    io::init_spdlog(spdlog::level::debug);

    Simulation sim({ 100, 100, 100 });

    auto o_file = fs::binary::initiate_writer(&sim.grid, "gird.bin", 1);
    fs::binary::append_current_field(o_file);

    sim.update(1, [&](int i) {
        if (i % 5 == 4) {
        }
    });
}
