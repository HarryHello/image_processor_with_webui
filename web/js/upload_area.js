// upload_area.js
// 处理上传区域的事件

import { els, setCurrentFile } from './state.js';

export function initUploadAreaEvents() {
    console.log('初始化上传区域事件');
    // 点击上传区域触发文件选择
    els.uploadArea.addEventListener('click', () => {
        els.fileInput.click();
    });
    
    // 拖拽上传功能
     els.uploadArea.addEventListener('dragover', (e) => {
        e.preventDefault();
        els.uploadArea.style.borderColor = '#0078d4';
        els.uploadArea.style.backgroundColor = 'rgba(0, 120, 212, 0.15)';
    });
    
    els.uploadArea.addEventListener('dragleave', () => {
        els.uploadArea.style.borderColor = '#e1e1e1';
        els.uploadArea.style.backgroundColor = 'rgba(0, 120, 212, 0.05)';
    });
    
    els.uploadArea.addEventListener('drop', (e) => {
        e.preventDefault();
        els.uploadArea.style.borderColor = '#e1e1e1';
        els.uploadArea.style.backgroundColor = 'rgba(0, 120, 212, 0.05)';
        
        if (e.dataTransfer.files.length) {
            handleFileSelect(e.dataTransfer.files[0]);
        }
    });
    
    // 文件选择事件
    els.fileInput.addEventListener('change', (e) => {
        console.log('文件选择事件触发');
        if (e.target.files.length) {
            handleFileSelect(e.target.files[0]);
        }
    });

    // 处理文件选择
    function handleFileSelect(file) {
        console.log('处理文件选择', file);
        // 检查文件类型
        const validTypes = ['image/jpeg', 'image/png', 'image/ppm'];
        if (!validTypes.includes(file.type) && !file.name.toLowerCase().endsWith('.ppm')) {
            alert('请选择有效的图像文件 (JPG, PNG, PPM)');
            return;
        }
        
        // 使用函数设置当前文件
        setCurrentFile(file);
        
        // 读取并显示图像
        const reader = new FileReader();
        reader.onload = function(e) {
            els.imagePreview.src = e.target.result;
            
            // 创建临时图像对象以获取尺寸
            const img = new Image();
            img.onload = function() {
                // 显示图像尺寸
                els.imageDimensions.textContent = `${this.naturalWidth}×${this.naturalHeight}`;
            };
            img.src = e.target.result;
            
            els.previewSection.style.display = 'block';
            els.operationsSection.style.display = 'block';
            els.resultSection.style.display = 'none';
        };
        reader.readAsDataURL(file);
    }
}