# Dark_Photon
The first version of dark shine simulation. 


# Output Data format (ROOT file)
|     Variables    |     Definition    |
|-|-|
|     For all variables with format “xxx_Y”, it means the Y of “xxx”. For example, “TRUTH_MC_E” means the Energy of “Truth MC”.    |  |
|     Px    |     Momentum along x    |
|     Py    |     Momentum along y    |
|     Pz    |     Momentum along z    |
|     E    |     Total Energy    |
|     Eremain    |     The remaining Kinetic Energy leaving detector region    |
|     VPosx    |     Vertex position x    |
|     VPosy    |     Vertex position y    |
|     VPosz    |     Vertex position z    |
|     EPosx    |     End position x    |
|     EPosy    |     End position y    |
|     EPosz    |     End position z    |
|          |          |
|     Rndm    |     random number seed    |
|     t_e1_Momentum    |     the momentum of initial electron while passing target    |
|     t_e1_VPos    |     the position of initial electron while passing target    |
|     t_e2_Momentum    |     the momentum of initial electron while hitting surface of ECAL    |
|     t_e2_VPos    |     the position of initial electron while hitting surface of ECAL    |
|     TRUTH_MC_Nb    |     # of MC particles recorded    |
|     TRUTH_MC_id    |     Track id in Geant 4 for each MC particle    |
|     TRUTH_MC_PDG    |     PDG    |
|     TRUTH_MC_ParentID    |     Parent track id    |
|     Process_Type    |     Type of creating process (refer to Geant4)    |
|     Process_SubType    |     SubType of creating process (refer to Geant4)    |
|     TRUTH_MC_PNEnergy_Tar    |     Max photonuclear reaction energy in target region    |
|     TRUTH_MC_PNEnergy_ECal    |     Max photonuclear reaction energy in ECAL region    |
|     TRUTH_MC_Eleak_ECAL    |     Energy deposition in ECAL holder/gap region    |
|          |          |
|     For all Detector Hit (Tracker and Calorimeter), the output format is the same.    |  |
|     <Detector>_No    |     # of hits in this detector    |
|     <Detector>_Eleak_Wrapper    |     Energy deposition in Cell Wrapper region    |
|     <Detector>_Type    |     Detector Type     |
|     <Detector>_PDG    |     Useless, ignore it.    |
|     <Detector>_Time    |     Hit time (Energy weighted)    |
|     <Detector>_EDep    |     Total energy deposition    |
|     <Detector>_EdepEM    |     EM part of EDep    |
|     <Detector>_EdepHad    |     Had part of EDep    |
|     <Detector>_X    |     Position x of the hit (For tracker, it is accurate; For calorimeter, it’s the center of the cell)    |
|     <Detector>_Y    |     Position y of the hit (For tracker, it is accurate; For calorimeter, it’s the center of the cell)    |
|     <Detector>_Z    |     Position z of the center of the cell    |
|     <Detector>_DetectorID    |     Replication Number of cor. detector    |
|     <Detector>_DetectorID_x    |     DetectorID of x direction     |
|     <Detector>_DetectorID_y    |     DetectorID of y direction     |
|     <Detector>_DetectorID_z    |     DetectorID of z direction     |