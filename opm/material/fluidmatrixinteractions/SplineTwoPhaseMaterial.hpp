/*
  Copyright (C) 2009-2013 by Andreas Lauser

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
 * \copydoc Opm::SplineTwoPhaseMaterial
 */
#ifndef OPM_SPLINE_TWO_PHASE_MATERIAL_HPP
#define OPM_SPLINE_TWO_PHASE_MATERIAL_HPP

#include "SplineTwoPhaseMaterialParams.hpp"

#include <opm/core/utility/ErrorMacros.hpp>
#include <opm/core/utility/Exceptions.hpp>

#include <algorithm>
#include <cmath>
#include <cassert>

namespace Opm {
/*!
 * \ingroup FluidMatrixInteractions
 *
 * \brief Implementation of a tabulated capillary pressure and relperm law which uses
 *        spline curves as interpolation functions.
 */
template <class TraitsT, class ParamsT = SplineTwoPhaseMaterialParams<TraitsT> >
class SplineTwoPhaseMaterial : public TraitsT
{
    typedef typename ParamsT::SamplePoints SamplePoints;

public:
    //! The traits class for this material law
    typedef TraitsT Traits;

    //! The type of the parameter objects for this law
    typedef ParamsT Params;

    //! The type of the scalar values for this law
    typedef typename Traits::Scalar Scalar;

    //! The number of fluid phases
    static const int numPhases = Traits::numPhases;
    static_assert(numPhases == 2,
                  "The piecewise linear two-phase capillary pressure law only"
                  "applies to the case of two fluid phases");

    //! Specify whether this material law implements the two-phase
    //! convenience API
    static const bool implementsTwoPhaseApi = true;

    //! Specify whether this material law implements the two-phase
    //! convenience API which only depends on the phase saturations
    static const bool implementsTwoPhaseSatApi = true;

    //! Specify whether the quantities defined by this material law
    //! are saturation dependent
    static const bool isSaturationDependent = true;

    //! Specify whether the quantities defined by this material law
    //! are dependent on the absolute pressure
    static const bool isPressureDependent = false;

    //! Specify whether the quantities defined by this material law
    //! are temperature dependent
    static const bool isTemperatureDependent = false;

    //! Specify whether the quantities defined by this material law
    //! are dependent on the phase composition
    static const bool isCompositionDependent = false;

    /*!
     * \brief The capillary pressure-saturation curve.
     */
    template <class Container, class FluidState>
    static void capillaryPressures(Container &values, const Params &params, const FluidState &fs)
    {
        values[Traits::wettingPhaseIdx] = 0.0; // reference phase
        values[Traits::nonWettingPhaseIdx] = pcnw(params, fs);
    }

    /*!
     * \brief The saturations of the fluid phases starting from their
     *        pressure differences.
     */
    template <class Container, class FluidState>
    static void saturations(Container &values, const Params &params, const FluidState &fs)
    { OPM_THROW(std::logic_error, "Not implemented: saturations()"); }

    /*!
     * \brief The relative permeabilities
     */
    template <class Container, class FluidState>
    static void relativePermeabilities(Container &values, const Params &params, const FluidState &fs)
    {
        values[Traits::wettingPhaseIdx] = krw(params, fs);
        values[Traits::nonWettingPhaseIdx] = krn(params, fs);
    }

    /*!
     * \brief The capillary pressure-saturation curve
     */
    template <class FluidState>
    static Scalar pcnw(const Params &params, const FluidState &fs)
    {
        Scalar Sw = fs.saturation(Traits::wettingPhaseIdx);

        return twoPhaseSatPcnw(params, Sw);
    }

    /*!
     * \brief The saturation-capillary pressure curve
     */
    static Scalar twoPhaseSatPcnw(const Params &params, Scalar Sw)
    { return params.pcnwSpline().eval(Sw, /*extrapolate=*/true); }

    /*!
     * \brief The saturation-capillary pressure curve
     */
    template <class FluidState>
    static Scalar Sw(const Params &params, const FluidState &fs)
    { OPM_THROW(std::logic_error, "Not implemented: Sw()"); }

    static Scalar twoPhaseSatSw(const Params &params, Scalar pC)
    { OPM_THROW(std::logic_error, "Not implemented: twoPhaseSatSw()"); }

    /*!
     * \brief Calculate the non-wetting phase saturations depending on
     *        the phase pressures.
     */
    template <class FluidState>
    static Scalar Sn(const Params &params, const FluidState &fs)
    { return 1 - Sw(params, fs); }

    static Scalar twoPhaseSatSn(const Params &params, Scalar pC)
    { return 1 - twoPhaseSatSw(params, pC); }

    /*!
     * \brief The partial derivative of the capillary pressure with
     *        regard to the saturation
     */
    template <class FluidState>
    static Scalar dPcnw_dSw(const Params &params, const FluidState &fs)
    { return twoPhaseSatDPcnw_dSw(params, fs.saturation(Traits::wettingPhaseIdx)); }

    static Scalar twoPhaseSatDPcnw_dSw(const Params &params, Scalar Sw)
    {
        assert(0 < Sw && Sw < 1);
        return params.pcnwSpline().evalDerivative(Sw, /*extrapolate=*/true);
    }

    /*!
     * \brief The relative permeability for the wetting phase of the
     *        porous medium
     */
    template <class FluidState>
    static Scalar krw(const Params &params, const FluidState &fs)
    { return twoPhaseSatKrw(params, fs.saturation(Traits::wettingPhaseIdx)); }

    static Scalar twoPhaseSatKrw(const Params &params, Scalar Sw)
    { return std::max(0.0, std::min(1.0, params.krwSpline().eval(Sw, /*extrapolate=*/true))); }

    /*!
     * \brief The derivative of the relative permeability of the
     *        wetting phase in regard to the wetting saturation of the
     *        porous medium
     */
    template <class FluidState>
    static Scalar dKrw_dSw(const Params &params, const FluidState &fs)
    { return twoPhaseSatDkrw_dSw(params, fs.saturation(Traits::wettingPhaseIdx)); }

    static Scalar twoPhaseSatDKrw_dSw(const Params &params, Scalar Sw)
    { return params.krwSpline().evalDerivative(Sw, /*extrapolate=*/true); }

    /*!
     * \brief The relative permeability for the non-wetting phase
     *        of the porous medium
     */
    template <class FluidState>
    static Scalar krn(const Params &params, const FluidState &fs)
    { return twoPhaseSatKrn(params, 1.0 - fs.saturation(Traits::nonWettingPhaseIdx)); }

    static Scalar twoPhaseSatKrn(const Params &params, Scalar Sw)
    { return std::max(0.0, std::min(1.0, params.krnSpline().eval(Sw, /*extrapolate=*/true))); }

    /*!
     * \brief The derivative of the relative permeability for the
     *        non-wetting phase in regard to the wetting saturation of
     *        the porous medium
     */
    template <class FluidState>
    static Scalar dKrn_dSw(const Params &params, const FluidState &fs)
    { return twoPhaseSatDkrn_dSw(params, fs.saturation(Traits::wettingPhaseIdx)); }

    static Scalar twoPhaseSatDKrn_dSw(const Params &params, Scalar Sw)
    { return params.krnSpline().evalDerivative(Sw, /*extrapolate=*/true); }
};
} // namespace Opm

#endif
