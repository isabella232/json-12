//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/vinniefalco/json
//

// Test that header file is self-contained.
#include <boost/json/storage_ptr.hpp>

#include "test.hpp"
#include "test_suite.hpp"

namespace boost {
namespace json {

class storage_ptr_test
{
public:
    struct throwing
        : memory_resource
    {
        throwing()
        {
            throw std::exception{};
        }

        void*
        do_allocate(
            std::size_t,
            std::size_t) override
        {
            return nullptr;
        }

        void
        do_deallocate(
            void*,
            std::size_t,
            std::size_t) noexcept override
        {
        }

        bool
        do_is_equal(
            memory_resource const&) const noexcept override
        {
            return false;
        }
    };

    void
    testMembers()
    {
        auto const dsp = storage_ptr{};
        auto const usp =
            make_counted_resource<unique_resource>();

        // ~storage_ptr()
        {
            // implied
        }

        // storage_ptr()
        {
            storage_ptr sp;
            BOOST_TEST(sp.get());
        }

        // storage_ptr(storage_ptr&&)
        {
            storage_ptr sp1 = dsp;
            storage_ptr sp2(std::move(sp1));
            BOOST_TEST(sp1.get());
            BOOST_TEST(*sp2 == *dsp);
        }

        // storage_ptr(storage_ptr const&)
        {
            storage_ptr sp1 = dsp;
            storage_ptr sp2(sp1);
            BOOST_TEST(sp1 == sp2);
        }

        // operator=(storage_ptr&&)
        {
            storage_ptr sp1(dsp);
            storage_ptr sp2(usp);
            sp2 = std::move(sp1);
            BOOST_TEST(*sp2 == *dsp);
        }

        // operator=(storage_ptr const&)
        {
            storage_ptr sp1(dsp);
            storage_ptr sp2(usp);
            sp2 = sp1;
            BOOST_TEST(*sp1 == *sp2);
        }

        // get()
        {
            storage_ptr sp(dsp);
            BOOST_TEST(sp.get() == dsp.get());
        }

        // operator->()
        {
            storage_ptr sp(dsp);
            BOOST_TEST(sp.operator->() == dsp.get());
        }

        // operator*()
        {
            storage_ptr sp(dsp);
            BOOST_TEST(&sp.operator*() == dsp.get());
        }

        // exception in make_storage
        {
            BOOST_TEST_THROWS(
                make_counted_resource<throwing>(),
                std::exception);
        }
    }

    void
    run()
    {
        testMembers();
    }
};

TEST_SUITE(storage_ptr_test, "boost.json.storage_ptr");

} // json
} // boost
