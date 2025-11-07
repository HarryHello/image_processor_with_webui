// scale.js
// 处理缩放功能

import { els } from './state.js';

export function initScaleEvents() {

    setTimeout(() => {
    const select = document.getElementById('interpolationMethod');
    const optionElements = select.querySelectorAll('option');
    const choiceItems = document.querySelectorAll('.choices__list--dropdown .choices__item--choice');
    choiceItems.forEach((item, index) => {
        const title = optionElements[index]?.title;
        if (title) {
        item.setAttribute('title', title);
        }
    });
    }, 100);
    
    // 缩放选项切换
     els.scaleOptionSelect.passedElement.element.addEventListener('change', () => {
        // 隐藏所有缩放相关控件
        els.dimensionsGroup.style.display = 'none';
        els.factorGroup.style.display = 'none';
        
        // 控制插值方法的显示/隐藏
        // 如果选择"不缩放"，隐藏插值方法选项
        if (scaleOption.value === 'none') {
            els.interpolationGroup.style.display = 'none';
        } else {
            els.interpolationGroup.style.display = 'block';
        }
        
        // 根据选择显示相应的控件
        switch(scaleOption.value) {
            case 'dimensions':
                els.dimensionsGroup.style.display = 'block';
                break;
            case 'factor':
                els.factorGroup.style.display = 'block';
                break;
        }
    });

    // 初始化时触发一次change事件，确保正确显示选项
    els.scaleOptionSelect.passedElement.element.dispatchEvent(new Event('change'));
}