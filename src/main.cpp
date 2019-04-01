#include "Parser.hpp"
#include "PillarsCracker.hpp"
#include "MultiprocessedCracker.hpp"
#include "GenerationCracker.hpp"

#include <fstream>
#include <wait.h>

int main(){
    std::string filename="data_example.txt";
    const Globals global_data = parse_file(filename);
    unsigned int pillar_seed = find_pillar_seed(global_data.pillars_array);
    printf("Pillar seed was found and it is: %d \n", pillar_seed);
    pid_t pidMain=fork();
    if (pidMain==0){
        multiprocess_handler(pillar_seed, global_data.structures_array,global_data.option.processes,pidMain);
    }
    else{
        wait(nullptr);
        std::vector<unsigned long long> partials_seeds=assemble_logs(global_data.option.processes);
        for (auto el:partials_seeds){
            std::cout<<"Potential seed "<<el<<std::endl;
        }
        if (partials_seeds.size()>1){
            std::cout<<"There is too much seed, let's try to reduce that number";
        }
        std::vector<unsigned long long> final_seeds=gen_handler(global_data.biome,partials_seeds,global_data.option.version);
        std::ofstream save("final_seeds.txt", std::ios_base::out | std::ios::trunc);
        if (save.is_open()) {
            for (auto el:final_seeds) {
                std::cout << "Final seeds " << el << std::endl;
                save<<el<<std::endl;
            }
        }
        else{
            std::cout<<"save file was not loaded"<<std::endl;
            throw std::runtime_error("save file was not loaded");
        }
        return 0;
    }

}

