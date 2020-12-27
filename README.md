# Dark SHINE Software: DSimu && DAna
The second version of dark shine simulation. :v:

[[_TOC_]]

# Get Started
There are two parts of the Dark SHINE Software: **DSimu** and **DAna**.

- **DSimu** is the simulation program based on Geant4 and ROOT, characterized by Dark SHINE detector.
- **DAna** is a framework for the analysis and reconstruction tools. It requires the output ROOT file from **DSimu**.

They could be executed separately, with totally different configuration file format.
## Installation
Dark SHINE Software can be easily downloaded through GitLab.
```c++
    git clone git@gitlab.com:yulei_zhang/darkshine-simulation.git
```
Before installing, several dependencies need to be checked.
- Geant4 10.06
- ROOT 6
- HepMC

With everything needed, it's ready to install :v:
```shell script
cd darkshine-simulation   # <source-directory>
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=<some-directory> ../
make -j100  # Just do it!
make install
```

Now in your install directory, there should be two binary files: **DSimu** and **DAna**.
Now it's the time to have fun with them. :relaxed:

## Running DSimu with macro
Without any arguments, DSimu will run in graphic mode, which is highly not recommended unless it is running on a computer with great graphic card.

**Batch Mode**: _DSimu [ -m config.file ] [ -o optical.file ]_

- The config file after '-m' is the normal configuration, e.g. biasing paramter, gun energy, etc. The details of how to write config file will be included in later chapter.
- The config file after '-o' is the optical simulation configuration, which will control whether to switch on the simulation of optical photon.

## Writing DSimu config file

### Changing Geometry
| Command                            | Detail                                                             | Example          |
|------------------------------------|--------------------------------------------------------------------|------------------|
|**Geometry**|
| /DP/det/reConstruct                | **Must be called after all geometry parameter settings is done.**  |                  |
| /DP/det/trkTarDis                  | Distance between the tracker and target.                           | 7.5 mm           |
| /DP/det/onlyTracker                | Only build the tagging tracker and the recoild tracker.            |                  |
| /DP/det/onlyECAL                   | Only build the ECAL.                                               |                  |
| /DP/det/onlyHCAL                   | Only build the HCAL.                                               |                  |

<details><summary><b>Tagging Tracker</b></summary>

| Command                            | Detail                                                             | Example          |
|------------------------------------|--------------------------------------------------------------------|------------------|
|**Tagging Tracker**|
| /DP/det/ifTagTracker               | Whether to build the tagging tracker.                              | 1(true)/0(false) |
| /DP/det/setTByField                | Define tagging tracker y-direction magnetic field.                 | -1.5 tesla       |
| /DP/det/tagTrk/addNewSize          | Add a new pair of tagging tracker layers, and specify it's size. Repeat this command to add multiple tagging tracker layers. | 10 20 0.01 cm |
| /DP/det/tagTrk/addNewPos           | Specify the position of the new tagging tracker layer. <br> The maximum Z value and minimum Z value should be centrosymmetric, and the number of this conmmand should be consistent with the addNewSize command. | 0 0 -10 cm <br> 0 0 5 cm <br> 0 0 10 cm |
| /DP/det/tagTrk/del                 | Delete all previous tagging tracker layers.                        |                  |
| /DP/det/tagTrk/rot1                | Angle of front tagging tracker layers.                             | 0 radian         |
| /DP/det/tagTrk/rot2                | Angle of back tagging trackers layers.                             | 0.1 radian       |
| /DP/det/tagTrk/color1              | Display RGB color of TagTrk1 in GUI.                               | 0.5 0.5 0        |
| /DP/det/tagTrk/color2              | Display RGB color of TagTrk2 in GUI.                               | 0.5 0.5 0        |
</details>

<details><summary><b>Recoil Tracker</b></summary>

| Command                            | Detail                                                             | Example          |
|------------------------------------|--------------------------------------------------------------------|------------------|
|**Recoil Tracker**|
| /DP/det/ifRecTracker               | Whether to build the recoil tracker.                               | 1(true)/0(false) |
| /DP/det/setRByField                | Define recoil tracker y-direction magnetic field.                  | -0.5 tesla       |
| /DP/det/recTrk/addNewSize          | Add a new pair of recoil tracker layers, and specify it's size. Repeat this command to add multiple recoil tracker layers. | 10 20 0.01 cm |
| /DP/det/recTrk/addNewPos           | Specify the position of the new recoil tracker layer. <br> The maximum Z value and minimum Z value should be centrosymmetric, and the number of this conmmand should be consistent with the addNewSize command. | 0 0 -10 cm <br> 0 0 5 cm <br> 0 0 10 cm |
| /DP/det/recTrk/del                 | Delete all previous recoil tracker layers.                         |                  |
| /DP/det/recTrk/rot1                | Angle of front recoil tracker layers.                              | 0 radian         |
| /DP/det/recTrk/rot2                | Angle of back recoil trackers layers.                              | 0.1 radian       |
| /DP/det/recTrk/color1              | Display RGB color of RecTrk1 in GUI.                               | 0.5 0.5 0        |
| /DP/det/recTrk/color2              | Display RGB color of RecTrk2 in GUI.                               | 0.5 0.5 0        |
</details>

<details><summary><b>Target</b></summary>

| Command                            | Detail                                                             | Example          |
|------------------------------------|--------------------------------------------------------------------|------------------|
|**Target**|
| /DP/det/ifTarget                   | Whether to build the target.                                       | 1(true)/0(false) |
</details>

<details><summary><b>ECAL</b></summary>

| Command                            | Detail                                                             | Example          |
|------------------------------------|--------------------------------------------------------------------|------------------|
|**ECAL**|
| /DP/det/ifECAL                     | Whether to build the ECAL.                                         | 1(true)/0(false) |
| /DP/det/selectECAL                 | Select the build-in ECAL configuration.                | 1 : XYCrossing <br> 2 : AllZ |
| /DP/det/ECAL/centerWrapSize        | ECAL center wrap size.                                             | 0.3 0.3 0.3 mm   |
| /DP/det/ECAL/centerSize            | ECAL center size.                                                  | 1 1 37.05 cm     |
| /DP/det/ECAL/centerModuleNo        | ECAL center module number.                                         | 6 6 1            |
</details>

<details><summary><b>HCAL</b></summary>

| Command                            | Detail                                                             | Example          |
|------------------------------------|--------------------------------------------------------------------|------------------|
|**HCAL**|
| /DP/det/ifHCAL                     | Whether to build the HCAL.                                         | 1(true)/0(false) |
| /DP/det/HCAL/wrapSize              | HCAL wrap size.                                                    | 0.3 0.3 0.3 mm   |
| /DP/det/HCAL/sizeDir               |  | 100.57 5 1 cm |
| /DP/det/HCAL/moduleNoDir           |  | 1 20 120 |
| /DP/det/HCAL/moduleNo              | HCAL module number.                                                | 3 3 1            |
| /DP/det/HCAL/moduleGap             | Gap between HCAL modules.                                          | 0.5 mm           |
| /DP/det/HCAL/absorberThickness     | Thickness of the absorber.                                         | 3 cm             |
</details>


### Rare Process Biasing
<details><summary><b>Biasing</b></summary>

| Command                            | Detail                                                             | Example          |
|------------------------------------|--------------------------------------------------------------------|------------------|
|**Biasing**|
| /DP/setifBias                      | If bias the corresponding physics process                          | 1(true)/0(false) |
| /DP/Bias/Process                   | Name of the biased physics process in Geant4                       | GammaToMuPair    |
| /DP/Bias/Factor                    | Enlarge the cross section of the process by a factor               | 1e6              |
| /DP/Bias/Emin                      | The minimal energy required for the particle to be biased          | 1 GeV            |
| /DP/Bias/if_bias_target            | Biased region                                                      | 1(true)/0(false) |
| /DP/Bias/if_bias_ecal              | Biased region                                                      | 1(true)/0(false) |
</details>

<details><summary><b>Filters</b></summary>

**new commands**

``` 
/DP/Filter/particle <PDG> <rising energy edge> <falling energy edge> <rising scan distance edge> <falling scan distance edge> <flag>
/DP/Filter/process <process name> <rising energy edge> <falling energy edge> <rising scan distance edge> <falling scan distance edge> <flag>
```

**example**

```
/DP/Filter/particle 22 0 4 -1000 200 1
/DP/Filter/process GammaToMuPair 0 1 -5 600 0
```

Each command will construct a new  ```FilterParticle``` or ```FilterProcess``` class, their pointers are emplace_back to a vector.

**flag:**

flag = 1: The Event to be computed must have this secondary particle/process in particular energy range and scan distance range. Otherwise this event will be aborted.

flag = 0: The Event to be computed must not have this secondary particle/process in particular energy range and scan distance range. If this particle/process was scanned, then this event will be aborted.

**usage of "edge":** Use "edge of the square wave" to specify energy range and scan distance range. So we can use 2 parameters to construct 5 types of filter.

| filter type | rising energy edge (MeV) | falling energy edge (MeV) |
| ---| ---| --- |
| low-pass filter | 0 | 4000 |
| high-pass filter | 4000 | 0 |
| band-pass filter | 4000 | 8000 |
| band-stop filter | 8000 | 4000 |
| all-pass filter | 0 | 0 |

| filter type | rising scan distance edge (MeV) | falling scan distance edge (MeV) |
| ---| ---| --- |
| band-pass filter | -1000 | 200 |
| band-stop filter | 200 | -1000 |
| all-pass filter | 0 | 0 |

related method: ```Square_Filter()```

</details>

# Data Recoding
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

## MC truth particle selection criteria
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


