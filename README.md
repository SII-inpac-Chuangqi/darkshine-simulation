# Dark SHINE Software Package

The second version of dark shine simulation. :v:

[[_TOC_]]

# Get Started

There are four parts of the Dark SHINE Software: **DSimu**, **DAna**, **DDis**, **DPlot**.

- **DSimu** is the simulation program based on Geant4 and ROOT, characterized by Dark SHINE detector.
- **DAna** is a framework for the analysis and reconstruction tools. It requires the output ROOT file from **DSimu**.
- **DDis** is the event display tool for Dark SHINE specifically.
- **DPlot** is a basic plotting program for quick plot, which is based on ROOT.

They could be executed separately, with totally different configuration file format.

## Installation

Dark SHINE Software can be easily downloaded through GitLab.

```c++
    git clone git@gitlab.com:yulei_zhang/darkshine-simulation.git
```

Before installing, several dependencies need to be checked.

- C++17
- Geant4 10.06
- ROOT 6
- HepMC
- gsl
- yaml-cpp

With everything needed, it's ready to install :v:

```shell script
cd darkshine-simulation   # <source-directory>
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=<some-directory> ../
make -j100  # Just do it!
make install
```

Now in your install directory, everything should be there. Now it's the time to have fun with them. :relaxed:

## Quick guide on DSS

<mark>All the example scripts can be found under the "scripts" folder in their corresponding source directories.<mark>

### DSimu


Without any arguments, DSimu will run in graphic mode, which is highly not recommended unless it is running on a
computer with great graphic card.

**Batch Mode**: ```DSimu [ -y file.yaml] [ -m config.file ]```

- (**IMPORTANT**)The config file after '-y' is the yaml file, which will control the whole configuration of the program.
  The default yaml file is in ```DP_simu/scripts/```, or it will also be installed to the ```CMAKE_ISNTALL_PREFIX```
  path.
- The config file after '-m' is the normal configuration, e.g. particle gun, beam on number.

### DAna

There are two commands for **DAna**:

- ```DAna -c config.txt```
    - **DAna** will run the config.txt.
- ```DAna -x```
    - **DAna** will print out all the available processors and their corresponding description and parameters (with the
      default value). One can simply generate an example config file by the bash command
      ```DAna -x > config.txt```

### DDis

Event Display is quite straight forward.

- ```DDis -h```
    - print out the help message
- ```DDis -b```
    - batch mode, which only print out the detector information
- ```DDis [-g geometry.root] [-f event.root]```
    - the command is not necessary to run the graphic mode. if no specific root file, **DDis**
      will automatically run "dp_out.root".

### DPlot

This is a really naive plotting program for newbies and lazy boys.

- ```DPlot -c config.yaml```
    - As like other DSS tools, **DPlot** receives yaml file as its configuration. The execution of **DPlot** will
      generate all the plots under
      ```./<name>/```, where ```name``` is the first block in yaml file, which represents the identifier of the whole
      job.

## DSS Wiki of yaml config

**Quick Look on yaml tutorial**: https://gettaurus.org/docs/YAMLTutorial/

<details><summary><b>DSimu</b></summary>

| Name | Description | Example |
| ---| ---| --- |
|**Global Variables** || |
| save_geometry | save geometry as ```TGeoManager``` in root file | false |
| check_overlaps | check the overlaps in Geant4, very time-consuming | false |
| signal_production | quick option for generate Dark Physics process. If signal production is activated, program will automatically bias for DMProcessDMBrem and record all the MC particles A process filter on target region will be applied | false |
|**RootManager** | | |
|outfile_Name| the output root file name| "dp_out.root" |
|tree_Name| the tree name in the output file| "Dark_Photon" |
|Run_Number| the start number for event ID| 0 |
|Total_Event_Number| total event number in this run| 10000|
|**OutCollection** | | |
|save_all_mcp| save all the MC particles in simulation (**be careful**, for 8 GeV EM shower, # of MC particles can up to 15K) | false |
|save_MC| save the selected MC particles | true |
|save_initial_particle_step| save the initial particle steps | true |
|RawMCCollection_Name| The name of MC particle collection in output | "RawMCParticle" |
|InitialParticleStepCollection_Name| The name of initial particle step collection in output | "Initial_Particle_Step" |
|**Biasing** | | |
|if_bias | Control of biasing | false |
|if_bias_target| bias process on target | false |
|if_bias_ECAL| bias process in ECAL | false |
|BiasProcess| the process name to be biased | "GammaToMuPair" |
|BiasFactor| the biased factor, simply defined as the scale factor on cross section| 1e20 |
|BiasEmin|  [Energy, "Unit"]:  the minimal energy for the process | [4, "GeV"] |
|**Filters**|||
|if_filter| Control of event filters | false |
|particle_filters_parameters| a list controlling all particle filters, add filters by appending: - [ 22, 6, "GeV", 8, "GeV", -1, "m", 0.2, "m", true ] | |
|process_filters_parameters| a list controlling all process filters,  add filters by appending: - [ "msc", 6, "GeV", 8, "GeV", -1, "m", 0.2, "m", true ] ||

</details>

<details><summary><b>DSimu: Filters</b></summary>

**new commands**

``` 
/DP/Filter/particle <PDG> <rising energy edge> <falling energy edge> <rising scan distance edge> <falling scan distance edge> <flag>
/DP/Filter/process <process name> <rising energy edge> <falling energy edge> <rising scan distance edge> <falling scan distance edge> <flag>
```

Each command will construct a new  ```FilterParticle``` or ```FilterProcess``` class, their pointers are emplace_back to
a vector.

**flag:**

flag = 1: The Event to be computed must have this secondary particle/process in particular energy range and scan
distance range. Otherwise this event will be aborted.

flag = 0: The Event to be computed must not have this secondary particle/process in particular energy range and scan
distance range. If this particle/process was scanned, then this event will be aborted.

**usage of "edge":** Use "edge of the square wave" to specify energy range and scan distance range. So we can use 2
parameters to construct 5 types of filter.

| filter type | Min Energy [MeV] | Max Energy [MeV] |
| ---| ---| --- |
| low-pass filter | 0 | 4000 |
| high-pass filter | 4000 | 0 |
| band-pass filter | 4000 | 8000 |
| band-stop filter | 8000 | 4000 |
| all-pass filter | 0 | 0 |

| filter type | min scan distance [mm] | max scan distance [mm] |
| ---| ---| --- |
| band-pass filter | -1000 | 200 |
| band-stop filter | 200 | -1000 |
| all-pass filter | 0 | 0 |

related method: ```In_Range()```

</details>


<details><summary><b>DPlot</b></summary>

| Name | Description | Example |
| ---| ---| --- |
| name | the name of this whole job | "test" |
| variables | all the variables to be plotted | see **variables** block |
| samples | plot variables from the selected samples | see **samples** block |
| *global_y_scale** | global Y axis scale factor | 2.5 |

|  | variables |  |
| ---| ---| --- |
| name | variable to be plotted, same as ```tree->draw()``` | "Energy/100" |
| range | [ bin_x, min_x, max_x ], same as ```TH1``` | [50, 1, 100] |
| label | variable label to be set as the title of X axis ( TLatex supported ) | "E_{e} [GeV]" |
| *selection_title** | The third line on the figure texts  | "No Selection" |
| *log_x** | if to apply log-scale on X axis | true |
| *log_y** | if to apply log-scale on Y axis | true |
| *grid_y** | if to draw grid along x axis | false |
| *normalize** | if to normalize the samples | false |
| *y_scale** | the scale factor to scale Y axis | 2.0 |

- *items* with star (*) are optional.
- The default value of optional items is *false or NULL*.

|  | samples |  |
| ---| ---| --- |
| name | the name of this sample | "signal" |
| file_name | the input root file | "dp_ana.root" |
| tree_name | the tree name | "dp" |
| *selection** | selection on this sample, same as ```TCut``` | "E >= 50." |
| *line_attr** | the line attributes: [ color, width ] | [ 2, 3 ] |
| *marker_attr** | the marker attributes: [ color, transparency, width, style ] | [ 3, 0.95, 2, 1] |
| *fill_attr** | the fill attributes: [ color, transparency, style ] | [ 4, 0.65, 3001 ] |

- *items* with star (*) are optional.
- The default value of optional items is *false or NULL*.

</details>

# Data Recoding

- The geometry will be saved in the output root file if set "save_geometry = true"
- All the information about event will be stored as DEvent class.
- For all Detector Hit (Trackers and Calorimeters), the output format is the same.
- **Units: MeV, mm, ns**

## Detector modules

There are totally 4 detector modules:

1. Tagging tracker ( 2 submodules )
2. Recoiled tracker ( 2 submodules )
3. ECAL center (ECAL_Center)
4. HCAL ( 3*3 = 9 submodules)

## MC truth particle selection criteria

MC particles satisfied any one of the following conditions will be recorded in truth MC collection:

- Initial particle (Track ID == 1)
- Momentum > 1 GeV
- Energy > 1 GeV and Energy < 8 GeV
- Muon
- Pion
- Kaon
- Proton
- Neutron
- Neutrino

## Detector ID

For detector ID, DetectorID and (DetectorID_x, DetectorID_y, DetectorID_z) are the same. Analyzer can either use
DetectorID or (DetectorID_x, DetectorID_y, DetectorID_z). The later one is calculated by splitting the former one
according to corresponding cell number along x, y and z. 

# Change Log

## Unreleased

### Added:

### Fixed:

## 2021-08-11

### Added:

- Integrated GenFit2 Library.