## v2.0.0

- 升级pugixml库，保持可独立更新
- 统一布局xml元素命名风格，目前看来有些大写，有些小写，早期dm对pugixml库做了特殊修改忽略大小写
- 更换布局元素Static为label
- 优化GetText/SetText，使之更易用，作为虚接口可作用于label, button, edit, richedit控件
- 优化编码转换接口，统一为ntcvt实现，避免不必要的内存拷贝
- C++11至C++23编译支持
- 优化BSTR使用，修复内存泄露
- UTF-8: 解析；UTF-16: 文本渲染、路径处理、字体创建
- 支持通过DM_EXCLUDE_XXX宏裁剪ActiveX等组件
- 优化头文件包含，用户不再需要添加一堆DM头文件搜索路径，更方便集成到已有软件系统
- DPI实现
- 支持在vs2010~vs2022下编译
- Github CI持续集成
