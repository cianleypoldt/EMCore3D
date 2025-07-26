#include "IO/file_output.h"
#include "emfdtd/emfdtd.h"

int main() {

    io::init_spdlog(spdlog::level::debug);

    emfdtd sim({100, 100, 100});
    io::file_output file_out;
    file_out.open(&sim.grid, "testsave.bin");
    sim.update(1, [&](int i) {
        if (i % 5 == 4) {
            file_out.write();
        }
    });
}
