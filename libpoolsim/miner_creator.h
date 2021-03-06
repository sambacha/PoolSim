#pragma once

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

#include "miner.h"
#include "network.h"
#include "random.h"
#include "factory.h"

namespace poolsim {

struct MinerCreationState {
  size_t miners_count = 0;
  uint64_t total_hashrate = 0;
  MinerCreationState() = default;
  MinerCreationState(size_t miners_count, uint64_t total_hashrate);
};

class MinerCreatorStopCondition {
public:
  virtual bool should_stop(const MinerCreationState& state) = 0;
};

MAKE_FACTORY(MinerCreatorStopConditionFactory, MinerCreatorStopCondition, const nlohmann::json&)

class TotalHashrateStopCondition :
  public MinerCreatorStopCondition,
  public Creatable1<MinerCreatorStopCondition, TotalHashrateStopCondition, const nlohmann::json&> {
public:
  explicit TotalHashrateStopCondition(const nlohmann::json& args);
  bool should_stop(const MinerCreationState& state);
private:
  uint64_t max_hashrate;
};

class MinersCountStopCondition :
  public MinerCreatorStopCondition,
  public Creatable1<MinerCreatorStopCondition, MinersCountStopCondition, const nlohmann::json&> {
public:
  explicit MinersCountStopCondition(const nlohmann::json& args);
  bool should_stop(const MinerCreationState& state);
private:
  uint64_t max_count;
};



class MinerCreator {
public:
    explicit MinerCreator(std::shared_ptr<Network> network);
    MinerCreator(std::shared_ptr<Network> network, std::shared_ptr<Random> _random);
    virtual std::vector<std::shared_ptr<Miner>> create_miners(const nlohmann::json& args) = 0;
protected:
    std::shared_ptr<Network> network;
    std::shared_ptr<Random> random;
};

class CSVMinerCreator : public MinerCreator,
                        public Creatable1<MinerCreator, CSVMinerCreator, std::shared_ptr<Network>> {
public:
    explicit CSVMinerCreator(std::shared_ptr<Network> network);
    std::vector<std::shared_ptr<Miner>> create_miners(const nlohmann::json& args) override;
};

class RandomMinerCreator : public MinerCreator,
                           public Creatable1<MinerCreator, RandomMinerCreator, std::shared_ptr<Network>> {
public:
    explicit RandomMinerCreator(std::shared_ptr<Network> network);
    std::vector<std::shared_ptr<Miner>> create_miners(const nlohmann::json& args) override;
};

class InlineMinerCreator : public MinerCreator,
                           public Creatable1<MinerCreator, InlineMinerCreator, std::shared_ptr<Network>> {
public:
    explicit InlineMinerCreator(std::shared_ptr<Network> network);
    std::vector<std::shared_ptr<Miner>> create_miners(const nlohmann::json& args) override;
};



MAKE_FACTORY(MinerCreatorFactory, MinerCreator, std::shared_ptr<Network>)

}
