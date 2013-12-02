// -*- mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
// vi: set et ts=4 sw=4 sts=4:
/*
  Copyright (C) 2008-2012 by Andreas Lauser

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
 * \copydoc Opm::NullMaterialParams
 */
#ifndef OPM_NULL_MATERIAL_PARAMS_HPP
#define OPM_NULL_MATERIAL_PARAMS_HPP

namespace Opm {
/*!
 * \brief Reference implementation of params for the linear M-phase
       material material.
 */
template<class TraitsT>
class NullMaterialParams
{
public:
    typedef typename TraitsT::Scalar Scalar;

    NullMaterialParams()
    { }

    /*!
     * \brief Finish the construction of the parameter object.
     */
    void finalize()
    { }
};
} // namespace Opm

#endif
