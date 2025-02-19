// Copyright (C) 2022 - 2022 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <boost/scope_exit.hpp>
#include <gtest/gtest.h>
#include <math.h>
#include <stdexcept>
#include <utility>
#include <vector>

#include "../../shared/gpubuf.h"
#include "../rocfft_params.h"
#include "fftw_transform.h"
#include "rocfft.h"
#include "rocfft_accuracy_test.h"
#include "rocfft_against_fftw.h"

void fft_vs_reference(rocfft_params& params)
{
    switch(params.precision)
    {
    case fft_precision_single:
        fft_vs_reference_impl<float, rocfft_params>(params);
        break;
    case fft_precision_double:
        fft_vs_reference_impl<double, rocfft_params>(params);
        break;
    }
}

// Test for comparison between FFTW and rocFFT.
TEST_P(accuracy_test, vs_fftw)
{
    rocfft_params params(GetParam());

    params.validate();

    // Test that the tokenization works as expected.
    auto       token = params.token();
    fft_params tokentest;
    tokentest.from_token(token);
    auto token1 = tokentest.token();
    EXPECT_TRUE(token == token1);

    if(!params.valid(verbose))
    {
        if(verbose)
        {
            std::cout << "Invalid parameters, skip this test." << std::endl;
        }
        GTEST_SKIP();
    }

    fft_vs_reference(params);
    SUCCEED();
}
