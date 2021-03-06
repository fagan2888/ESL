/// \file   interval.hpp
///
/// \brief
///
/// \authors    Maarten P. Scholl
/// \date       2018-01-31
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
#ifndef ESL_MATHEMATICS_INTERVAL_HPP
#define ESL_MATHEMATICS_INTERVAL_HPP

#include <sstream>
#include <type_traits>

#include <boost/serialization/serialization.hpp>


namespace esl::mathematics {
    ///
    /// \brief  A set of numbers lying between a lower and upper endpoint. The
    ///         endpoints may be included in the set or not, if the lower is
    ///         included the interval is left_closed_, and if the upper is
    ///         included  the interval is right_closed_.
    ///
    /// \tparam number_t_
    /// \tparam left_closed_
    /// \tparam right_closed_
    template<typename number_t_,
             bool left_closed_  = true,
             bool right_closed_ = true>
    struct interval
    {
        ///
        /// \brief Specifies whether the interval includes the lower value
        ///
        /// \return
        constexpr static bool left_closed()
        {
            return left_closed_;
        }

        ///
        /// \brief Specifies whether the interval includes the upper value
        ///
        /// \return
        constexpr static bool right_closed()
        {
            return right_closed_;
        }

        static_assert(std::is_floating_point<number_t_>::value
                      // TODO: || esl::is_rational<number_t_>::value
                      || std::is_integral<number_t_>::value);

        number_t_ lower;
        number_t_ upper;


        ///
        /// \brief  default interval constructor sets the lower and upper bound
        ///         to time_point(0)
        ///
        constexpr interval()
        : lower(0)
        , upper(0)
        {

        }

        ///
        /// \brief  constructs an interval from `lower` to `upper`
        ///
        /// \param lower    lower bound
        /// \param upper    upper bound
        constexpr interval(number_t_ lower, number_t_ upper)
        : lower(lower)
        , upper(upper)
        {

        }

        ///
        /// \return true iff the interval contains no elements
        ///
        [[nodiscard]] constexpr bool empty() const
        {
            if(lower > upper) {
                return true;
            }

            if(lower == upper) {
                return left_closed_ || right_closed_;
            }

            // (lower < upper) is implied
            if(1 == upper - lower){
               return left_closed_ && right_closed_;
            }

            return false;
        }

        ///
        /// \return true iff interval contains exactly one element
        ///
        [[nodiscard]] constexpr bool singleton() const
        {
            bool sufficient_ = !left_closed_ && !right_closed_ && lower == upper;
            if(std::is_floating_point<number_t_>::value || sufficient_) {
                // || TODO: esl::is_rational<number_t_>::value
                return sufficient_;
            }

            // is_integral is implied from here
            bool asymmetric_ =
                (1 == upper - lower) && (left_closed_ != right_closed_);
            bool symmetric_ =
                (2 == upper - lower) && (left_closed_ && right_closed_);
            return (upper > lower) && (asymmetric_ || symmetric_);
        }

        ///
        /// \return true iff the interval is singleton or empty
        ///
        [[nodiscard]] constexpr bool degenerate() const
        {
            return empty() || singleton();
        }

        ///
        /// \param value element to test
        /// \return true iff element is in contained in interval
        ///
        [[nodiscard]] constexpr bool contains(number_t_ value) const
        {
            return (lower < value || (left_closed_ && lower == value))
                   && (upper > value || (right_closed_ && upper == value));
        }

        ///
        /// \brief  renders the interval to a string as detailed in the class's
        ///         ostream operator implementation
        ///
        /// \return
        [[nodiscard]] std::string representation() const
        {
            std::stringstream stream_;
            stream_ << *this;
            return stream_.str();
        }

        ///
        /// \brief  renders the interval using '[' and ']' to denote open lower
        ///         and upper bounds respectively, and '(' and ')' for closed
        ///         lower and upper bounds.
        ///
        /// \param stream
        /// \param self
        /// \return
        friend std::ostream &
        operator<<(std::ostream &stream,
                   const interval<number_t_, left_closed_, right_closed_> &self)
        {
            stream << (left_closed_ ? '[' : '(');
            stream << self.lower << ',' << self.upper;
            stream << (right_closed_ ? ']' : ')');
            return stream;
        }

        ///
        /// \tparam archive_t
        /// \param archive
        /// \param version
        template<class archive_t>
        void serialize(archive_t &archive, const unsigned int version)
        {
            (void)version;
            archive &BOOST_SERIALIZATION_NVP(lower);
            archive &BOOST_SERIALIZATION_NVP(upper);
        }
    };
}  // namespace esl

#endif  // ESL_MATHEMATICS_INTERVAL_HPP
