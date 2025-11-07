// color.js
// 处理颜色调整功能

// 基于类似 photoshop 的图层逻辑实现颜色操作
// 用 sortableJS 库实现图层拖拽排序
// 每个图层都只有一行，格式如下
// // ≡ 是拖拽手柄
// // × 是删除图层按钮
// [≡ 灰度化 [默认(这是一个下拉选项卡)]  ×]
// [≡ 灰度化 [自定义(这是一个下拉选项卡)] R:$可填写的系数$ G:$可填写的系数$ B:$可填写的系数$  ×]
// [≡ 亮度 [滑块(-100-100)] $滑块对应的数字(可双击手动填写)$  ×]
// [≡ 对比度 [滑块(-100-100)] $滑块对应的数字(可双击手动填写)$  ×]
// [≡ 饱和度 [滑块(-100-100)] $滑块对应的数字(可双击手动填写)$  ×]

import { els, colorLayers, dragSrcElement } from './state.js';


export function initColorEvents() {
    console.log('初始化颜色操作事件');

    // 创建一个统一的函数来更新按钮状态
    const updateButtonState = () => {
        const isDisabled = !els.colorOption.value;
        els.addColorOperationButton.disabled = isDisabled;
        console.log('更新按钮状态:', isDisabled);
    };

    // 监听原生select的change事件
    els.colorOptionSelect.passedElement.element.addEventListener('change', (e) => {
        console.log('Choices change事件触发，值为:', e.detail.value);
        updateButtonState();
    });
    
    // 灰度化类型切换
    els.grayscaleTypeRadios.forEach(radio => {
        radio.addEventListener('change', () => {
            if (radio.value === 'custom') {
                els.customGrayscaleCoefficients.style.display = 'block';
            } else {
                els.customGrayscaleCoefficients.style.display = 'none';
            }
        });
    });
    
    // 添加颜色操作图层
    els.addColorOperationButton.addEventListener('click', () => {
        const operation = els.colorOption.value;
        if (operation === '') return;
        
        const layer = createColorLayer(operation);
        if (layer) {
            colorLayers.push(layer);
            renderColorLayers();
        }
    });
  
    // 初始化时设置按钮状态
    updateButtonState();
}


// 创建颜色操作图层
export function createColorLayer(operation) {
    const layer = {
        id: Date.now(),
        type: operation,
        params: {}
    };
    
    switch (operation) {
        case 'grayscale':
            layer.params.type = 'default';
            break;
        case 'brightness':
            layer.params.value = 0;
            break;
        case 'contrast':
            layer.params.value = 0;
            break;
        case 'saturation':
            layer.params.value = 0;
            break;
        case 'invert':
            // 反色没有参数
            break;
    }
    
    return layer;
}

// 渲染颜色操作图层
export function renderColorLayers() {
    if (!els.colorLayersContainer) return;
    
    // 清空容器
    els.colorLayersContainer.innerHTML = '';
    
    // 为每个图层创建DOM元素
    colorLayers.forEach((layer, index) => {
        const layerElement = document.createElement('div');
        layerElement.className = 'color-layer';
        layerElement.dataset.layerId = layer.id;
        
        // 创建拖拽手柄
        const handle = document.createElement('span');
        handle.className = 'layer-handle';
        handle.textContent = '≡';
        layerElement.appendChild(handle);
        
        // 创建图层内容
        const content = document.createElement('div');
        content.className = 'layer-content';
        
        // 根据图层类型创建不同的内容
        switch (layer.type) {
            case 'grayscale':
                content.innerHTML = createGrayscaleLayerContent(layer);
                break;
            case 'brightness':
            case 'contrast':
            case 'saturation':
                content.innerHTML = createSliderLayerContent(layer);
                break;
            case 'invert':
                content.innerHTML = `<span class="layer-label">反色</span>`;
                break;
        }
        
        layerElement.appendChild(content);
        
        // 创建删除按钮
        const deleteButton = document.createElement('button');
        deleteButton.className = 'layer-delete';
        deleteButton.textContent = '×';
        deleteButton.addEventListener('click', () => {
            removeColorLayer(layer.id);
        });
        layerElement.appendChild(deleteButton);
        
        // 将图层添加到容器
        els.colorLayersContainer.appendChild(layerElement);
        
        // 添加滑块事件处理
        if (['brightness', 'contrast', 'saturation'].includes(layer.type)) {
            const slider = layerElement.querySelector(`.${layer.type}-slider`);
            const valueDisplay = layerElement.querySelector(`.${layer.type}-value`);
            
            if (slider && valueDisplay) {
                slider.addEventListener('input', () => {
                    valueDisplay.textContent = slider.value;
                    updateLayerParam(layer.id, 'value', parseInt(slider.value));
                });
                
                // 双击值显示可编辑
                valueDisplay.addEventListener('dblclick', () => {
                    const input = document.createElement('input');
                    input.type = 'number';
                    input.value = valueDisplay.textContent;
                    input.min = -100;
                    input.max = 100;
                    input.className = 'layer-value-input';
                    
                    valueDisplay.replaceWith(input);
                    input.focus();
                    
                    const finishEditing = () => {
                        let newValue = parseInt(input.value) || 0;
                        newValue = Math.max(-100, Math.min(100, newValue));
                        
                        slider.value = newValue;
                        const newDisplay = document.createElement('span');
                        newDisplay.className = `${layer.type}-value`;
                        newDisplay.textContent = newValue;
                        
                        input.replaceWith(newDisplay);
                        updateLayerParam(layer.id, 'value', newValue);
                    };
                    
                    input.addEventListener('blur', finishEditing);
                    input.addEventListener('keydown', (e) => {
                        if (e.key === 'Enter') {
                            finishEditing();
                        }
                    });
                });
            }
        }
        
        // 添加灰度化类型切换事件
        if (layer.type === 'grayscale') {
            const typeSelect = layerElement.querySelector('.grayscale-type-select');
            const customCoefficients = layerElement.querySelector('.custom-coefficients');
            const redInput = layerElement.querySelector('.red-coefficient-input');
            const greenInput = layerElement.querySelector('.green-coefficient-input');
            const blueInput = layerElement.querySelector('.blue-coefficient-input');
            
            if (typeSelect && customCoefficients) {
                typeSelect.addEventListener('change', () => {
                    if (typeSelect.value === 'custom') {
                        customCoefficients.style.display = 'inline-block';
                    } else {
                        customCoefficients.style.display = 'none';
                    }
                    updateLayerParam(layer.id, 'type', typeSelect.value);
                });
            }
            
            if (redInput && greenInput && blueInput) {
                redInput.addEventListener('change', () => {
                    updateLayerParam(layer.id, 'red', parseFloat(redInput.value) || 0);
                });
                
                greenInput.addEventListener('change', () => {
                    updateLayerParam(layer.id, 'green', parseFloat(greenInput.value) || 0);
                });
                
                blueInput.addEventListener('change', () => {
                    updateLayerParam(layer.id, 'blue', parseFloat(blueInput.value) || 0);
                });
            }
        }
    });
    
    // 初始化拖拽排序
    initLayerSorting();
}

// 创建灰度化图层内容
function createGrayscaleLayerContent(layer) {
    const isCustom = layer.params.type === 'custom';
    const red = layer.params.red || 0.299;
    const green = layer.params.green || 0.587;
    const blue = layer.params.blue || 0.114;
    
    return `
        <span class="layer-label">灰度化</span>
        <select class="grayscale-type-select">
            <option value="default" ${!isCustom ? 'selected' : ''}>默认</option>
            <option value="custom" ${isCustom ? 'selected' : ''}>自定义</option>
        </select>
        <div class="custom-gray-coefficients" style="display: ${isCustom ? 'inline-block' : 'none'};">
            R:<input type="number" class="red-coefficient-input" step="0.001" min="0" max="1" value="${red}"> 
            G:<input type="number" class="green-coefficient-input" step="0.001" min="0" max="1" value="${green}"> 
            B:<input type="number" class="blue-coefficient-input" step="0.001" min="0" max="1" value="${blue}">
        </div>
    `;
}

// 创建带滑块的图层内容
function createSliderLayerContent(layer) {
    const typeLabels = {
        brightness: '亮度&#x3000;',
        contrast: '对比度',
        saturation: '饱和度'
    };
    
    const value = layer.params.value || 0;
    
    return `
        <span class="layer-label">${typeLabels[layer.type]}
            <input type="range" class="${layer.type}-slider" min="-100" max="100" value="${value}">
            <span class="${layer.type}-value">${value}</span>
        </span>
    `;
}

// 移除颜色操作图层
export function removeColorLayer(layerId) {
    const index = colorLayers.findIndex(layer => layer.id === layerId);
    if (index !== -1) {
        colorLayers.splice(index, 1);
        renderColorLayers();
    }
}

// 更新图层参数
export function updateLayerParam(layerId, paramName, value) {
    const layer = colorLayers.find(layer => layer.id === layerId);
    if (layer) {
        layer.params[paramName] = value;
    }
}

// 初始化图层拖拽排序
function initLayerSorting() {
    if (!els.colorLayersContainer) return;
    
    new Sortable(els.colorLayersContainer, {
        handle: '.layer-handle',
        animation: 150,
        ghostClass: 'layer-ghost',
        onEnd: (evt) => {
            // 重新排序colorLayers数组
            const newOrder = Array.from(els.colorLayersContainer.children).map(el => 
                parseInt(el.dataset.layerId)
            );
            
            const reorderedLayers = newOrder.map(id => 
                colorLayers.find(layer => layer.id === id)
            );
            
            colorLayers.length = 0;
            colorLayers.push(...reorderedLayers);
        }
    });
}
