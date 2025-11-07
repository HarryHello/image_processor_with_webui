// state.js
// 用于不同 js 文件之间共享状态

// 当前选中的文件
export let currentFile = null;

// 设置当前文件的函数
export function setCurrentFile(file) {
    console.log('设置当前文件', file);
    currentFile = file;
}

// 保存处理后的图像数据
export let processedImageData = null;

// 设置处理后图像数据的函数
export function setProcessedImageData(data) {
    processedImageData = data;
}
    
// 存储颜色操作图层
export let colorLayers = [];
export let dragSrcElement = null;

export const els = {
    // 上传区域相关元素
    fileInput: null,
    uploadArea: null,
    imagePreview: null,
    previewSection: null,
    operationsSection: null,
    resultSection: null,

    // 结果图像相关元素
    resultImage: null,
    imageDimensions: null,
    
    // 标签页相关元素
    tabButtons: null,
    tabContents: null,
    
    // 缩放相关元素
    scaleOption: null,
    scaleOptionSelect: null,
    dimensionsGroup: null,
    factorGroup: null,
    interpolationGroup: null,
    
    // 颜色处理相关元素
    colorOption: null,
    colorOptionSelect: null,
    addColorOperationButton: null,
    colorLayersContainer: null,
    
    // 参数控制元素
    grayscaleParams: null,
    brightnessParams: null,
    contrastParams: null,
    saturationParams: null,
    
    // 灰度化参数
    grayscaleTypeRadios: null,
    customGrayscaleCoefficients: null,
    redCoefficient: null,
    greenCoefficient: null,
    blueCoefficient: null,
    redCoefficientValue: null,
    greenCoefficientValue: null,
    blueCoefficientValue: null,
    
    // 亮度参数
    brightnessValue: null,
    brightnessValueDisplay: null,
    
    // 对比度参数
    contrastValue: null,
    contrastValueDisplay: null,
    
    // 饱和度参数
    saturationValue: null,
    saturationValueDisplay: null,
    
    // 按钮元素
    processButton: null,
    downloadButton: null,
}