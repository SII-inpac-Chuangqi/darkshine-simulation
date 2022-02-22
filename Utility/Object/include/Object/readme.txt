versions:
b1 : baseline1
b1_4 : baseline1.5
    - DHit - remove Cell XYZ
    - DStep - remain only tracker region
    - TODO: Create Cell XYZ Map
    - TODO: SimulatedHits - move double to short
    - TODO: ECAL Hit - only save calo ID and energy


PS: steps to create new version:
    1. edit CMakeLists.txt, refresh cache
    2. create Objetc/versions/*_b*.h
    3. check #ifndef
    4. check Object/*.h ( commit at here to backup )
    5. refactor