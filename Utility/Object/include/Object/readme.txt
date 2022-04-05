versions:
b1 : baseline1
b1_4 : baseline1.5
    - DHit - remove Cell XYZ
    - DStep - remain only tracker region
    - TODO: Create Cell XYZ Map
    - TODO: SimulatedHits - move double to short
    - TODO: ECAL Hit - only save calo ID and energy


PS: steps to create new version:
    1. create Objetc/versions/*_b*.h
    2. check #ifndef
    3. check Object/*.h
    4. edit CMakeLists.txt, refresh cache
    5. refactor ( commit here to backup )