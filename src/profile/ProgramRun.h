/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2016, The Souffle Developers. All rights reserved
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

#pragma once

#include "Relation.h"
#include "StringUtils.h"
#include "Table.h"
#include <chrono>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace souffle {
namespace profile {

/*
 * Stores the relations of the program
 * ProgramRun -> Relations -> Iterations/Rules
 */
class ProgramRun {
private:
    std::unordered_map<std::string, std::shared_ptr<Relation>> relation_map;
    std::chrono::microseconds startTime{0};
    std::chrono::microseconds endTime{0};

    double tot_rec_tup = 0.0;
    double tot_copy_time = 0.0;

public:
    ProgramRun() : relation_map() {}

    inline void setStarttime(std::chrono::microseconds time) {
        startTime = time;
    }

    inline void setEndtime(std::chrono::microseconds time) {
        endTime = time;
    }

    inline void setRelation_map(std::unordered_map<std::string, std::shared_ptr<Relation>>& relation_map) {
        this->relation_map = relation_map;
    }

    inline void update() {
        tot_rec_tup = (double)getTotNumRecTuples();
        tot_copy_time = getTotCopyTime();
    };

    std::string toString() {
        std::ostringstream output;
        output << "ProgramRun:" << getRuntime() << "\nRelations:\n";
        for (auto r = relation_map.begin(); r != relation_map.end(); ++r) {
            output << r->second->toString() << "\n";
        }
        return output.str();
    }

    inline std::unordered_map<std::string, std::shared_ptr<Relation>>& getRelation_map() {
        return relation_map;
    }

    std::string getRuntime() {
        if (startTime == endTime) {
            return "--";
        }
        return formatTime((endTime - startTime).count() / 1000000);
    }

    std::chrono::microseconds getStarttime() const {
        return startTime;
    }

    std::chrono::microseconds getEndtime() const {
        return endTime;
    }

    double getTotLoadtime() {
        double result = 0;
        for (auto& item : relation_map) {
            result += item.second->getLoadtime();
        }
        return result;
    }

    double getTotSavetime() {
        double result = 0;
        for (auto& item : relation_map) {
            result += item.second->getSavetime();
        }
        return result;
    }

    long getTotNumTuples() {
        long result = 0;
        for (auto& item : relation_map) {
            result += item.second->getTotNum_tuples();
        }
        return result;
    }

    long getTotNumRecTuples() {
        long result = 0;
        for (auto& item : relation_map) {
            result += item.second->getTotNumRec_tuples();
        }
        return result;
    }

    double getTotCopyTime() {
        double result = 0;
        for (auto& item : relation_map) {
            result += item.second->getCopyTime();
        }
        return result;
    }

    double getTotTime() {
        double result = 0;
        for (auto& item : relation_map) {
            result += item.second->getRecTime();
        }
        return result;
    }

    Relation* getRelation(std::string name) {
        if (relation_map.find(name) != relation_map.end()) {
            return &(*relation_map[name]);
        }
        return nullptr;
    }

    std::set<std::shared_ptr<Relation>> getRelationsAtTime(double start, double end) {
        std::set<std::shared_ptr<Relation>> result;
        for (auto& cur : relation_map) {
            if (cur.second->getStarttime() <= end && cur.second->getEndtime() >= start) {
                result.insert(cur.second);
            }
        }
        return result;
    }

    inline std::string formatTime(double runtime) {
        return Tools::formatTime(runtime);
    }

    inline std::string formatNum(int precision, long number) {
        return Tools::formatNum(precision, number);
    }

    inline std::vector<std::vector<std::string>> formatTable(Table table, int precision) {
        return Tools::formatTable(table, precision);
    }
};

}  // namespace profile
}  // namespace souffle
