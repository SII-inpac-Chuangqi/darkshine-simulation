#include <vector>
#include <algorithm>
#include <iostream>

// McParticle definition for demonstration purposes.
// Replace this with your actual class definition.
struct McParticle {
    int id;
    float energy;

    McParticle(int id, float energy) : id(id), energy(energy) {}
    int getId() const { return id; }
    float getEnergy() const { return energy; }
};

int main() {
    std::vector<McParticle> mcps = {
            McParticle(1, 300.0f),
            McParticle(2, 200.0f),
            McParticle(1, 500.0f),
            McParticle(2, 100.0f),
            McParticle(1, 400.0f),
            McParticle(1, 500.0f),
            McParticle(2, 100.0f),
            McParticle(1, 400.0f),
            McParticle(1, 100.0f),
            McParticle(2, 150.0f),
            McParticle(1, 90.0f),
            McParticle(2, 140.0f),
            McParticle(3, 80.0f)
    };

    std::sort(mcps.begin(), mcps.end(), [](const McParticle &a, const McParticle &b) {
        if (a.id == b.id) {
            return a.energy < b.energy;
        } else {
            return a.id < b.id;
        }
    });

    for (const auto &particle : mcps) {
        std::cout << "Id: " << particle.id << ", Energy: " << particle.energy << '\n';
    }

//    // Manually check the output and see if the sorting is correct.
//    std::cout<<"Duplicate Check"<<std::endl;
//    std::vector<McParticle> duplicates;
//    auto it = mcps.begin();
//    while (it != mcps.end()) {
//        it = std::adjacent_find(it, mcps.end(), [](const McParticle &a, const McParticle &b) {
//            return a.id == b.id;
//        });
//        if (it != mcps.end()) {
//            duplicates.push_back(*it);
//            // Skip over remaining elements with the same id
//            int current_id = it->id;
//            while (it != mcps.end() && it->id == current_id) {
//                ++it;
//            }
//        }
//    }
//
//    for (const auto &particle : duplicates) {
//        std::cout << "Id: " << particle.id << ", Energy: " << particle.energy << '\n';
//    }

    std::cout<<"Duplicate Check 2"<<std::endl;
    float min_energy = 95.0f;
    auto last = std::unique(mcps.begin(), mcps.end(), [](const McParticle &a, const McParticle &b) {
        return a.getId() == b.getId();
    });
    mcps.erase(last, mcps.end());
//    mcps.erase(
//            std::remove_if(
//                    mcps.begin(), mcps.end(),
//                    [min_energy](const McParticle &mcp) { return mcp.getEnergy() < min_energy; }),
//            mcps.end()
//    );

    for (const auto &particle : mcps) {
        std::cout << "Id: " << particle.id << ", Energy: " << particle.energy << '\n';
    }


    return 0;
}
