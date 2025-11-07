// download.js
// 处理图像下载功能

import { els, processedImageData } from './state.js';

export function initDownloadEvents() {
    // 下载按钮事件
    els.downloadButton.addEventListener('click', () => {
        if (processedImageData) {
            const link = document.createElement('a');
            link.href = processedImageData;
            link.download = 'processed_image.png';
            document.body.appendChild(link);
            link.click();
            document.body.removeChild(link);
        }
    });
}