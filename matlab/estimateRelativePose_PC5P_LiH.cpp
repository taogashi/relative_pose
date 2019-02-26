#include "mex.h"
#include <opencv2/matlab/bridge.hpp>
#include <opencv2/matlab/mxarray.hpp>

#include "relative_pose/relative_pose.hpp"
#include "../src/relative_pose/precomp.hpp"

#define _DO_NOT_EXPORT
#if defined(_DO_NOT_EXPORT)
#define DllExport  
#else
#define DllExport __declspec(dllexport)
#endif

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    using namespace cv;
    matlab::conditionalError(nrhs == 4, "Need 4 input arguments");

    matlab::MxArrayVector raw(prhs, prhs+nrhs);
    matlab::conditionalError(raw.at(2).size() == 1 && raw.at(3).size() == 1,
            "Input 2 or 3 is not scalar");

    matlab::ArgumentParser parser("what");
    parser.addVariant("estimateRelativePose_PC5P_LiH", 4, 0);

    matlab::MxArrayVector reordered = parser.parse(raw);
    bridge::BridgeVector inputs(reordered.begin(), reordered.end());

    Mat rays1 = inputs.at(0).toMat(),
        rays2 = inputs.at(1).toMat();
    double thresh = inputs.at(2).toDouble(),
           prob = inputs.at(3).toDouble();

    Mat E = estimateRelativePose_PC5P_LiH(rays1, rays2,
            RANSAC, prob, thresh, noArray());

    bridge::Bridge output;
    output = E;
    plhs[0] = output.toMxArray().releaseOwnership();
}
