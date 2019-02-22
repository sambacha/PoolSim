#include <fstream>
#include <iostream>

#include <spdlog/spdlog.h>

#include "simulator.h"
#include "miner.h"
#include "event.h"
#include "miner_creator.h"


Simulator::Simulator(Simulation _simulation)
  : Simulator(_simulation, SystemRandom::get_instance()) {}

Simulator::Simulator(Simulation _simulation, std::shared_ptr<Random> _random)
  : simulation(_simulation), random(_random), current_time(0), blocks_mined(0) {}


void Simulator::initialize() {
  for (auto pool_config : simulation.pools) {
    auto miner_config = pool_config.miner_config;
    auto miner_creator = MinerCreatorFactory::create(miner_config.generator);
    auto new_miners = miner_creator->create_miners(miner_config.params);
    auto pool = std::make_shared<MiningPool>(pool_config.difficulty);
    for (auto miner : new_miners) {
      miner->join_pool(pool);
      add_miner(miner);
    }
    add_pool(pool);
  }
}

void Simulator::run() {
  initialize();

  if (pools.empty() || miners.empty()) {
    throw InvalidSimulationException("simulation must have at least one miner and one pool");
  }

  schedule_all();

  spdlog::info("running {} blocks", simulation.blocks);
  while (get_blocks_mined() <= simulation.blocks) {
    auto event = queue.pop();
    process_event(event);
  }
}


void Simulator::schedule_all() {
  for (auto miner_kv: miners) {
    schedule_miner(miner_kv.second);
  }
}

void Simulator::process_event(const Event& event) {
  current_time = event.time;
  auto miner = get_miner(event.miner_address);
  auto pool = miner->get_pool();
  double p = (double) pool->get_difficulty() / simulation.network_difficulty;
  bool is_network_share = random->drand48() < p;
  if (is_network_share) {
    blocks_mined++;
    spdlog::debug("new block mined: {} / {}", blocks_mined, simulation.blocks);
  }
  Share share(is_network_share);
  schedule_miner(miner);
  miner->process_share(share);
}

void Simulator::schedule_miner(const std::shared_ptr<Miner> miner) {
  auto pool = miner->get_pool();
  double lambda = miner->get_hashrate() / pool->get_difficulty();
  double t = -log(random->drand48()) / lambda;

  Event miner_next_event(miner->get_address(), current_time + t);
  queue.schedule(miner_next_event);
}

void Simulator::add_miner(std::shared_ptr<Miner> miner) {
  miners[miner->get_address()] = miner;
}

void Simulator::add_pool(std::shared_ptr<MiningPool> pool) {
  pools.push_back(pool);
}

std::shared_ptr<Miner> Simulator::get_miner(const std::string& miner_address) {
  return miners[miner_address];
}

double Simulator::get_current_time() const {
  return current_time;
}

uint64_t Simulator::get_blocks_mined() const {
  return blocks_mined;
}

size_t Simulator::get_miners_count() const {
  return miners.size();
}

size_t Simulator::get_pools_count() const {
  return pools.size();
}

size_t Simulator::get_events_count() const {
  return queue.size();
}

Event Simulator::get_next_event() const {
  return queue.get_top();
}
