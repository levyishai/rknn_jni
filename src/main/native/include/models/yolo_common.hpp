#pragma once

#include "postprocess_v5.h"
#include "opencv2/core/core.hpp"
#include "rknn_api.h"
#include "image_utils.h"
#include <vector>

typedef struct {
    double nms_thresh;
    double box_thresh;
} DetectionFilterParams;

enum ModelVersion {
    YOLO_V5,
    YOLO_V8
};

class YoloModel {
public:
    YoloModel(std::string modelPath, int num_classes_, ModelVersion type_, int coreNumber);

    detect_result_group_t forward(cv::Mat &orig_img, DetectionFilterParams params);

    ~YoloModel();

protected:
    virtual detect_result_group_t postProcess(std::vector<rknn_output> output,
        DetectionFilterParams params, 
        cv::Size inputImageSize,
        cv::Size2d inputImageScale,
        letterbox_t letterbox
    ) = 0;

    int numClasses;
    ModelVersion yoloType;

    // todo matt do we need to keep this model data pointer around? seems like no?
    unsigned char *model_data;
    rknn_context ctx;
    cv::Size modelSize;
    int channels;

    rknn_input_output_num io_num;
    std::vector<rknn_tensor_attr> input_attrs;
    std::vector<rknn_tensor_attr> output_attrs;

    rknn_input inputs[1];
};

class YoloV5Model : public YoloModel {
    using YoloModel::YoloModel;

    detect_result_group_t postProcess(std::vector<rknn_output> output,
        DetectionFilterParams params, 
        cv::Size inputImageSize,
        cv::Size2d inputImageScale,
        letterbox_t letterbox
    );
};

