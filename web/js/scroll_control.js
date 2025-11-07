// scroll_control.js
// 控制输入框滚动行为，防止意外滚动修改数值

export function initScrollControl() {
    console.log('初始化滚动控制');
    let inputHovered = false;
    
    // 鼠标进入/离开输入框时更新状态
    document.body.addEventListener('mouseover', e => {
        const isInput = e.target.matches('input[type="number"], input[type="range"], textarea, select');
        inputHovered = isInput;
    });
    
    document.body.addEventListener('mouseout', e => {
        const isInput = e.target.matches('input[type="number"], input[type="range"], textarea, select');
        if (isInput) {
            inputHovered = false;
        }
    });
    
    // 全局wheel事件处理
    document.body.addEventListener('wheel', e => {
        if (inputHovered && !e.shiftKey) { // 按住Shift允许滚动
            e.preventDefault();
        }
    }, { passive: false });
    
    // 防止数字输入框的默认滚动行为
    document.querySelectorAll('input[type="number"]').forEach(input => {
        input.addEventListener('wheel', e => {
            e.preventDefault();
        }, { passive: false });
    });
}