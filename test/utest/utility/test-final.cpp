// ================================================================================================
//
// vlk  Vulkan support library to experiment with VULKAN SDK
//
// Copyright (C) 2019 Alexander Seifarth
//
// This program is free software; you can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation; either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
// the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this program;
// if not, write to the Free Software Foundation,
//          Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
//
// ================================================================================================
#include <gtest/gtest.h>
#include <vlk/final.h>
#include <exception>

TEST(final, empty)
{
    {
        vlk::final fin{};
    }
}

TEST(final, invoked)
{
    bool invoked{false};
    {
        vlk::final fin{[&invoked]() {invoked = true;}};
        ASSERT_FALSE(invoked);
    }
    ASSERT_TRUE(invoked);
}

TEST(final, reset)
{
    bool invoked{false};
    {
        vlk::final fin{[&invoked]() {invoked = true;}};
        ASSERT_FALSE(invoked);
        fin.reset();
        ASSERT_FALSE(invoked);
    }
    ASSERT_FALSE(invoked);
}

TEST(final, set)
{
    bool inv1{false};
    bool inv2{false};
    {
        vlk::final fin{[&inv1]() {inv1 = true;}};
        ASSERT_FALSE(inv1);
        ASSERT_FALSE(inv2);

        fin.set([&inv2]() {inv2 = true;});
        ASSERT_FALSE(inv1);
        ASSERT_FALSE(inv2);
    }
    ASSERT_FALSE(inv1);
    ASSERT_TRUE(inv2);
}

TEST(final, exception_thrown)
{
    bool inv{false};
    try {
        vlk::final fin{[&inv]() {inv = true;}};
        ASSERT_FALSE(inv);
        if (!inv) throw std::runtime_error{""};
        fin.reset();
    }
    catch (std::runtime_error e) {
        ASSERT_TRUE(inv);
    }
}

TEST(final, exception_not_thrown)
{
    bool inv{false};
    try {
        vlk::final fin{[&inv]() {inv = true;}};
        ASSERT_FALSE(inv);
        if (inv) throw std::runtime_error{""};
        fin.reset();
    }
    catch (std::runtime_error e) {
        ASSERT_FALSE(inv);
    }
}
