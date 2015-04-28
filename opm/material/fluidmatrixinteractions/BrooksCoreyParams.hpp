/*
  Copyright (C) 2008-2013 by Andreas Lauser
  Copyright (C) 2011 by Holger Class

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/
/*!
 * \file
 * \copydoc Opm::BrooksCoreyParams
 */
#ifndef OPM_BROOKS_COREY_PARAMS_HPP
#define OPM_BROOKS_COREY_PARAMS_HPP

#include <opm/material/common/Valgrind.hpp>

#include <cassert>

namespace Opm {

/*!
 * \ingroup FluidMatrixInteractions
 *
 * \brief Specification of the material parameters for the
 *        Brooks-Corey constitutive relations.
 *
 *\see BrooksCorey
 */
template <class TraitsT>
class BrooksCoreyParams
{
    typedef typename TraitsT::Scalar Scalar;

public:
    typedef TraitsT Traits;

    BrooksCoreyParams()
    {
        Valgrind::SetUndefined(*this);
#ifndef NDEBUG
        finalized_ = false;
#endif
    }

    BrooksCoreyParams(Scalar entryPressure, Scalar lambda)
        : entryPressure_(entryPressure), lambda_(lambda)
    {
        finalize();
    }

    /*!
     * \brief Calculate all dependent quantities once the independent
     *        quantities of the parameter object have been set.
     */
    void finalize()
    {
#ifndef NDEBUG
        finalized_ = true;
#endif
    }

    /*!
     * \brief Returns the entry pressure [Pa]
     */
    Scalar entryPressure() const
    { assertFinalized_(); return entryPressure_; }

    /*!
     * \brief Set the entry pressure [Pa]
     */
    void setEntryPressure(Scalar v)
    { entryPressure_ = v; }


    /*!
     * \brief Returns the lambda shape parameter
     */
    Scalar lambda() const
    { assertFinalized_(); return lambda_; }

    /*!
     * \brief Set the lambda shape parameter
     */
    void setLambda(Scalar v)
    { lambda_ = v; }

private:
#ifndef NDEBUG
    void assertFinalized_() const
    { assert(finalized_); }

    bool finalized_;
#else
    void assertFinalized_() const
    { }
#endif

    Scalar entryPressure_;
    Scalar lambda_;
};
} // namespace Opm

#endif
