#include <eigen3/Eigen/Dense>

class YeeGrid {

  public:
    YeeGrid(Eigen::Vector3d, unsigned int x_resolution);
    ~YeeGrid();

  private:
    Eigen::Vector3<uint32_t> m_resolution;
    Eigen::Vector3d m_dimensions;
    double m_single_cube_dim;

    Eigen::Vector3d* m_memory;
    size_t m_memory_size;

    Eigen::Vector3d* m_magnetic_field_grid;
    Eigen::Vector3d* m_electric_field_grid;

    bool gridAllocate();
};
