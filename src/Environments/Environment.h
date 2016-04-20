//
// Created by Max Horn on 18/04/16.
//

#ifndef PROJECT_NAME_ENVIRONMENT_H
#define PROJECT_NAME_ENVIRONMENT_H

#define GPU_REALTYPE float
#define BORDER_SIZE 1
#define LAPLACIAN_SIZE 1 + 2 * BORDER_SIZE



#include <array>
#include <arrayfire.h>
#include <type_traits>

using namespace af;

struct Ligand {
    std::string name;
    double initialConcentration;
    double globalProductionRate;
    double globalDegradationRate;
    double coliUptakeRate;
    double coliProductionRate;
    double diffusionCoefficient;
};

enum BoundaryConditionType {
    BC_PERIODIC,
    BC_DIRICHELET,
    BC_NEUMANN
};

struct BoundaryCondition {
    BoundaryConditionType type;
    double xneg, xpos, yneg, ypos, zneg, zpos = 0;
};

struct EnvironmentSettings {
    // Definition of Size
    double resolution;
    std::vector<double> dimensions;

    // Definition of ligands
    std::vector<Ligand> ligands;

    //Simulation parameters
    double dt;
    af_dtype dataType;
    BoundaryCondition boundaryCondition;
    // Visualization parameters
    Window *win = NULL;
};

class Environment {
protected:
    // Internal arrays
    af::array densities;
    af::array density_changes;
    af::array diffusion_filters;
    af::array globalProductionRates;

    std::vector<Ligand> ligands;

    // Simultation Parameters
    GPU_REALTYPE dt;
    BoundaryCondition boundaryCondition;
    double resolution;
    std::vector<dim_t> internal_dimensions;

    Window *visualizationWin;
    bool WindowInitialized = false;

    std::function<void(void)> applyBoundaryCondition;
    void initializeWindow();
public:
    Environment(EnvironmentSettings settings);
    std::vector<Ligand> getLigands();
    void printInternals();
    void simulate(double advanceTime);
    void visualize(double normalizer);
    virtual dim4 getSize() = 0;
    virtual void simulateTimeStep() = 0;
    virtual array getDensity(unsigned int ligand) = 0;
    virtual array getAllDensities() = 0;
    //void test();
};

#endif //PROJECT_NAME_ENVIRONMENT_H