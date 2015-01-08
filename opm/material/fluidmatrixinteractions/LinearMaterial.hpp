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
 * \copydoc Opm::LinearMaterial
 */
#ifndef OPM_LINEAR_MATERIAL_HPP
#define OPM_LINEAR_MATERIAL_HPP

#include "LinearMaterialParams.hpp"

#include <opm/material/Valgrind.hpp>

#include <opm/core/utility/Exceptions.hpp>
#include <opm/core/utility/ErrorMacros.hpp>

#include <algorithm>
#include <type_traits>

namespace Opm {

/*!
 * \ingroup material
 *
 * \brief Implements a linear saturation-capillary pressure relation
 *
 * Implements a linear saturation-capillary pressure relation for
 * M-phase fluid systems.
 *
 * \sa LinearMaterialParams
 */
template <class TraitsT, class ParamsT = LinearMaterialParams<TraitsT> >
class LinearMaterial : public TraitsT
{
public:
    typedef TraitsT Traits;
    typedef ParamsT Params;
    typedef typename Traits::Scalar Scalar;

    //! The number of fluid phases
    static const int numPhases = Traits::numPhases;

    //! Specify whether this material law implements the two-phase
    //! convenience API
    static const bool implementsTwoPhaseApi = (numPhases == 2);

    //! Specify whether this material law implements the two-phase
    //! convenience API which only depends on the phase saturations
    static const bool implementsTwoPhaseSatApi = (numPhases == 2);

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
     * \brief The linear capillary pressure-saturation curve.
     *
     * This material law is linear:
     * \f[
     p_C = (1 - \overline{S}_w) (p_{C,max} - p_{C,entry}) + p_{C,entry}
     \f]
     *
     * \param values Container for the return values
     * \param params Parameters
     * \param state The fluid state
     */
    template <class ContainerT, class FluidState>
    static void capillaryPressures(ContainerT &values,
                                   const Params &params,
                                   const FluidState &state)
    {
        for (int phaseIdx = 0; phaseIdx < Traits::numPhases; ++phaseIdx) {
            Scalar S = state.saturation(phaseIdx);
            Valgrind::CheckDefined(S);

            values[phaseIdx] =
                S*params.pcMaxSat(phaseIdx) +
                (1.0 - S)*params.pcMinSat(phaseIdx);
        }
    }

    /*!
     * \brief The inverse of the capillary pressure
     */
    template <class ContainerT, class FluidState>
    static void saturations(ContainerT &values,
                            const Params &params,
                            const FluidState &state)
    {
        OPM_THROW(std::runtime_error, "Not implemented: LinearMaterial::saturations()");
    }

    /*!
     * \brief The relative permeability of all phases.
     */
    template <class ContainerT, class FluidState>
    static void relativePermeabilities(ContainerT &values,
                                       const Params &params,
                                       const FluidState &state)
    {
        for (int phaseIdx = 0; phaseIdx < Traits::numPhases; ++phaseIdx) {
            Scalar S = state.saturation(phaseIdx);
            Valgrind::CheckDefined(S);

            values[phaseIdx] = std::max(std::min(S,1.0),0.0);
        }
    }

    /*!
     * \brief The difference between the pressures of the non-wetting and wetting phase.
     */
    template <class FluidState>
    static Scalar pcnw(const Params &params, const FluidState &fs)
    {
        Scalar S = fs.saturation(Traits::wettingPhaseIdx);
        Valgrind::CheckDefined(S);

        Scalar wPhasePressure =
            S*params.pcMaxSat(Traits::wettingPhaseIdx) +
            (1.0 - S)*params.pcMinSat(Traits::wettingPhaseIdx);

        S = fs.saturation(Traits::nonWettingPhaseIdx);
        Valgrind::CheckDefined(S);

        Scalar nPhasePressure =
            S*params.pcMaxSat(Traits::nonWettingPhaseIdx) +
            (1.0 - S)*params.pcMinSat(Traits::nonWettingPhaseIdx);

        return nPhasePressure - wPhasePressure;
    }


    template <class ScalarT = Scalar>
    static typename std::enable_if<Traits::numPhases == 2, ScalarT>::type
    twoPhaseSatPcnw(const Params &params, Scalar Sw)
    {
        Scalar wPhasePressure =
            Sw*params.pcMaxSat(Traits::wettingPhaseIdx) +
            (1.0 - Sw)*params.pcMinSat(Traits::wettingPhaseIdx);

        Scalar nPhasePressure =
            (1.0 - Sw)*params.pcMaxSat(Traits::nonWettingPhaseIdx) +
            Sw*params.pcMinSat(Traits::nonWettingPhaseIdx);

        return nPhasePressure - wPhasePressure;
    }

    /*!
     * \brief Calculate wetting phase saturation given that the rest
     *        of the fluid state has been initialized
     */
    template <class FluidState>
    static Scalar Sw(const Params &params, const FluidState &fs)
    { OPM_THROW(std::runtime_error, "Not implemented: Sw()"); }

    template <class ScalarT = Scalar>
    static typename std::enable_if<Traits::numPhases == 2, ScalarT>::type
    twoPhaseSatSw(const Params &params, Scalar Sw)
    { OPM_THROW(std::runtime_error, "Not implemented: twoPhaseSatSw()"); }

    /*!
     * \brief Calculate non-wetting liquid phase saturation given that
     *        the rest of the fluid state has been initialized
     */
    template <class FluidState>
    static Scalar Sn(const Params &params, const FluidState &fs)
    { OPM_THROW(std::runtime_error, "Not implemented: Sn()"); }

    template <class ScalarT = Scalar>
    static typename std::enable_if<Traits::numPhases == 2, ScalarT>::type
    twoPhaseSatSn(const Params &params, Scalar Sw)
    { OPM_THROW(std::runtime_error, "Not implemented: twoPhaseSatSn()"); }

    /*!
     * \brief Calculate gas phase saturation given that the rest of
     *        the fluid state has been initialized
     *
     * This method is only available for at least three fluid phases
     */
    template <class FluidState, class ScalarT = Scalar>
    static typename std::enable_if< (Traits::numPhases > 2), ScalarT>::type
    Sg(const Params &params, const FluidState &fs)
    { OPM_THROW(std::runtime_error, "Not implemented: Sg()"); }

    /*!
     * \brief The relative permability of the wetting phase
     */
    template <class FluidState>
    static Scalar krw(const Params &params, const FluidState &fs)
    { return std::max(0.0, std::min(1.0, fs.saturation(Traits::wettingPhaseIdx))); }

    template <class ScalarT = Scalar>
    static typename std::enable_if<Traits::numPhases == 2, ScalarT>::type
    twoPhaseSatKrw(const Params &params, Scalar Sw)
    { return std::max(0.0, std::min(1.0, Sw)); }

    /*!
     * \brief The relative permability of the liquid non-wetting phase
     */
    template <class FluidState>
    static Scalar krn(const Params &params, const FluidState &fs)
    { return std::max(0.0, std::min(1.0, fs.saturation(Traits::nonWettingPhaseIdx))); }

    template <class ScalarT = Scalar>
    static typename std::enable_if<Traits::numPhases == 2, ScalarT>::type
    twoPhaseSatKrn(const Params &params, Scalar Sw)
    { return std::max(0.0, std::min(1.0, 1 - Sw)); }

    /*!
     * \brief The relative permability of the gas phase
     *
     * This method is only available for at least three fluid phases
     */
    template <class FluidState, class ScalarT=Scalar>
    static typename std::enable_if< (Traits::numPhases > 2), ScalarT>::type
    krg(const Params &params, const FluidState &fs)
    { return std::max(0.0, std::min(1.0, fs.saturation(Traits::gasPhaseIdx))); }

    /*!
     * \brief The difference between the pressures of the gas and the non-wetting phase.
     *
     * This method is only available for at least three fluid phases
     */
    template <class FluidState, class ScalarT=Scalar>
    static typename std::enable_if< (Traits::numPhases > 2), ScalarT>::type
    pcgn(const Params &params, const FluidState &fs)
    {
        Scalar S = fs.saturation(Traits::nonWettingPhaseIdx);
        Valgrind::CheckDefined(S);

        Scalar nPhasePressure =
            S*params.pcMaxSat(Traits::nonWettingPhaseIdx) +
            (1.0 - S)*params.pcMinSat(Traits::nonWettingPhaseIdx);

        S = fs.saturation(Traits::gasPhaseIdx);
        Valgrind::CheckDefined(S);

        Scalar gPhasePressure =
            S*params.pcMaxSat(Traits::gasPhaseIdx) +
            (1.0 - S)*params.pcMinSat(Traits::gasPhaseIdx);

        return gPhasePressure - nPhasePressure;
    }
};
} // namespace Opm

#endif
