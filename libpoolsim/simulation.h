#pragma once

#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

namespace poolsim {

class InvalidSimulationException : public std::exception {
public:
  explicit InvalidSimulationException(const char* _message);
  const char* what() const throw();
private:
  const char* message;
};

struct MinerConfig {
  std::string generator;
  nlohmann::json params;
};

struct RewardSchemeConfig {
  std::string scheme_type;
  nlohmann::json params;
};

struct PoolConfig {
    // The name of the pool
    std::string name;

    // The difficulty of the pool
    uint64_t difficulty;

    // Probability of having an uncle block
    double uncle_block_prob;

    // Reward scheme to use for this pool
    RewardSchemeConfig reward_scheme_config;

    // How to create initial miners for this pool
    std::vector<MinerConfig> miners_config;
};

struct Simulation {
    // Creates a Simulation from a config file
    static Simulation from_config_file(const std::string& filepath);
    // Creates a Simulation from a stream
    static Simulation from_stream(std::istream& stream);
    // Creates a Simulation from a JSON string
    static Simulation from_string(const std::string& string);

    // The output file for the simulation
    std::string output;

    // The number of blocks to reach before ending the simulation
    uint64_t blocks;

    // Difficulty of the network
    uint64_t network_difficulty;

    // Pools to include in the simulation
    std::vector<PoolConfig> pools;

    // Random seed to use for the simulation
    long seed = 0;
};

void from_json(const nlohmann::json& j, Simulation& simulation);
void from_json(const nlohmann::json& j, PoolConfig& pool_config);
void from_json(const nlohmann::json& j, MinerConfig& miner_config);
void from_json(const nlohmann::json& j, RewardSchemeConfig& reward_scheme_config);

}
