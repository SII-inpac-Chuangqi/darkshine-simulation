# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

DarkSHINE Software Package (DSS) is a simulation and analysis framework for the DarkSHINE experiment, built on Geant4 and ROOT. The software consists of four main components:

- **DSimu**: Geant4-based simulation with DarkSHINE detector geometry
- **DAna**: Analysis and reconstruction framework (processor-based architecture)
- **DDis**: Event display tool
- **DPlot**: Basic plotting utility

## Build System

### Dependencies

- C++17 compiler
- Geant4 10.06
- ROOT 6 (>=6.20)
- GSL
- yaml-cpp
- nlohmann/json
- XercesC
- Eigen3 (for DAna)
- ACTS (optional, modified version: https://github.com/ykrsama/acts/tree/xuliang-v30)
- HDF5 (optional)
- ONNX Runtime (optional)

### Build Commands

```bash
# Standard build
mkdir build install
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install -DBUILD_ACTS=ON ..
make -j
make install
cd ..

# Setup environment
export DSS_DIR=<absolute-path-to-install-dir>
export PATH=${DSS_DIR}/bin:${PATH}
export LD_LIBRARY_PATH=${DSS_DIR}/lib:${LD_LIBRARY_PATH}
```

### Build Options

- `WITH_GEANT4_UIVIS=ON/OFF`: Build with Geant4 UI and visualization (default: ON)
- `BUILD_ACTS=ON/OFF`: Build with ACTS tracking (default: OFF)
- `BUILD_HDF5=ON/OFF`: Build with HDF5 support (default: ON)
- `BUILD_DANA=ON/OFF`: Build analysis framework (default: ON)
- `BUILD_DSIMU=ON/OFF`: Build simulation (default: ON)
- `BUILD_DDIS=ON/OFF`: Build event display (default: ON)
- `BUILD_TOOLS=ON/OFF`: Build tools (default: ON)
- `BUILD_ONNX=ON/OFF`: Build with ONNX support (default: OFF)

### Automated Build and Test

```bash
# Build everything
./Automaton/build.sh

# Run tests
./Automaton/test.sh
```

## Running the Software

### DSimu (Simulation)

```bash
# Basic run with YAML configuration
DSimu -y default.yaml

# Batch mode with custom beam-on number
DSimu -y default.yaml -b 10000

# Configuration mode (interactive Geant4 commands)
DSimu -y default.yaml -m

# Save geometry
DSimu -y default.yaml -b 1 --save_geometry true -f Geometry.root

# Set random seed
DSimu -y default.yaml --seed 12345 --Run_Number 0
```

Configuration file: `DP_simu/scripts/default.yaml`

Magnetic field file required: `mag_default.root` (example: `magnet_1.5.root`)

### DAna (Analysis/Reconstruction)

```bash
# Run with configuration file
DAna -c config.txt

# Print available processors
DAna -x
```

Configuration file: `DP_ana/scripts/config.txt`

### DDis (Event Display)

```bash
# Graphic mode with default files
DDis

# With specific files
DDis -g geometry.root -f event.root

# Batch mode (print detector info only)
DDis -b

# Help
DDis -h
```

### DPlot (Plotting)

```bash
DPlot -c plot.yaml
```

### Parallel Batch Processing

```bash
# Generate geometry
parDSS.sh Geom

# Run simulation in parallel (10 jobs)
parDSS.sh Simu 10

# Run analysis in parallel (10 jobs)
parDSS.sh Ana 10

# Merge results
parDSS.sh Merge 10

# Run everything
parDSS.sh all 10 <batch_id>
```

## Architecture

### Core Data Objects (Utility/Object)

All data structures inherit from or use ROOT classes for I/O:

- **DEvent**: Central event container holding all event data
- **DParticle**: MC truth particle information
- **DHit**: Generic hit information for detectors
- **CalorimeterHit**: Specialized calorimeter hit
- **SimulatedHit**: Simulation-level hit information
- **ReconstructedParticle**: Reconstructed particle after analysis
- **DTruth**: MC truth association data
- **DMagnet**: Magnetic field representation
- **DDetectorIDMaps**: Detector ID mapping utilities

Units: **MeV, mm, ns**

### Core Services (Utility/Service)

- **EventStoreAndWriter**: ROOT I/O management
- **AlgoManager**: Manages processor execution pipeline
- **AnaProcessor**: Base class for all analysis processors

### DAna Processor Architecture

DAna uses a modular processor-based design. Each processor inherits from `AnaProcessor` and implements:

- `Begin()`: Initialization before event loop
- `InitEvt()`: Per-event initialization
- `ProcessEvt(AnaEvent*)`: Main event processing
- `CheckEvt(AnaEvent*)`: Event validation
- `End()`: Finalization after event loop

Processors are configured via `config.txt` with format:
```
Algorithm.List = Processor1 Processor2 Processor3
Processor1.Parameter = value
```

### Available Processors (Algorithms/)

- **Digitizer**: Digitization for calorimeters with optical processes
- **MCTruthAnalysis**: MC truth analysis and particle selection
- **Tracking**: Track reconstruction (Kalman/Riemann fitting)
- **RecECAL**: ECAL reconstruction and clustering
- **RecHCAL**: HCAL reconstruction
- **CutFlowAnalysis**: Cut flow analysis
- **GNN_DataExporter**: Export data for GNN training
- **ECAL_ML_IO**: ECAL ML data I/O (HDF5)
- **ActsSequencer**: ACTS-based tracking (requires BUILD_ACTS=ON)

### DSimu Components (DP_simu/)

- **Geometry/**: Detector geometry construction
- **Control/**: Run management and action initialization
- **DarkPhysics/**: Dark matter physics processes
- **Bias_Filter/**: Biasing and filtering for rare processes
- **Optical/**: Optical physics processes
- **Animation/**: Event data recording

### Detector Modules

1. **Tagging Tracker**: 2 submodules upstream
2. **Recoil Tracker**: 2 submodules downstream
3. **ECAL**: Electromagnetic calorimeter (Center + Outer)
4. **HCAL**: Hadronic calorimeter (3×3 = 9 submodules)

### MC Truth Selection Criteria

Particles recorded in MC truth collection if they satisfy any:
- Initial particle (Track ID == 1)
- Momentum > 1 GeV
- Energy > 1 GeV and < 8 GeV
- Muon, Pion, Kaon, Proton, Neutron, or Neutrino

## Development Workflows

### Adding a New Analysis Processor

1. Create directory under `Algorithms/<ProcessorName>/`
2. Add header in `include/Algo/<ProcessorName>.h` inheriting from `AnaProcessor`
3. Implement source in `src/<ProcessorName>.cpp`
4. Register parameters in constructor using `RegisterXParameter()` methods
5. Add to `Algorithms/CMakeLists.txt`
6. Use in DAna by adding to `Algorithm.List` in config file

### Running Single Test

Example from `DP_simu/scripts/test.sh`:
```bash
DSimu -y default.yaml -b 5000
```

Example from `DP_ana/scripts/test.sh`:
```bash
DAna -c config.txt
```

### Signal Production Mode

For Dark Physics signal generation, set in YAML:
```yaml
signal_production: true
```

This automatically:
- Applies biasing for DMProcessDMBrem
- Records all MC particles
- Applies process filter on target region

## Configuration Files

### DSimu YAML Structure

Key configuration blocks:
- **Global Variables**: `save_geometry`, `check_overlaps`, `signal_production`
- **RootManager**: Output file settings, event numbers
- **OutCollection**: What to save (MC particles, hits, etc.)
- **Biasing**: Cross-section biasing for rare processes
- **Filters**: Particle and process filters for event selection

### DAna Config Structure

```
# I/O
InputFile = dp_simu.root
InputGeoFile = dp_simu.root
OutputFile = dp_ana.root

# Verbosity
AlgoManager.Verbose = 0

# Algorithm sequence
Algorithm.List = Digitizer MCTruthAnalysis Tracking RecECAL RecHCAL

# Processor parameters
Tracking.if_strip = 1
Tracking.if_smear = 1
RecECAL.StaggeredECAL = 1
```

## CI/CD

GitLab CI pipeline (`.gitlab-ci.yml`) runs automatically on:
- Merge requests to master
- Commits with `[CI]` or `[VIP]` in message
- Tags

Stages:
1. **build**: Compile and install
2. **test**: Run simulation and analysis tests
3. **post**: Update wiki (on master or with `[WIKI]`)

Skip CI with `[NOCI]` in commit message.

## Tools

Located in `Tools/`:
- **DConvertor**: Convert between data formats
- **DCopy**: Copy/filter events
- **DCutflow**: Cut flow analysis
- **DDump**: Dump event information
- **DPlot**: Quick plotting

## Notes

- Example scripts are in `<component>/scripts/` directories
- All scripts are also installed to `CMAKE_INSTALL_PREFIX`
- For INPAC cluster, use provided setup scripts
- Docker image available: `ykrsama/darkshine-simulation`
- When reading existing code, pay attention to the processor registration pattern and parameter handling
- The codebase uses ROOT's reflection system extensively for I/O