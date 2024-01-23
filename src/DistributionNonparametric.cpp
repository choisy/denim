//
// Created by thinh on 04/03/2021.
//

#include <algorithm>
#include <stdexcept>
#include "DistributionNonparametric.h"

DistributionNonparametric::DistributionNonparametric(std::vector<double> waitingTime) {
    // Make sure that waiting time distribution is a probability distribution (sum = 1)
    double sumWaitingTime {0};
    for (auto& wt: waitingTime) {
        sumWaitingTime += wt;
    }
    if (sumWaitingTime != 1) {
        for (size_t i {0}; i < waitingTime.size(); ++i) {
            waitingTime[i] /= sumWaitingTime;
        }
    }
    this->waitingTime = waitingTime;
    this->calcTransitionProb();
    this->distName = "nonparametric";
}
void DistributionNonparametric::calcTransitionProb() {
    // Compute transitionProb using waiting time
    for (size_t k {0}; k < waitingTime.size(); ++k) {
        transitionProb.push_back(calcTransitionProbHelper(waitingTime, k));
    }

    // Remember to calculate max day
    this -> maxDay = transitionProb.size();
}

double DistributionNonparametric::getTransitionProb(size_t index) {
    if (index >= transitionProb.size()) {
        return 1;
    } else {
        return transitionProb[index];
    }
}


std::vector<double> DistributionNonparametric::getWaitingTime() {
    return waitingTime;
}