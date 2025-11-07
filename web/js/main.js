// main.js

import { els } from './state.js';
import { initUploadAreaEvents } from './upload_area.js';
import { initScaleEvents } from './scale.js';
import { initColorEvents } from './color.js';
import { initProcessEvents } from './process.js';
import { initDownloadEvents } from './download.js';
import { initScrollControl } from './scroll_control.js';


document.addEventListener('DOMContentLoaded', () => {
    console.log('DOM内容已加载，初始化元素和事件');
    // 初始化DOM元素引用
    Object.assign(els, {
        uploadArea: document.getElementById('uploadArea'),
        fileInput: document.getElementById('fileInput'),
        imagePreview: document.getElementById('imagePreview'),
        previewSection: document.getElementById('previewSection'),
        operationsSection: document.getElementById('operationsSection'),
        resultSection: document.getElementById('resultSection'),
        resultImage: document.getElementById('resultImage'),
        imageDimensions: document.getElementById('imageDimensions'), // 新增：用于显示图像尺寸
        
        // 标签页相关元素
        tabButtons: document.querySelectorAll('.tab-button'),
        tabContents: document.querySelectorAll('.tab-content'),
        
        // 缩放相关元素
        scaleOption: document.getElementById('scaleOption'),
        dimensionsGroup: document.getElementById('dimensionsGroup'),
        factorGroup: document.getElementById('factorGroup'),
        interpolationGroup: document.getElementById('interpolationGroup'),
        
        // 颜色处理相关元素
        colorOption: document.getElementById('colorOption'),
        addColorOperationButton: document.getElementById('addColorOperationButton'),
        colorLayersContainer: document.getElementById('colorLayersContainer'),
        
        // 参数控制元素
        grayscaleParams: document.getElementById('grayscaleParams'),
        brightnessParams: document.getElementById('brightnessParams'),
        contrastParams: document.getElementById('contrastParams'),
        saturationParams: document.getElementById('saturationParams'),
        
        // 灰度化参数
        grayscaleTypeRadios: document.querySelectorAll('input[name="grayscaleType"]'),
        customGrayscaleCoefficients: document.getElementById('customGrayscaleCoefficients'),
        redCoefficient: document.getElementById('redCoefficient'),
        greenCoefficient: document.getElementById('greenCoefficient'),
        blueCoefficient: document.getElementById('blueCoefficient'),
        redCoefficientValue: document.getElementById('redCoefficientValue'),
        greenCoefficientValue: document.getElementById('greenCoefficientValue'),
        blueCoefficientValue: document.getElementById('blueCoefficientValue'),
        
        // 亮度参数
        brightnessValue: document.getElementById('brightnessValue'),
        brightnessValueDisplay: document.getElementById('brightnessValueDisplay'),
        
        // 对比度参数
        contrastValue: document.getElementById('contrastValue'),
        contrastValueDisplay: document.getElementById('contrastValueDisplay'),
        
        // 饱和度参数
        saturationValue: document.getElementById('saturationValue'),
        saturationValueDisplay: document.getElementById('saturationValueDisplay'),
        
        // 按钮元素
        processButton: document.getElementById('processButton'),
        downloadButton: document.getElementById('downloadButton'),
    });

    console.log('初始化颜色操作选项选择器');
    // 颜色操作选项使用 Choices.js
    els.colorOptionSelect = new Choices(els.colorOption, {
        searchEnabled: false,
        itemSelectText: '',
        shouldSort: false,
        placeholder: true,
        placeholderValue: '选择操作',
        allowHTML: true
    });

    console.log('初始化缩放选择器');
    // 缩放选项使用 Choices.js
    els.scaleOptionSelect = new Choices(els.scaleOption, {
        searchEnabled: false,        // 禁用搜索（你不需要）
        itemSelectText: '',          // 点击选项时不显示“选中”提示文字
        shouldSort: false,           // 保持选项顺序不变
        placeholder: true,           // 如果你有 placeholder（你没有，可忽略）
        allowHTML: true              // 允许 HTML（但我们没用）
    });

    console.log('初始化插值算法选择器');
    // 插值算法选项使用 Choices.js
    els.interpolationSelect = new Choices(els.interpolationMethod, {
        searchEnabled: false,        // 禁用搜索（你不需要）
        itemSelectText: '',          // 点击选项时不显示“选中”提示文字
        shouldSort: false,           // 保持选项顺序不变
        placeholder: true,           // 如果你有 placeholder（你没有，可忽略）
        allowHTML: true              // 允许 HTML（但我们没用）
    });

    initUploadAreaEvents();
    initScaleEvents();
    initColorEvents();
    initProcessEvents();
    initDownloadEvents();
    initScrollControl();
});