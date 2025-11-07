// process.js
// 图像处理功能

import { els, currentFile, processedImageData, setProcessedImageData, colorLayers } from './state.js';

export function initProcessEvents() {
    // 处理图像按钮事件
    els.processButton.addEventListener('click', processImage);

    // 处理图像功能
    function processImage() {
        console.log('开始处理图像');

        if (!currentFile) {
            alert('请先选择一个图像文件!');
            return;
        }
        
        // 显示处理中的提示
        els.processButton.textContent = '处理中...';
        els.processButton.disabled = true;
        
        // 创建 FormData 对象发送文件
        const formData = new FormData();
        formData.append('image', currentFile);
        console.log('在请求中加入 ' + currentFile);
        
        
        // 添加操作参数
        const params = {};
        
        // 处理颜色操作
        if (colorLayers.length > 0) {
            // 先更新所有图层的参数
            colorLayers.forEach(layer => {
                updateLayerParams(layer.id);
            });
            
            // 然后构建colorOps参数
            params.colorOps = colorLayers.map(layer => {
                console.log("处理图层", layer);
                return {
                    type: layer.type,
                    params: layer.params || {}  // 确保即使params为undefined也传递空对象
                };
            });
            
            // 添加调试日志，确认颜色操作参数已设置
            console.log("设置的颜色操作参数:", params.colorOps);
        } else {
            console.log("未设置颜色操作");
        }

        // 处理缩放操作
        if (els.scaleOption && els.scaleOption.value != 'none') {
            const scaleType = els.scaleOption.value;
            console.log("scaleType", scaleType);
            
            if (scaleType === 'factor') {
                const factor = parseFloat(document.getElementById('scaleFactor').value);
                if (!isNaN(factor) && factor > 0) {
                    params.scaleOp = {
                        type: 'factor',
                        factor: factor
                    };
                }
            } else if (scaleType === 'dimensions') {
                const width = parseInt(document.getElementById('newWidth').value); // 修正ID匹配原始代码
                const height = parseInt(document.getElementById('newHeight').value); // 修正ID匹配原始代码
                if (!isNaN(width) && width > 0 && !isNaN(height) && height > 0) {
                    params.scaleOp = {
                        type: 'dimensions',
                        width: width,
                        height: height
                    };
                }
            } else if (scaleType === 'enlarge') {
                params.scaleOp = {
                    type: 'enlarge'
                };
            } else if (scaleType === 'shrink') {
                params.scaleOp = {
                    type: 'shrink'
                };
            }
            
            // 添加调试日志，确认缩放参数已设置
            console.log("设置的缩放参数:", params.scaleOp);
            
            // 插值方法处理保持不变
            params.method = document.getElementById('interpolationMethod').value;
            console.log('缩放请求插值方法为 ' + params.method);
        } else {
            console.log("未设置缩放选项");
        }
        
        // 添加调试日志，确认发送的参数
        console.log("发送到后端的完整参数:", params);
        console.log("JSON序列化后的参数:", JSON.stringify(params));
        
        // 将参数添加到FormData
        formData.append('params', JSON.stringify(params));
        
        // 发送请求到后端
        fetch('/api/process', {
            method: 'POST',
            body: formData
        })
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.json();
        })
        .then(data => {
            if (data.success) {
                // 显示处理结果
                setProcessedImageData('data:image/png;base64,' + data.data);
                els.resultImage.src = processedImageData;
                
                // 创建临时图像对象以获取处理后图像的尺寸
                const img = new Image();
                img.src = processedImageData;
                
                els.resultSection.style.display = 'block';
                
                // 滚动到结果区域
                els.resultSection.scrollIntoView({ behavior: 'smooth' });
            } else {
                alert('处理失败: ' + data.message);
            }
        })
        .catch(error => {
            console.error('处理图像时出错:', error);
            alert('处理图像时出错，请重试');
        })
        .finally(() => {
            // 恢复按钮状态
            els.processButton.textContent = '处理图像';
            els.processButton.disabled = false;
        });
    } // function ProcessImage END

    // 更新图层参数的函数
    function updateLayerParams(layerId) {
        console.log(`更新 id 为 ${layerId} 的颜色图层`)
        const layer = colorLayers.find(l => l.id === layerId);

        if (!layer) {
            console.error('没有找到对应 layer');
            return;
        }

        console.log(`获取到的 layer type 参数为 ${layer.type}`);
        const layer_operation = layer.type;

        // 根据图层类型更新参数
        if (layer_operation === 'grayscale') {
            // 添加灰度参数的更新逻辑
            const layerElement = document.querySelector(`[data-layer-id="${layerId}"]`);
            if (layerElement) {
                const typeSelect = layerElement.querySelector('.grayscale-type-select');
                if (typeSelect) {
                    const type = typeSelect.value;
                    layer.params = { type };
                    console.log(`id 为 ${layerId} 的 grayscale 图层类型为 ${type}`);
                    if (type === 'custom') {
                        const redInput = layerElement.querySelector('.red-coefficient-input');
                        const greenInput = layerElement.querySelector('.green-coefficient-input');
                        const blueInput = layerElement.querySelector('.blue-coefficient-input');
                        
                        if (redInput && greenInput && blueInput) {
                            layer.params.r = parseFloat(redInput.value);
                            layer.params.g = parseFloat(greenInput.value);
                            layer.params.b = parseFloat(blueInput.value);
                            console.log(`参数为 r: ${layer.params.r}, g: ${layer.params.g}, b: ${layer.params.b}`);
                        } else {
                            console.error(`未找到 id 为 ${layerId} 的自定义灰度系数输入框`);
                        }
                    }
                } else {
                    console.error(`未找到 id 为 ${layerId} 的 grayscale 类型选择器`);
                }
            } else {
                console.error(`未找到 id 为 ${layerId} 的图层元素`);
            }
        } else if (layer_operation === 'brightness') {
            // 添加亮度参数的更新逻辑
            const layerElement = document.querySelector(`[data-layer-id="${layerId}"]`);
            if (layerElement) {
                const slider = layerElement.querySelector('.brightness-slider');
                if (slider) {
                    layer.params = {
                        value: parseInt(slider.value)
                    };
                    console.log(`id 为 ${layerId} 的 brightness 图层参数为 ${layer.params.value}`)
                } else {
                    console.error(`未找到 id 为 ${layerId} 的 brightness 滑块元素`);
                }
            } else {
                console.error(`未找到 id 为 ${layerId} 的图层元素`);
            }
        } else if (layer_operation === 'contrast') {
            // 添加对比度参数的更新逻辑
            const layerElement = document.querySelector(`[data-layer-id="${layerId}"]`);
            if (layerElement) {
                const slider = layerElement.querySelector('.contrast-slider');
                if (slider) {
                    layer.params = {
                        value: parseInt(slider.value)
                    };
                    console.log(`id 为 ${layerId} 的 contrast 图层参数为 ${layer.params.value}`)
                } else {
                    console.error(`未找到 id 为 ${layerId} 的 contrast 滑块元素`);
                }
            } else {
                console.error(`未找到 id 为 ${layerId} 的图层元素`);
            }
        } else if (layer_operation === 'saturation') {
            // 添加饱和度参数的更新逻辑
            const layerElement = document.querySelector(`[data-layer-id="${layerId}"]`);
            if (layerElement) {
                const slider = layerElement.querySelector('.saturation-slider');
                if (slider) {
                    layer.params = {
                        value: parseInt(slider.value)
                    };
                    console.log(`id 为 ${layerId} 的 saturation 图层参数为 ${layer.params.value}`)
                } else {
                    console.error(`未找到 id 为 ${layerId} 的 saturation 滑块元素`);
                }
            } else {
                console.error(`未找到 id 为 ${layerId} 的图层元素`);
            }
        } else if (layer_operation === 'invert') {
            // 反色操作不需要额外参数
            layer.params = {};
            console.log(`id 为 ${layerId} 的 invert 图层不需要额外参数`);
        }
    } // function updateLayerParams END

} // export function initProcessEvent END