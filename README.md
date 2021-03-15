# README #

### Solvers ###
Two solvers, BBPScalarFoam and BBPSdoorSwitch, along with the bus and music-room cases are placed in two seperate directories. BBPScalarFoam adds CEqn that transports concentration C to the standard buoyantBoussinesqPimpleFoam solver and circulation of C (or other quantities) through the HVAC system can be achived by evaluating and specifying BC in the solver, see TEqn.H for details. BBPSdoorSwitch uses a switch variable to control the openning and closing of doors with a body force in the UEqn.

### Geometries ###
Due to their large file size, the geometry files (.stl) for the simulation cases were uploaded to https://drive.google.com/drive/folders/1YS9xyLPyPFCsGSJBkdcN3u6c4xWpKQp1?usp=sharing

### Post-processing ###
For post processing, paraview state files (.pvsm) are given in the case directories. The time-averaged C field (CMean in controlDict) will be used for plotting the accumulative concentration, which can be computed by using the 'fieldAverage' function object.

### Reference ###
The reference paper for the solvers and bus cases: doi:10.1063/5.0037452
