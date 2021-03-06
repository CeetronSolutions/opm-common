/*
  Copyright 2016  Statoil ASA.

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify it under the terms
  of the GNU General Public License as published by the Free Software
  Foundation, either version 3 of the License, or (at your option) any later
  version.

  OPM is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with
  OPM.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OPM_RUNSPEC_HPP
#define OPM_RUNSPEC_HPP

#include <iosfwd>
#include <string>
#include <optional>

#include <opm/common/OpmLog/KeywordLocation.hpp>
#include <opm/parser/eclipse/EclipseState/Tables/Tabdims.hpp>
#include <opm/parser/eclipse/EclipseState/EndpointScaling.hpp>
#include <opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQParams.hpp>
#include <opm/parser/eclipse/EclipseState/Schedule/Action/Actdims.hpp>

namespace Opm {
class Deck;


enum class Phase {
    OIL     = 0,
    GAS     = 1,
    WATER   = 2,
    SOLVENT = 3,
    POLYMER = 4,
    ENERGY  = 5,
    POLYMW  = 6,
    FOAM  = 7,
    BRINE = 8,
    ZFRACTION  = 9

    // If you add more entries to this enum, remember to update NUM_PHASES_IN_ENUM below.
};

constexpr int NUM_PHASES_IN_ENUM = static_cast<int>(Phase::ZFRACTION) + 1;  // Used to get correct size of the bitset in class Phases.

Phase get_phase( const std::string& );
std::ostream& operator<<( std::ostream&, const Phase& );

class Phases {
    public:
        Phases() noexcept = default;
        Phases( bool oil, bool gas, bool wat, bool solvent = false, bool polymer = false, bool energy = false,
                bool polymw = false, bool foam = false, bool brine = false, bool zfraction = false ) noexcept;

        static Phases serializeObject();

        bool active( Phase ) const noexcept;
        size_t size() const noexcept;

        bool operator==(const Phases& data) const;

        template<class Serializer>
        void serializeOp(Serializer& serializer)
        {
            if (serializer.isSerializing())
                serializer(bits.to_ulong());
            else {
              unsigned long Bits = 0;
              serializer(Bits);
              bits = std::bitset<NUM_PHASES_IN_ENUM>(Bits);
            }
        }

    private:
        std::bitset< NUM_PHASES_IN_ENUM > bits;
};


class Welldims {
public:
    Welldims() = default;
    explicit Welldims(const Deck& deck);

    static Welldims serializeObject();

    int maxConnPerWell() const
    {
        return this->nCWMax;
    }

    int maxWellsPerGroup() const
    {
        return this->nWGMax;
    }

    int maxGroupsInField() const
    {
        return this->nGMax;
    }

    int maxWellsInField() const
    {
        return this->nWMax;
    }

    const std::optional<KeywordLocation>& location() const {
        return this->m_location;
    }


    bool operator==(const Welldims& data) const {
        return this->maxConnPerWell() == data.maxConnPerWell() &&
               this->maxWellsPerGroup() == data.maxWellsPerGroup() &&
               this->maxGroupsInField() == data.maxGroupsInField() &&
               this->maxWellsInField() == data.maxWellsInField() &&
               this->location() == data.location();
    }

    template<class Serializer>
    void serializeOp(Serializer& serializer)
    {
        serializer(nWMax);
        serializer(nCWMax);
        serializer(nWGMax);
        serializer(nGMax);
        serializer(m_location);
    }

private:
    int nWMax  { 0 };
    int nCWMax { 0 };
    int nWGMax { 0 };
    int nGMax  { 0 };
    std::optional<KeywordLocation> m_location;
};

class WellSegmentDims {
public:
    WellSegmentDims();
    explicit WellSegmentDims(const Deck& deck);

    static WellSegmentDims serializeObject();


    int maxSegmentedWells() const
    {
        return this->nSegWellMax;
    }

    int maxSegmentsPerWell() const
    {
        return this->nSegmentMax;
    }

    int maxLateralBranchesPerWell() const
    {
        return this->nLatBranchMax;
    }

    bool operator==(const WellSegmentDims& data) const;

    template<class Serializer>
    void serializeOp(Serializer& serializer)
    {
        serializer(nSegWellMax);
        serializer(nSegmentMax);
        serializer(nLatBranchMax);
    }

private:
    int nSegWellMax;
    int nSegmentMax;
    int nLatBranchMax;
};

class NetworkDims {
public:
    NetworkDims();
    explicit NetworkDims(const Deck& deck);

    static NetworkDims serializeObject();

    int maxNONodes() const
    {
        return this->nMaxNoNodes;
    }

    int maxNoBranches() const
    {
        return this->nMaxNoBranches;
    }

    int maxNoBranchesConToNode() const
    {
        return this->nMaxNoBranchesConToNode;
    }

    bool operator==(const NetworkDims& data) const;

    template<class Serializer>
    void serializeOp(Serializer& serializer)
    {
        serializer(nMaxNoNodes);
        serializer(nMaxNoBranches);
        serializer(nMaxNoBranchesConToNode);
    }

private:
    int nMaxNoNodes;
    int nMaxNoBranches;
    int nMaxNoBranchesConToNode;
};


class EclHysterConfig
{
public:
    EclHysterConfig() = default;
    explicit EclHysterConfig(const Deck& deck);

    static EclHysterConfig serializeObject();

    /*!
     * \brief Specify whether hysteresis is enabled or not.
     */
    //void setActive(bool yesno);

    /*!
     * \brief Returns whether hysteresis is enabled (active).
     */
    bool active() const;

    /*!
     * \brief Return the type of the hysteresis model which is used for capillary pressure.
     *
     * -1: capillary pressure hysteresis is disabled
     * 0: use the Killough model for capillary pressure hysteresis
     */
    int pcHysteresisModel() const;

    /*!
     * \brief Return the type of the hysteresis model which is used for relative permeability.
     *
     * -1: relperm hysteresis is disabled
     * 0: use the Carlson model for relative permeability hysteresis
     */
    int krHysteresisModel() const;

    bool operator==(const EclHysterConfig& data) const;

    template<class Serializer>
    void serializeOp(Serializer& serializer)
    {
        serializer(activeHyst);
        serializer(pcHystMod);
        serializer(krHystMod);
    }

private:
    // enable hysteresis at all
    bool activeHyst  { false };

    // the capillary pressure and the relperm hysteresis models to be used
    int pcHystMod { 0 };
    int krHystMod { 0 };
};

class SatFuncControls {
public:
    enum class ThreePhaseOilKrModel {
        Default,
        Stone1,
        Stone2
    };

    enum class KeywordFamily {
        Family_I,               // SGOF, SWOF, SLGOF
        Family_II,              // SGFN, SOF{2,3}, SWFN
        Undefined,
    };

    SatFuncControls();
    explicit SatFuncControls(const Deck& deck);
    explicit SatFuncControls(const double tolcritArg,
                             const ThreePhaseOilKrModel model,
                             const KeywordFamily family);

    static SatFuncControls serializeObject();

    double minimumRelpermMobilityThreshold() const
    {
        return this->tolcrit;
    }

    ThreePhaseOilKrModel krModel() const
    {
        return this->krmodel;
    }

    KeywordFamily family() const
    {
        return this->satfunc_family;
    }

    bool operator==(const SatFuncControls& rhs) const;

    template<class Serializer>
    void serializeOp(Serializer& serializer)
    {
        serializer(tolcrit);
        serializer(krmodel);
        serializer(satfunc_family);
    }

private:
    double tolcrit;
    ThreePhaseOilKrModel krmodel = ThreePhaseOilKrModel::Default;
    KeywordFamily satfunc_family = KeywordFamily::Undefined;
};

class Runspec {
public:
    Runspec() = default;
    explicit Runspec( const Deck& );

    static Runspec serializeObject();

    const UDQParams& udqParams() const noexcept;
    const Phases& phases() const noexcept;
    const Tabdims&  tabdims() const noexcept;
    const EndpointScaling& endpointScaling() const noexcept;
    const Welldims& wellDimensions() const noexcept;
    const WellSegmentDims& wellSegmentDimensions() const noexcept;
    const NetworkDims& networkDimensions() const noexcept;
    int eclPhaseMask( ) const noexcept;
    const EclHysterConfig& hysterPar() const noexcept;
    const Actdims& actdims() const noexcept;
    const SatFuncControls& saturationFunctionControls() const noexcept;
    int nupcol() const noexcept;
    bool co2Storage() const noexcept;

    bool operator==(const Runspec& data) const;

    template<class Serializer>
    void serializeOp(Serializer& serializer)
    {
        active_phases.serializeOp(serializer);
        m_tabdims.serializeOp(serializer);
        endscale.serializeOp(serializer);
        welldims.serializeOp(serializer);
        wsegdims.serializeOp(serializer);
        netwrkdims.serializeOp(serializer);
        udq_params.serializeOp(serializer);
        hystpar.serializeOp(serializer);
        m_actdims.serializeOp(serializer);
        m_sfuncctrl.serializeOp(serializer);
        serializer(m_nupcol);
        serializer(m_co2storage);
    }

private:
    Phases active_phases;
    Tabdims m_tabdims;
    EndpointScaling endscale;
    Welldims welldims;
    WellSegmentDims wsegdims;
    NetworkDims netwrkdims;
    UDQParams udq_params;
    EclHysterConfig hystpar;
    Actdims m_actdims;
    SatFuncControls m_sfuncctrl;
    int m_nupcol;
    bool m_co2storage;
};


}

#endif // OPM_RUNSPEC_HPP
