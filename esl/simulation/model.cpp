/// \file   model.cpp
///
/// \brief
///
/// \authors    Maarten P. Scholl
/// \date       2017-12-06
/// \copyright  Copyright 2017-2019 The Institute for New Economic Thinking,
///             Oxford Martin School, University of Oxford
///
///             Licensed under the Apache License, Version 2.0 (the "License");
///             you may not use this file except in compliance with the License.
///             You may obtain a copy of the License at
///
///                 http://www.apache.org/licenses/LICENSE-2.0
///
///             Unless required by applicable law or agreed to in writing,
///             software distributed under the License is distributed on an "AS
///             IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
///             express or implied. See the License for the specific language
///             governing permissions and limitations under the License.
///
///             You may obtain instructions to fulfill the attribution
///             requirements in CITATION.cff
///
#include <esl/simulation/model.hpp>

#include <esl/agent.hpp>
#include <esl/computation/environment.hpp>
#include <esl/data/log.hpp>


namespace esl::simulation {
    model::model( computation::environment &e
                , const parameter::parametrization &parameters)
    : environment_(e)
    , parameters(parameters)
    , start(parameters.get<time_point>("start"))
    , end(parameters.get<time_point>("end"))
    , time(parameters.get<time_point>("start"))
    , sample(parameters.get<std::uint64_t>("sample"))
    , agents(e)
    {

    }

    void model::initialize()
    {

    }

    time_point model::step(time_interval step)
    {
        assert(!step.empty());

        environment_.before_step();

        // to be set externally
        std::uint64_t sample_ = this->parameters.get<std::uint64_t>("sample");

        auto first_event_   = step.upper;
        unsigned int round_ = 0;

        unsigned int print_every = 100;

        do {
            if (0==(step.lower%print_every)){
                std::cout <<" time " << step << " round " << round_ << std::endl;
            }
            first_event_   = step.upper;
            for(auto &[i, a] : agents.local_agents_) {
                // The seed is deterministic in the following variables:
                std::seed_seq seed_ {
                    std::uint64_t(std::hash<identity<agent>>()(i)),
                    std::uint64_t(step.lower),
                    std::uint64_t(round_),
                    sample_
                };

                //try {
                    first_event_ = std::min(first_event_,
                                            a->process_messages(step, seed_));
                    first_event_ = std::min(first_event_, a->act(step, seed_));
                /*} catch(const std::runtime_error &e) {

                    std::cout <<  e.what() << std::endl;
                    throw e;
                } catch(const std::exception &e) {
                    std::cout <<  e.what() << std::endl;
                    throw e;
                } catch(...) {
                    throw;
                }*/
                a->inbox.clear();
            }
            environment_.send_messages(*this);
            ++round_;
        } while(step.lower >= first_event_);
        environment_.after_step(*this);

        return first_event_;
    }

    void model::terminate()
    {

    }

}  // namespace esl::simulation


#ifdef WITH_PYTHON
#include <boost/python.hpp>
using namespace boost::python;

using namespace esl::simulation;

BOOST_PYTHON_MODULE(model)
{
    class_<model>(
        "model",
        init< esl::computation::environment &
                , parameter::parametrization>())
        .def_readonly("start", &model::start)
        .def_readwrite("end", &model::end)
        .def_readwrite("time", &model::time)
        ;
}


#endif