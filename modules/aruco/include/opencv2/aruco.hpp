// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
#ifndef OPENCV_ARUCO_HPP
#define OPENCV_ARUCO_HPP

#include "opencv2/objdetect/aruco_detector.hpp"
#include "opencv2/aruco/aruco_calib.hpp"

namespace cv {
namespace aruco {

/**
@deprecated Use class ArucoDetector::detectMarkers
*/
CV_EXPORTS_W void detectMarkers(InputArray image, const Ptr<Dictionary> &dictionary, OutputArrayOfArrays corners,
                                OutputArray ids, const Ptr<DetectorParameters> &parameters = makePtr<DetectorParameters>(),
                                OutputArrayOfArrays rejectedImgPoints = noArray());

/**
@deprecated Use class ArucoDetector::refineDetectedMarkers
*/
CV_EXPORTS_W void refineDetectedMarkers(InputArray image,const  Ptr<Board> &board,
                                        InputOutputArrayOfArrays detectedCorners,
                                        InputOutputArray detectedIds, InputOutputArrayOfArrays rejectedCorners,
                                        InputArray cameraMatrix = noArray(), InputArray distCoeffs = noArray(),
                                        float minRepDistance = 10.f, float errorCorrectionRate = 3.f,
                                        bool checkAllOrders = true, OutputArray recoveredIdxs = noArray(),
                                        const Ptr<DetectorParameters> &parameters = makePtr<DetectorParameters>());

/**
@deprecated Use Board::draw
*/
CV_EXPORTS_W void drawPlanarBoard(const Ptr<Board> &board, Size outSize, OutputArray img, int marginSize,
                                  int borderBits);

/**
@deprecated Use Board::matchImagePoints
*/
CV_EXPORTS_W void getBoardObjectAndImagePoints(const Ptr<Board> &board, InputArrayOfArrays detectedCorners,
                                               InputArray detectedIds, OutputArray objPoints, OutputArray imgPoints);


/**
 * @brief Pose estimation for a board of markers
 *
 * @param corners vector of already detected markers corners. For each marker, its four corners
 * are provided, (e.g std::vector<std::vector<cv::Point2f> > ). For N detected markers, the
 * dimensions of this array should be Nx4. The order of the corners should be clockwise.
 * @param ids list of identifiers for each marker in corners
 * @param board layout of markers in the board. The layout is composed by the marker identifiers
 * and the positions of each marker corner in the board reference system.
 * @param cameraMatrix input 3x3 floating-point camera matrix
 * \f$A = \vecthreethree{f_x}{0}{c_x}{0}{f_y}{c_y}{0}{0}{1}\f$
 * @param distCoeffs vector of distortion coefficients
 * \f$(k_1, k_2, p_1, p_2[, k_3[, k_4, k_5, k_6],[s_1, s_2, s_3, s_4]])\f$ of 4, 5, 8 or 12 elements
 * @param rvec Output vector (e.g. cv::Mat) corresponding to the rotation vector of the board
 * (see cv::Rodrigues). Used as initial guess if not empty.
 * @param tvec Output vector (e.g. cv::Mat) corresponding to the translation vector of the board.
 * @param useExtrinsicGuess defines whether initial guess for \b rvec and \b tvec will be used or not.
 * Used as initial guess if not empty.
 *
 * This function receives the detected markers and returns the pose of a marker board composed
 * by those markers.
 * A Board of marker has a single world coordinate system which is defined by the board layout.
 * The returned transformation is the one that transforms points from the board coordinate system
 * to the camera coordinate system.
 * Input markers that are not included in the board layout are ignored.
 * The function returns the number of markers from the input employed for the board pose estimation.
 * Note that returning a 0 means the pose has not been estimated.
 * @sa use cv::drawFrameAxes to get world coordinate system axis for object points
 */
CV_EXPORTS_W int estimatePoseBoard(InputArrayOfArrays corners, InputArray ids, const Ptr<Board> &board,
                                   InputArray cameraMatrix, InputArray distCoeffs, InputOutputArray rvec,
                                   InputOutputArray tvec, bool useExtrinsicGuess = false);

/**
 * @brief Pose estimation for a ChArUco board given some of their corners
 * @param charucoCorners vector of detected charuco corners
 * @param charucoIds list of identifiers for each corner in charucoCorners
 * @param board layout of ChArUco board.
 * @param cameraMatrix input 3x3 floating-point camera matrix
 * \f$A = \vecthreethree{f_x}{0}{c_x}{0}{f_y}{c_y}{0}{0}{1}\f$
 * @param distCoeffs vector of distortion coefficients
 * \f$(k_1, k_2, p_1, p_2[, k_3[, k_4, k_5, k_6],[s_1, s_2, s_3, s_4]])\f$ of 4, 5, 8 or 12 elements
 * @param rvec Output vector (e.g. cv::Mat) corresponding to the rotation vector of the board
 * (see cv::Rodrigues).
 * @param tvec Output vector (e.g. cv::Mat) corresponding to the translation vector of the board.
 * @param useExtrinsicGuess defines whether initial guess for \b rvec and \b tvec will be used or not.
 *
 * This function estimates a Charuco board pose from some detected corners.
 * The function checks if the input corners are enough and valid to perform pose estimation.
 * If pose estimation is valid, returns true, else returns false.
 * @sa use cv::drawFrameAxes to get world coordinate system axis for object points
 */
CV_EXPORTS_W bool estimatePoseCharucoBoard(InputArray charucoCorners, InputArray charucoIds,
                                           const Ptr<CharucoBoard> &board, InputArray cameraMatrix,
                                           InputArray distCoeffs, InputOutputArray rvec,
                                           InputOutputArray tvec, bool useExtrinsicGuess = false);

/**
 * @brief Pose estimation for single markers
 *
 * @param corners vector of already detected markers corners. For each marker, its four corners
 * are provided, (e.g std::vector<std::vector<cv::Point2f> > ). For N detected markers,
 * the dimensions of this array should be Nx4. The order of the corners should be clockwise.
 * @sa detectMarkers
 * @param markerLength the length of the markers' side. The returning translation vectors will
 * be in the same unit. Normally, unit is meters.
 * @param cameraMatrix input 3x3 floating-point camera matrix
 * \f$A = \vecthreethree{f_x}{0}{c_x}{0}{f_y}{c_y}{0}{0}{1}\f$
 * @param distCoeffs vector of distortion coefficients
 * \f$(k_1, k_2, p_1, p_2[, k_3[, k_4, k_5, k_6],[s_1, s_2, s_3, s_4]])\f$ of 4, 5, 8 or 12 elements
 * @param rvecs array of output rotation vectors (@sa Rodrigues) (e.g. std::vector<cv::Vec3d>).
 * Each element in rvecs corresponds to the specific marker in imgPoints.
 * @param tvecs array of output translation vectors (e.g. std::vector<cv::Vec3d>).
 * Each element in tvecs corresponds to the specific marker in imgPoints.
 * @param objPoints array of object points of all the marker corners
 * @param estimateParameters set the origin of coordinate system and the coordinates of the four corners of the marker
 * (default estimateParameters.pattern = PatternPositionType::ARUCO_CCW_CENTER, estimateParameters.useExtrinsicGuess = false,
 * estimateParameters.solvePnPMethod = SOLVEPNP_ITERATIVE).
 *
 * This function receives the detected markers and returns their pose estimation respect to
 * the camera individually. So for each marker, one rotation and translation vector is returned.
 * The returned transformation is the one that transforms points from each marker coordinate system
 * to the camera coordinate system.
 * The marker coordinate system is centered on the middle (by default) or on the top-left corner of the marker,
 * with the Z axis perpendicular to the marker plane.
 * estimateParameters defines the coordinates of the four corners of the marker in its own coordinate system (by default) are:
 * (-markerLength/2, markerLength/2, 0), (markerLength/2, markerLength/2, 0),
 * (markerLength/2, -markerLength/2, 0), (-markerLength/2, -markerLength/2, 0)
 * @sa use cv::drawFrameAxes to get world coordinate system axis for object points
 * @sa @ref tutorial_aruco_detection
 * @sa EstimateParameters
 * @sa PatternPositionType
 */
CV_EXPORTS_W void estimatePoseSingleMarkers(InputArrayOfArrays corners, float markerLength,
                                            InputArray cameraMatrix, InputArray distCoeffs,
                                            OutputArray rvecs, OutputArray tvecs, OutputArray objPoints = noArray(),
                                            const Ptr<EstimateParameters>& estimateParameters = makePtr<EstimateParameters>());


/** @deprecated Use CharucoBoard::checkCharucoCornersCollinear
 */
CV_EXPORTS_W bool testCharucoCornersCollinear(const Ptr<CharucoBoard> &board, InputArray charucoIds);

}
}

#endif
