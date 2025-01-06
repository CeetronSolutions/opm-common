
/*
  Copyright (c) 2016 Robert W. Rose
  Copyright (c) 2018 Paul Maevskikh
  Copyright (c) 2024 NORCE
  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <filesystem>
#include <fmt/format.h>
#include <iostream>
#include <opm/common/ErrorMacros.hpp>

namespace fs = std::filesystem;

using namespace Opm;

template <class Evaluation>
bool
test_scalingdense_10x1(Evaluation* load_time, Evaluation* apply_time)
{
    printf("TEST scalingdense_10x1\n");

    OPM_ERROR_IF(!load_time, "Invalid Evaluation");
    OPM_ERROR_IF(!apply_time, "Invalid Evaluation");

    Opm::ML::Tensor<Evaluation> in {10};
    in.data_ = {0.4282019,
                0.59699935,
                0.10826572,
                0.34815696,
                0.06803443,
                0.4096877,
                0.44572437,
                0.5300221,
                0.22084814,
                0.6827634};

    Opm::ML::Tensor<Evaluation> out {10};
    out.data_
        = {337.923, 363.04617, 325.79288, 347.2265, 373.64267, 352.02765, 350.22614, 306.45822, 356.82947, 331.95105};

    Opm::ML::NNTimer load_timer;
    load_timer.start();

    Opm::ML::NNModel<Evaluation> model;
    OPM_ERROR_IF(!model.loadModel("./tests/ml/ml_tools/models/test_scalingdense_10x1.model"), "Failed to load model");

    *load_time = load_timer.stop();

    Opm::ML::NNTimer apply_timer;
    apply_timer.start();

    Opm::ML::Tensor<Evaluation> predict = out;
    OPM_ERROR_IF(!model.apply(in, out), "Failed to apply");

    *apply_time = apply_timer.stop();

    for (int i = 0; i < out.dims_[0]; i++) {
        OPM_ERROR_IF((fabs(out(i).value() - predict(i).value()) > 1e-3),
                     fmt::format(" Expected "
                                 "{}"
                                 " got "
                                 "{}",
                                 predict(i).value(),
                                 out(i).value()));
    }

    return true;
}
