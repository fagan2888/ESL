/// \file   contract.hpp
///
/// \brief  A contract generates obligations
///
/// \authors    Maarten P. Scholl
/// \date       2018-04-02
/// \copyright  Copyright 2017-2019 The Institute for New Economic Thinking, Oxford Martin School, University of Oxford
///
///             Licensed under the Apache License, Version 2.0 (the "License");
///             you may not use this file except in compliance with the License.
///             You may obtain a copy of the License at
///
///                 http://www.apache.org/licenses/LICENSE-2.0
///
///             Unless required by applicable law or agreed to in writing, software
///             distributed under the License is distributed on an "AS IS" BASIS,
///             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
///             See the License for the specific language governing permissions and
///             limitations under the License.
///
///             You may obtain instructions to fulfill the attribution requirements in CITATION.cff
///
#ifndef ESL_CONTRACT_HPP
#define ESL_CONTRACT_HPP


#include <esl/economics/fungibility.hpp>
#include <esl/economics/tangibility.hpp>
#include <esl/law/property.hpp>

class agent;

namespace esl::law {

    template<size_t parties_ = 2>
    struct contract
    : public property
    , public infungible
    , public intangible
    {
    private:
        static_assert(parties_ >= 2);

    public:
        identity<agent> parties[parties_];

        contract(identity<contract<parties_>> i)
        : property(), infungible<contract<parties_>>(i), intangible()
        {

        }

        virtual ~contract() = default;
    };

}

#endif //ESL_CONTRACT_HPP