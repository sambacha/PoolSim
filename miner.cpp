#include "miner.h"
#include "networkshare.h"
#include "poolshare.h"
#include "miningpool.h"
#inlcude "eventqueue.h"
#include <cmath>
#include <cassert>

Miner::Miner(std::string _adress, double _hashrate, MiningPool *_pool) : address(_address), hashrate(_hashrate), pool(_pool) {
  // initialise member variables to zero

}

bool Miner::schedule_share(EventQueue *queue) {
    assert(queue != NULL);

    if (hashrate <= 0) {
        return false;
    }

    double lambda = (double) hashrate / pool->getShareDiff();
    double t = -log(drand48()) / lambda;
    double p = (double) pool->getShareDiff() / pool->getNetDiff();

    if (drand48() < p) {
        queue->schedule(new NetworkShareEvent(this, queue->getTime() + t));
        return true;
    } else {
      queue->schedule(new MiningPoolShareEvent(this, queue->getTime() + t));
      return true;
    }
}

void Miner::set_credits(unsigned long long _credits) {
  credits = _credits;
}

void Miner::inc_blocks_received() {
  blocks_received++;
}

void Miner::inc_blocks_mined() {
  blocks_mined++;
}

void Miner::inc_uncles_received() {
  uncles_received++;
}

void Miner::inc_uncles_mined() {
  uncles_mined++;
}

void Miner::network_share() {
  pool->network_share(this);
}

void Miner::pool_share() {
  pool->pool_share(this);
}
