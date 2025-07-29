#include "emfdtd/emfdtd.h"
#include "IO/file_system.h"
#include "IO/io.h"

int main() {
    io::init_spdlog(spdlog::level::debug);

    Emfdtd sim({ 100, 100, 100 });

    sim.update(1, [&](int i) {
        if (i % 5 == 4) {
        }
    });
}
