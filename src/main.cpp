#include <arrayfire.h>
#include "easylogging++.h"

#include "Environments/Environment2D.h"
#include <random>
#include "BacterialPopulations/BacterialPopulation.h"


INITIALIZE_EASYLOGGINGPP

int main(int argc, char** argv)
{
	char t_device_name[64] = {0};
	char t_device_platform[64] = {0};
	char t_device_toolkit[64] = {0};
	char t_device_compute[64] = {0};
	af::deviceInfo(t_device_name, t_device_platform, t_device_toolkit, t_device_compute);

    printf("Device name: %s\n", t_device_name);
    printf("Platform name: %s\n", t_device_platform);
    printf("Toolkit: %s\n", t_device_toolkit);
    printf("Compute version: %s\n", t_device_compute);

    EnvironmentSettings ESettings;

    ESettings.resolution = 0.5;
    ESettings.dimensions = std::vector<double> {10, 10};
    ESettings.dt = 0.005;
    ESettings.dataType = f32;
    ESettings.convolutionType = CT_SERIAL;

    BoundaryCondition boundaryCondition(BC_NEUMANN);
    boundaryCondition.xpos = 0;
    boundaryCondition.ypos = 0;
    ESettings.boundaryCondition = boundaryCondition;

    // Ligands
    Ligand ligand1 = {"Ligand1", 0, 1.0, 0.0, 0.0, 5.0};
    //Ligand ligand2 = {"Ligand2", 1, 10.0, 0.0, 0.0, 10.0};
    //Ligand ligand3 = {"Ligand3", 2, 20.0, 0.0, 0.0, 15.0};
    //Ligand ligand4 = {"Ligand4", 3, 20.0, 0.0, 0.05,20.0};
    ESettings.ligands = std::vector<Ligand> {ligand1};//, ligand2};

#ifndef NO_GRAPHICS
    Window mywindow(1024, 512,"Simple Diffusion simulation");
    mywindow.setColorMap(AF_COLORMAP_HEAT);
    ESettings.win = &mywindow;
#endif
    Environment2D simEnv(ESettings);

     LigandInteraction interaction1 = {0, 5, 0, 0};
//    LigandInteraction interaction2 = {0, 10, 0, 0};
    std::vector<LigandInteraction> ligandInteractions = {interaction1};//, interaction2};

    BacterialParameters bactParams = {ligandInteractions, ESettings.dt, 0.01};

    GPU_REALTYPE xpos[] = {5.0, 9.0, 10.0};
    GPU_REALTYPE ypos[] = {5.0, 9.0, 10.0};
    Bacterial2DPopulation bacteria(&simEnv, bactParams, 3, xpos, ypos);


    time_t start= time(NULL);
//    simEnv.test();
    std::cout << time(NULL)-start;
    for(int i =0; i < 20000; i++) {
        double normalizer = max<double>(simEnv.getAllDensities());
        bacteria.simulateTimestep();
        simEnv.evalDensities();
        simEnv.simulateTimeStep();
        simEnv.visualize(normalizer);

        if(i % 20 == 0){
            std::cout << pow((double)(time(NULL)-start)/20.0, -1) << " iterations per second" << std::endl;
            start = time(NULL);
        }

    }

    // simEnv.test();
    // simEnv.printInternals();

    // array test = (randomDistribution(1000000) >= 0.5).as(b8);

    // printf("%f\n", sum<float>(test)/1000000);

    // simEnv.test();

    return 0;
}
