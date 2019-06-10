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
#pragma once

#include <functional>

namespace vlk {

    //! \brief Final object to do cleanup tasks when object's dtor is called.
    //! A 'final' object holds a functor object that will be invoked when its destructor is called.
    //! Its main use is to do cleanup task at the end of functions:
    //! \begincode{C++}
    //! void myfunction() {
    //!    vlk::final final_cleanup([]() { // cleanup code here // });
    //!    ....
    //!    // dtor of final_cleanup called here --> lambda called.
    //! }
    //! \endcode
    //! The second scenario is to use it as cleanup for exceptions to avoid sub-optimal try-catch clauses:
    //! void my_throwing_function() {
    //!    vlk::final exception_cleanup([]() { // cleanup when exception occurred here // });
    //!    ...
    //!    if ( //some-bad-condition) {
    //!      throw my_exception();
    //!    }
    //!    ....
    //!    excetpion_cleanup.reset(); // prevent exception cleanup called here when no exception occured
    //! }
    class final
    {
    public:
        using functor_type = void();

        //! Creates an empty final object, e.g. one without callable functor.
        final();

        //! Creates a final object with the functor f callable in its destructor.
        template<typename F>
        explicit final(F f);
        ~final();

        final(final const&) = delete;
        final& operator=(final const&) = delete;

        //! Deletes the functor stored inside the final object.
        void reset();

        //! Sets a new functor object f in the final object.
        template<typename F>
        void set(F f);

    private:
        std::function<functor_type> _functor;
    };

} // namespace vlk

inline
vlk::final::final()
    : final{std::function<functor_type>{}}
{}

template<typename F>
vlk::final::final(F f)
    : _functor{f}
{}

inline
vlk::final::~final()
{
    if (_functor) {
        _functor();
    }
}

template<typename F>
void vlk::final::set(F f)
{
    _functor = f;
}

inline
void vlk::final::reset()
{
    _functor = decltype(_functor){};
}
