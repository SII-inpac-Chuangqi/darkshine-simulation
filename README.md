# Dark_Photon
The first version of dark shine simulation. :v:

[[_TOC_]]

# Data Recoding
## Output Data format (ROOT file)
|     Variables    |     Definition    |
|-|-|
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
| |
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
| |
|     Detector_No    |     # of hits in this detector    |
|     Detector_Eleak_Wrapper    |     Energy deposition in Cell Wrapper region    |
|     Detector_Type    |     Detector Type     |
|     Detector_PDG    |     Useless, ignore it.    |
|     Detector_Time    |     Hit time (Energy weighted)    |
|     Detector_EDep    |     Total energy deposition    |
|     Detector_EdepEM    |     EM part of EDep    |
|     Detector_EdepHad    |     Had part of EDep    |
|     Detector_X    |     Position x of the hit (For tracker, it is accurate; For calorimeter, it’s the center of the cell)    |
|     Detector_Y    |     Position y of the hit (For tracker, it is accurate; For calorimeter, it’s the center of the cell)    |
|     Detector_Z    |     Position z of the center of the cell    |
|     Detector_DetectorID    |     Replication Number of cor. detector    |
|     Detector_DetectorID_x    |     DetectorID of x direction     |
|     Detector_DetectorID_y    |     DetectorID of y direction     |
|     Detector_DetectorID_z    |     DetectorID of z direction     |
| |
- For all variables with format “xxx_Y”, it means the Y of “xxx”. For example, “TRUTH_MC_E” means the Energy of “Truth MC”.   
- For all Detector Hit (Trackers and Calorimeters), the output format is the same.   
- **Units: MeV, mm, ns**

## Detector modules
There are totally 8 detector modules:
1.	Tagging tracker ( 2 submodules )
2.	Recoiled tracker ( 2 submodules )
3.	ECAL center (LYSO) (ECAL_Center)
4.	ECAL outer top left (ECAL_Outer_1)
5.	ECAL outer top right (ECAL_Outer_2)
6.	ECAL outer bottom left (ECAL_Outer_3)
7.	ECAL outer bottom right (ECAL_Outer_4)
8.	HCAL ( 3*3 = 9 submodules)

## MC truth particle criteria
MC particles satisfied any one of the following conditions will be recorded in TRUTH_MC:
- Initial particle (Track ID == 1)
- Momentum > 1 GeV
- Energy > 1 GeV and Energy < 8 GeV
- Muon
- Pion ($`\pi ^0`$ and $`\pi ^\pm`$)
- Kaon
- Proton
- Neutron
- Neutrino ($`\nu _e`$ and $`\nu _\mu `$)

## Detector Type
For detector type,
0. Tracker
1. ECAL_Center
2. ECAL_Outer

## Detector ID
For detector ID, DetectorID and (DetectorID_x, DetectorID_y, DetectorID_z) are the same. Analyzer can either use DetectorID or (DetectorID_x, DetectorID_y, DetectorID_z). The later one is calculated by splitting the former one according to corresponding cell number along x, y and z. 