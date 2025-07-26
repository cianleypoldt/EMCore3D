#include "emfdtd/emfdtd.h"
#include "IO/file_output.h"
#include "IO/io.h"

int main() {
    io::init_spdlog(spdlog::level::debug);

    Emfdtd         sim({ 100, 100, 100 });
    io::FileOutput file_out;
    file_out.open(&sim.grid, "testsave.bin");
    sim.update(1, [&](int i) {
        if (i % 5 == 4) {
            file_out.write();
        }
    });
}
